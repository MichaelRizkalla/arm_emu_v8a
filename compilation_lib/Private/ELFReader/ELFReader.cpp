
#include <ELFReader/ELFReader.h>
#include <InstructionSet/A64InstructionSet.h>
#include <Utility/Bitset.h>
#include <Utility/File.h>
#include <cassert>

BEGIN_NAMESPACE

// Credits: https://github.com/lefticus/cpp_box/blob/master/src/elf_reader.cpp

ELFFile ELFReader::ParseObjectFile(const std::filesystem::path &                  objectFilePath,
                                   std::unique_ptr< std::vector< std::uint8_t > > objectFileData, Object &debugObject) {
    if (objectFileData->size() >= 64) {
        const auto fileHeader = FileHeader { { objectFileData->data(), objectFileData->size() } };
        const auto isElf      = fileHeader.IsELFFile();
        debugObject.Log(LogType::ParsingObjectFile, "{} is ELF?: {}", objectFilePath.string(), isElf);

        if (isElf) {
            const auto &sectionHeaderStringTable = fileHeader.GetSectionHeaderStringTable();

            std::map< std::string, std::uint64_t > sectionOffsets;

            for (const auto &header : fileHeader.GetSectionHeaders()) {
                const auto headerName      = std::string { header.ReadNameFrom(sectionHeaderStringTable) };
                const auto offset          = header.ReadOffset();
                sectionOffsets[headerName] = offset;
                debugObject.Log(LogType::ParsingObjectFile, "Found Section: '{}' at offset: {}", headerName, offset);
            }

            const auto &stringTable = fileHeader.GetStringTable();
            for (const auto &header : fileHeader.GetSectionHeaders()) {
                for (const auto &symbolTableEntry : header.GetSymbolTableEntries()) {
                    if (symbolTableEntry.ReadNameFrom(stringTable) == "main") {

                        ResolveSymbols(*objectFileData.get(), fileHeader, debugObject);

                        const auto mainSection =
                            fileHeader.GetSectionHeader(symbolTableEntry.ReadSectionHeaderTableIndex());
                        const auto mainFileOffset =
                            static_cast< std::uint32_t >(mainSection.ReadOffset() + symbolTableEntry.ReadValue());

                        debugObject.Log(LogType::ParsingObjectFile, "Found 'main' in '{}':{} at offset: {}",
                                        mainSection.ReadNameFrom(sectionHeaderStringTable),
                                        symbolTableEntry.ReadValue(), mainFileOffset);
                        return ELFFile { std::move(objectFileData), fileHeader, sectionOffsets, mainFileOffset, true };
                    }
                }
            }
        }
    }

    debugObject.Log(LogType::ParsingObjectFile, "Did not find 'main', returning invalid binary");
    return ELFFile {
        std::move(objectFileData), { {} }, {}, {}, false
    }; // empty ELFFile, and handing back the object file data
}

void ELFReader::ResolveSymbols(std::vector< std::uint8_t > &data, const FileHeader &fileHeader, Object &debugObject) {

    const auto &sectionHeaderStringTable = fileHeader.GetSectionHeaderStringTable();
    const auto &stringTable              = fileHeader.GetStringTable();
    const auto  symbolTable              = fileHeader.GetSymbolTable();

    for (const auto &sectionHeader : fileHeader.GetSectionHeaders()) {
        if (sectionHeader.ReadType() == SectionHeader::EntryType::SHT_REL &&
            sectionHeader.ReadNameFrom(sectionHeaderStringTable) == ".rel.text") {
            debugObject.Log(LogType::SymbolResolving, "Found .rel.text section");

            const auto sourceSection = [&]() {
                const auto &sectionNameToFind = sectionHeader.ReadNameFrom(sectionHeaderStringTable).substr(4);

                debugObject.Log(LogType::SymbolResolving, "Looking for matching text section '{}'", sectionNameToFind);
                for (const auto &section : fileHeader.GetSectionHeaders()) {
                    if (section.ReadNameFrom(sectionHeaderStringTable) == sectionNameToFind) {
                        return section;
                    }
                }

                debugObject.Log(LogType::SymbolResolving, "'{}' is not find, aborting ...", sectionNameToFind);
                std::terminate();
            }();

            // is a relocation section
            for (const auto &relocation : sectionHeader.GetRelocationEntries()) {
                const auto symbol        = symbolTable.GetSymbolTableEntry(relocation.ReadSymbol());
                const auto targetSection = fileHeader.GetSectionHeader(symbol.ReadSectionHeaderTableIndex());
                const auto from          = relocation.ReadFileOffset() + sourceSection.ReadOffset();
                const auto to            = symbol.ReadValue() + targetSection.ReadOffset();

                debugObject.Log(LogType::SymbolResolving, "Attempting to relocate '{}'@{} -> {} (Offset: {})",
                                symbol.ReadNameFrom(stringTable), from, to, static_cast< std::int32_t >(to - from));

                const auto value = static_cast< std::uint32_t >(data[from]) |
                                   (static_cast< std::uint32_t >(data[from + 1]) << 8) |
                                   (static_cast< std::uint32_t >(data[from + 2]) << 16) |
                                   (static_cast< std::uint32_t >(data[from + 3]) << 24);

                auto isBranchInstruction = [&]() -> bool {
                    auto instruction = Instruction { value };
                    auto decodeBits  = Bitset { 8, static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b1111) &
                                                                              (instruction.Get() >> 25)) };
                    assert(decodeBits.Size() == 8);

                    auto decodeGroup = A64DecodeGroupTable.Lookup(static_cast< std::uint8_t >(decodeBits.ToULong()));
                    if (decodeGroup == A64DecodeGroup::BranchExceptionSystem) {
                        const auto instructionType =
                            BranchExceptionSystemGroup::GetInstance().GetInstructionClass(instruction);

                        if (instructionType == A64BranchExceptionSystemGroup::CONDITIONAL_BRANCHING ||
                            instructionType == A64BranchExceptionSystemGroup::COMPARE_AND_BRANCH_IMMEDIATE ||
                            instructionType == A64BranchExceptionSystemGroup::TEST_AND_BRANCH_IMMEDIATE ||
                            instructionType == A64BranchExceptionSystemGroup::UNCONDITIONAL_BRANCH_IMMEDIATE ||
                            instructionType == A64BranchExceptionSystemGroup::UNCONDITIONAL_BRANCH_REGISTER) {
                            return true;
                        }
                    }
                    return false;
                }();

                if (isBranchInstruction) {
                    const auto newValue =
                        (value & 0xFF000000) | (((static_cast< std::int32_t >(to - from) >> 2) - 2) & 0x00FFFFFF);
                    debugObject.Log(LogType::SymbolResolving, "Resolving Branch Instruction: {:#x} -> {:#x}", value,
                                    newValue);
                    data[from]     = static_cast< uint8_t >(newValue & 0xFF);
                    data[from + 1] = static_cast< uint8_t >((newValue >> 8) & 0xFF);
                    data[from + 2] = static_cast< uint8_t >((newValue >> 16) & 0xFF);
                    data[from + 3] = static_cast< uint8_t >((newValue >> 24) & 0xFF);
                } else if (value == 0) {
                    debugObject.Log(LogType::SymbolResolving, "Instruction is '0', nothing to resolve");
                } else {
                    debugObject.Log(LogType::SymbolResolving, "Unable to resolve instruction: {0:#x}, aborting ...",
                                    value);
                    std::terminate();
                }
            }
        } else if (sectionHeader.ReadType() == SectionHeader::EntryType::SHT_RELA &&
                   sectionHeader.ReadNameFrom(sectionHeaderStringTable) == ".rela.text") {
            debugObject.Log(LogType::SymbolResolving, "Found .rela.text section");

            const auto sourceSection = [&]() {
                const auto &sectionNameToFind = sectionHeader.ReadNameFrom(sectionHeaderStringTable).substr(5);

                debugObject.Log(LogType::SymbolResolving, "Looking for matching text section '{}'", sectionNameToFind);
                for (const auto &section : fileHeader.GetSectionHeaders()) {
                    if (section.ReadNameFrom(sectionHeaderStringTable) == sectionNameToFind) {
                        return section;
                    }
                }

                debugObject.Log(LogType::SymbolResolving, "'{}' is not find, aborting ...", sectionNameToFind);
                std::terminate();
            }();

            // is a relocation section
            for (const auto &relocation : sectionHeader.GetRelocationAddendEntries()) {
                const auto symbol        = symbolTable.GetSymbolTableEntry(relocation.ReadSymbol());
                const auto targetSection = fileHeader.GetSectionHeader(symbol.ReadSectionHeaderTableIndex());
                const auto from = relocation.ReadFileOffset() + sourceSection.ReadOffset() + relocation.ReadAddend();
                const auto to   = symbol.ReadValue() + targetSection.ReadOffset();

                debugObject.Log(LogType::SymbolResolving, "Attempting to relocate '{}'@{} -> {} (Offset: {})",
                                symbol.ReadNameFrom(stringTable), from, to, static_cast< std::int32_t >(to - from));

                const auto value = static_cast< std::uint32_t >(data[from]) |
                                   (static_cast< std::uint32_t >(data[from + 1]) << 8) |
                                   (static_cast< std::uint32_t >(data[from + 2]) << 16) |
                                   (static_cast< std::uint32_t >(data[from + 3]) << 24);

                auto isBranchInstruction = [&]() -> bool {
                    auto instruction = Instruction { value };
                    auto decodeBits  = Bitset { 8, static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b1111) &
                                                                              (instruction.Get() >> 25)) };
                    assert(decodeBits.Size() == 8);

                    auto decodeGroup = A64DecodeGroupTable.Lookup(static_cast< std::uint8_t >(decodeBits.ToULong()));
                    if (decodeGroup == A64DecodeGroup::BranchExceptionSystem) {
                        const auto instructionType =
                            BranchExceptionSystemGroup::GetInstance().GetInstructionClass(instruction);

                        if (instructionType == A64BranchExceptionSystemGroup::CONDITIONAL_BRANCHING ||
                            instructionType == A64BranchExceptionSystemGroup::COMPARE_AND_BRANCH_IMMEDIATE ||
                            instructionType == A64BranchExceptionSystemGroup::TEST_AND_BRANCH_IMMEDIATE ||
                            instructionType == A64BranchExceptionSystemGroup::UNCONDITIONAL_BRANCH_IMMEDIATE ||
                            instructionType == A64BranchExceptionSystemGroup::UNCONDITIONAL_BRANCH_REGISTER) {
                            return true;
                        }
                    }
                    return false;
                }();

                if (isBranchInstruction) {
                    const auto newValue =
                        (value & 0xFF000000) | (((static_cast< std::int32_t >(to - from) >> 2) - 2) & 0x00FFFFFF);
                    debugObject.Log(LogType::SymbolResolving, "Resolving Branch Instruction: {:#x} -> {:#x}", value,
                                    newValue);
                    data[from]     = static_cast< uint8_t >(newValue & 0xFF);
                    data[from + 1] = static_cast< uint8_t >((newValue >> 8) & 0xFF);
                    data[from + 2] = static_cast< uint8_t >((newValue >> 16) & 0xFF);
                    data[from + 3] = static_cast< uint8_t >((newValue >> 24) & 0xFF);
                } else if (value == 0) {
                    debugObject.Log(LogType::SymbolResolving, "Instruction is '0', nothing to resolve");
                } else {
                    debugObject.Log(LogType::SymbolResolving, "Unable to resolve instruction: {0:#x}, aborting ...",
                                    value);
                    std::terminate();
                }
            }
        }
    }
}

END_NAMESPACE
