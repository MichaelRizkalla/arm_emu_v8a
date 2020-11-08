#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <fstream>
#include <filesystem>

#include "A64InstructionSet.h"
#include "A64Instructions.h"
#include "Memory.h"
#include "Registers.h"
#include "A64System.h"

void my_log(arm_emu::DecodeGroupA64 g) {
    using namespace arm_emu;
    switch (g) {
        case DecodeGroupA64::Reserved:
            std::cout << "Reserved\n";
            break;
        case DecodeGroupA64::ScalableVectorExtension: // not yet implemented
            std::cout << "ScalableVectorExtension\n";
            break;
        case DecodeGroupA64::DataProcessingImmediate:
            std::cout << "DataProcessingImmediate\n";
            break;
        case DecodeGroupA64::Branch_Exception_System:
            std::cout << "Branch_Exception_System\n";
            break;
        case DecodeGroupA64::Load_Store:
            std::cout << "Load_Store\n";
            break;
        case DecodeGroupA64::DataProcessingRegister:
            std::cout << "DataProcessingRegister\n";
            break;
        case DecodeGroupA64::DataProcessing_ScalarFloatingPoint_AdvancedSIMD:
            std::cout << "DataProcessing_ScalarFloatingPoint_AdvancedSIMD\n";
            break;
        default:
            break;
    }
}

struct line : public std::string {};
std::istream &operator>>(std::istream &is, line &l)
{
    std::getline(is, l);
    return is;
}

std::uint32_t process_line(line mLine) {
    if (mLine.size() != 11 && mLine.size() != 10) {
        std::cout << "Incorrect instruction format!\n";
        std::cout << mLine << '\n' << mLine.size() << '\n';
        std::exit(2);
    }
    if (mLine[0] != '0' || mLine[1] != 'x') {
        std::cout << "Incorrect instruction format!\n";
        std::exit(2);
    }
    try {
        std::uint32_t return_value = std::stoul(mLine, nullptr, 16);
        return return_value;
    }
    catch (...) {
        std::cout << "Failed to parse instructions!\n";
        std::exit(3);
    }
}

constexpr std::array< std::uint32_t, 1 > single_instruction { { 0b0101'0010'0000'0000'0000'0000'0000'0000 } };

int main(int argc, char *argv[]) {
    using namespace arm_emu;

    if (argc != 2) {
        std::cout << "Incorrect command line arguments\n";
        std::cout << "provide relative path to the instruction file\n";
        std::exit(1);
    }

    std::vector< std::uint32_t > program;
    auto prog_path = std::filesystem::absolute(argv[1]);
    
    std::ifstream instFile(prog_path);
    std::istream_iterator<line> beginf(instFile);
    std::istream_iterator<line> endf;

    for(std::istream_iterator<line> it = beginf; it != endf; ++it)
    {
        program.emplace_back(process_line(*it));
    }

    std::cout << "Starting system: \n\n\n";
    System sys(1024, program);

    std::uint32_t        opCount = 0;
    System::LogSignature logger  = [&opCount]([[maybe_unused]] auto sys, [[maybe_unused]] auto pc, [[maybe_unused]] auto decode, [[maybe_unused]] auto base,
                                             [[maybe_unused]] auto gp, [[maybe_unused]] auto sp, [[maybe_unused]] auto fp) {
        my_log(decode);
        std::cout << "Program counter: " << pc << '\n';
        std::bitset< 32 > inst = base.data();
        std::cout << "Instruction: " << inst << '\n';
        opCount++;
    };

    sys.run(logger, 5);

    std::cout << opCount << " instructions were processed!\n";
}