#if !defined(RELOCATIONADDENDENTRY_H_INCLUDED_1C56DFC6_AF40_4A41_A06C_9EFC15DAB482)
    #define RELOCATIONADDENDENTRY_H_INCLUDED_1C56DFC6_AF40_4A41_A06C_9EFC15DAB482

// Credits: https://github.com/lefticus/cpp_box/blob/master/include/cpp_box/elf_reader.hpp
// https://refspecs.linuxbase.org/elf/gabi4+/ch4.reloc.html

    #include <API/Api.h>
    #include <ELFReader/Entries/BaseEntry.h>
    #include <cstdint>
    #include <string_view>

BEGIN_NAMESPACE

struct [[nodiscard]] RelocationAddendEntry final : public BaseEntry {

    enum class Entry : std::uint32_t
    {
        FileOffset = 0,
        Info       = 1,
        Addend     = 2,
    };

    RelocationAddendEntry(bool is32Bits, bool isLittleEndian, std::basic_string_view< std::uint8_t > data);

    [[nodiscard]] std::size_t GetEntryOffset(std::uint32_t entry) const noexcept final;

    [[nodiscard]] std::size_t GetEntrySize(std::uint32_t entry) const noexcept final;

    [[nodiscard]] std::uint64_t Read(std::uint32_t entry) const noexcept final;

    [[nodiscard]] std::uint64_t ReadFileOffset() const noexcept;

    [[nodiscard]] std::uint64_t ReadInfo() const noexcept;

    [[nodiscard]] std::uint64_t ReadAddend() const noexcept;

    [[nodiscard]] std::uint64_t ReadSymbol() const noexcept;

    [[nodiscard]] std::uint64_t GetType() const noexcept;

    [[nodiscard]] static std::size_t GetRelocationAddendEntrySize(const bool is32Bits) noexcept;

  private:
    bool                                   m_is32Bits;
    bool                                   m_isLittleEndian;
    std::basic_string_view< std::uint8_t > m_data;
};

END_NAMESPACE

#endif // !defined(RELOCATIONADDENDENTRY_H_INCLUDED_1C56DFC6_AF40_4A41_A06C_9EFC15DAB482)
