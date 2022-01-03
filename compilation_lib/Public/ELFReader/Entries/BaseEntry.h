#if !defined(BASEENTRY_H_INCLUDED_21A4A9B4_57BE_4E5B_8D20_8212424E717D)
    #define BASEENTRY_H_INCLUDED_21A4A9B4_57BE_4E5B_8D20_8212424E717D

// Credits: https://github.com/lefticus/cpp_box/blob/master/include/cpp_box/elf_reader.hpp

    #include <API/Api.h>
    #include <cstddef>
    #include <cstdint>
    #include <functional>
    #include <memory_resource>

namespace arm_emu {

    struct [[nodiscard]] ARMEMU_API BaseEntry {
      public:
        template < class T >
        struct Iterator {
            using iterator_category = std::forward_iterator_tag;

            Iterator(const size_t index, std::function< T(const std::size_t) > callable) :
                m_callable(std::move(callable)), m_index(index) {
            }

            [[nodiscard]] T operator*() const noexcept {
                return m_callable(m_index);
            }

            [[nodiscard]] T operator->() const noexcept {
                return m_callable(m_index);
            }

            Iterator &operator++() noexcept {
                ++m_index;
                return *this;
            }

            Iterator operator++(int) noexcept {
                Iterator tmp = *this;
                ++*this;
                return tmp;
            }

            [[nodiscard]] bool operator!=(const Iterator &rhs) const noexcept {
                return m_index != rhs.m_index;
            }

            [[nodiscard]] bool operator==(const Iterator &rhs) const noexcept {
                return m_index == rhs.m_index;
            }

          private:
            std::function< T(const std::size_t) > m_callable;
            size_t                                m_index;
        };

        template < class T >
        struct Iterable {

            Iterable(std::function< T(const std::size_t) > callable, const size_t maxIterations) :
                m_callable(std::move(callable)), m_maxIterations(maxIterations) {
            }

            [[nodiscard]] Iterator< T > begin() const noexcept {
                return Iterator< T > { 0, m_callable };
            }
            [[nodiscard]] Iterator< T > end() const noexcept {
                return Iterator< T > { m_maxIterations, m_callable };
            }

            std::function< T(const std::size_t) > m_callable;
            size_t                                m_maxIterations;
        };

        template < std::size_t Bytes, class Type >
        inline static std::uint64_t ReadFrom(const Type &data, const std::size_t location,
                                             const bool isLittleEndian = true) noexcept {
            static_assert(Bytes == 1 || Bytes == 2 || Bytes == 4 || Bytes == 8);

            if (Bytes == 1) {
                return static_cast< std::uint8_t >(data[location]);
            }

            if (Bytes == 2) {
                const std::uint16_t byte0 = data[location];
                const std::uint16_t byte1 = data[location + 1];
                if (isLittleEndian) {
                    return static_cast< std::uint16_t >(byte0 | (byte1 << 8));
                } else {
                    return static_cast< std::uint16_t >((byte0 << 8) | byte1);
                }
            }

            if (Bytes == 4) {
                const std::uint32_t byte0 = data[location];
                const std::uint32_t byte1 = data[location + 1];
                const std::uint32_t byte2 = data[location + 2];
                const std::uint32_t byte3 = data[location + 3];
                if (isLittleEndian) {
                    return byte0 | (byte1 << 8) | (byte2 << 16) | (byte3 << 24);
                } else {
                    return (byte0 << 24) | (byte1 << 16) | (byte2 << 8) | byte3;
                }
            }

            if (Bytes == 8) {
                const std::uint64_t byte0 = data[location];
                const std::uint64_t byte1 = data[location + 1];
                const std::uint64_t byte2 = data[location + 2];
                const std::uint64_t byte3 = data[location + 3];
                const std::uint64_t byte4 = data[location + 4];
                const std::uint64_t byte5 = data[location + 5];
                const std::uint64_t byte6 = data[location + 6];
                const std::uint64_t byte7 = data[location + 7];
                if (isLittleEndian) {
                    return byte0 | (byte1 << 8) | (byte2 << 16) | (byte3 << 24) | (byte4 << 32) | (byte5 << 40) |
                           (byte6 << 48) | (byte7 << 56);
                } else {
                    return (byte0 << 56) | (byte1 << 48) | (byte2 << 40) | (byte3 << 32) | (byte4 << 24) |
                           (byte5 << 16) | (byte6 << 8) | byte7;
                }
            }
        }

        /// @brief Get the offset of the entry in the binary file
        /// @param entry entry type
        /// @return offset from index 0
        virtual std::size_t GetEntryOffset(std::uint32_t entry) const noexcept = 0;

        /// @brief Get entry size in bytes
        /// @param entry entry type
        /// @return size in bytes
        virtual std::size_t GetEntrySize(std::uint32_t entry) const noexcept = 0;

        /// @brief Read the entry data
        /// @param entry entry type
        /// @return entry data
        virtual std::uint64_t Read(std::uint32_t entry) const noexcept = 0;
    };

} // namespace arm_emu

#endif // !defined(BASEENTRY_H_INCLUDED_21A4A9B4_57BE_4E5B_8D20_8212424E717D)
