#pragma once
#include <stdexcept>
#include <memory>

#include <utilities.hpp>

namespace arm_emu {

// Byte-Addressed RAM
template < typename AddressingMode >
requires std::integral< AddressingMode > struct Ram {

    using type = AddressingMode;

    template < typename Ty >
        requires std::integral< std::remove_cvref_t< Ty > > && (!enum_type< Ty >)constexpr Ram(Ty size__) : size_(static_cast< std::size_t >(size__)) {
        data = std::make_unique< AddressingMode[] >(static_cast< AddressingMode >(size_));
    }
    ~Ram()                      = default;
    Ram(const Ram&)             = delete;
    Ram(Ram&&)                  = delete;
    Ram& operator==(const Ram&) = delete;
    Ram& operator==(Ram&&)      = delete;

    constexpr auto operator[](const std::size_t& loc) const {
        if (nullptr == data) throw ram_not_initialized {};
        if (size_ < loc) throw std::out_of_range { "Location is out of ram's boundary!" };
        return data.get()[loc];
    }

    inline constexpr auto read_loc(const std::size_t& loc) {
        if (nullptr == data) throw ram_not_initialized {};
        if (size_ < loc) throw std::out_of_range { "Location is out of ram's boundary!" };
        return data.get()[loc];
    }

    constexpr auto write_loc(const std::size_t& loc, AddressingMode inData) {
        if (nullptr == data) throw ram_not_initialized {};
        if (size_ < loc) throw std::out_of_range { "Location is out of ram's boundary!" };
        data.get()[loc] = inData;
    }

    inline constexpr auto reset() noexcept {
        for (auto i = 0ull; i < size_; i++) data.get()[i] = 0;
    }

    template < typename Ty >
    requires std::same_as< typename Ty::value_type, AddressingMode >&& iteratable< Ty >&&
                                                                         function_in< size_checker, Ty, typename Ty::size_type(void) > constexpr auto
                                                                         write_block(const std::size_t& startLoc, Ty inData) {
        if (nullptr == data) throw ram_not_initialized {};
        if (size_ < startLoc) throw std::out_of_range { "Starting location is out of ram's boundary!" };
        if (size_ < (startLoc + inData.size())) throw std::out_of_range { "Data exceeds ram's boundary!" };

        auto loc = startLoc;
        auto ptr = data.get();
        for (auto&& e : inData) { ptr[loc++] = e; }
    }

    template < typename Ty >
        requires std::integral< std::remove_cvref_t< Ty > > && (!enum_type< Ty >) [[nodiscard]] inline constexpr auto read_block(Ty startLoc, Ty blockSize) const {
        if (nullptr == data) throw ram_not_initialized {};
        if (size_ < startLoc) throw std::out_of_range { "Starting location is out of ram's boundary!" };
        auto readEnd = startLoc + blockSize;
        if (size_ < readEnd) throw std::out_of_range { "Data exceeds ram's boundary!" };

        auto        loc      = startLoc;
        std::size_t i        = 0;
        auto        data_ptr = data.get();
        auto        out      = std::make_unique< AddressingMode >(blockSize);
        auto        out_ptr  = out.get();
        while (loc < readEnd) { out_ptr[i++] = data_ptr[loc++]; }
        return out;
    }

    inline constexpr auto size() const noexcept { return size_; }

  protected:
    std::unique_ptr< AddressingMode[] > data;
    std::size_t                          size_;
};
} // namespace arm_emu