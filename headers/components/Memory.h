#pragma once
#include <stdexcept>
#include <memory>

#include <utilities.hpp>

namespace arm_emu {

// Byte-Addressed RAM
template < typename _AddressingMode >
requires std::integral< _AddressingMode > struct Ram {

    using type = _AddressingMode;

    template < typename _Ty >
        requires std::integral< std::remove_cvref_t< _Ty > > && (!enum_type< _Ty >)constexpr Ram(_Ty size__) : size_(static_cast< std::size_t >(size__)) {
        data = std::make_unique< _AddressingMode[] >(static_cast< _AddressingMode >(size_));
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

    constexpr auto write_loc(const std::size_t& loc, _AddressingMode inData) {
        if (nullptr == data) throw ram_not_initialized {};
        if (size_ < loc) throw std::out_of_range { "Location is out of ram's boundary!" };
        data.get()[loc] = inData;
    }

    inline constexpr auto reset() noexcept {
        for (auto i = 0ull; i < size_; i++) data.get()[i] = 0;
    }

    template < typename _Ty >
    requires std::same_as< typename _Ty::value_type, _AddressingMode >&& iteratable< _Ty >&&
                                                                         function_in< size_checker, _Ty, typename _Ty::size_type(void) > constexpr auto
                                                                         write_block(const std::size_t& startLoc, _Ty inData) {
        if (nullptr == data) throw ram_not_initialized {};
        if (size_ < startLoc) throw std::out_of_range { "Starting location is out of ram's boundary!" };
        if (size_ < (startLoc + inData.size())) throw std::out_of_range { "Data exceeds ram's boundary!" };

        auto loc = startLoc;
        auto ptr = data.get();
        for (auto&& e : inData) { ptr[loc++] = e; }
    }

    template < typename _Ty >
        requires std::integral< std::remove_cvref_t< _Ty > > && (!enum_type< _Ty >) [[nodiscard]] inline constexpr auto read_block(_Ty startLoc, _Ty blockSize) const {
        if (nullptr == data) throw ram_not_initialized {};
        if (size_ < startLoc) throw std::out_of_range { "Starting location is out of ram's boundary!" };
        auto readEnd = startLoc + blockSize;
        if (size_ < readEnd) throw std::out_of_range { "Data exceeds ram's boundary!" };

        auto        loc      = startLoc;
        std::size_t i        = 0;
        auto        data_ptr = data.get();
        auto        out      = std::make_unique< _AddressingMode >(blockSize);
        auto        out_ptr  = out.get();
        while (loc < readEnd) { out_ptr[i++] = data_ptr[loc++]; }
        return out;
    }

    inline constexpr auto size() const noexcept { return size_; }

  protected:
    std::unique_ptr< _AddressingMode[] > data;
    std::size_t                          size_;
};
} // namespace arm_emu