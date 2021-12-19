#if !defined(DATAUNIT_H_INCLUDED_F109F336_457F_4137_A7FC_4408CACA84A8)
    #define DATAUNIT_H_INCLUDED_F109F336_457F_4137_A7FC_4408CACA84A8

    #include <API/Api.h>
    #include <initializer_list>
    #include <vector>

namespace arm_emu {

    template < class Unit >
    struct [[nodiscard]] DataBlock {
        constexpr DataBlock() = default;
        constexpr DataBlock(size_t size, Unit initVal) : m_data(std::pmr::vector< Unit >(size, initVal)) {
        }
        constexpr DataBlock(std::initializer_list< Unit > lst) : m_data(std::move(lst)) {
        }
        constexpr DataBlock(size_t size, const Unit* const ptr) : m_data(std::pmr::vector< Unit >(size)) {
            std::copy(ptr, ptr + size, m_data.data());
        }

        [[nodiscard]] constexpr decltype(auto) At(size_t loc) const {
            assert(loc < m_data.size());
            return m_data.at(loc);
        }

        [[nodiscard]] constexpr decltype(auto) At(size_t loc) {
            assert(loc < m_data.size());
            return m_data.at(loc);
        }

        [[nodiscard]] constexpr decltype(auto) Get() const noexcept {
            return m_data;
        }

        [[nodiscard]] constexpr decltype(auto) Get() noexcept {
            return m_data;
        }

        [[nodiscard]] constexpr decltype(auto) Data() const noexcept {
            return m_data.data();
        }

        [[nodiscard]] constexpr decltype(auto) Data() noexcept {
            return m_data.data();
        }

        [[nodiscard]] constexpr decltype(auto) Begin() const noexcept {
            return m_data.begin();
        }

        [[nodiscard]] constexpr decltype(auto) Begin() noexcept {
            return m_data.begin();
        }

        [[nodiscard]] constexpr decltype(auto) End() const noexcept {
            return m_data.end();
        }

        [[nodiscard]] constexpr decltype(auto) End() noexcept {
            return m_data.end();
        }

        [[nodiscard]] constexpr size_t Size() const noexcept {
            return m_data.size();
        }

      private:
        std::pmr::vector< Unit > m_data {};
    };

    template < class Unit >
    [[nodiscard]] constexpr decltype(auto) begin(DataBlock< Unit >& block) noexcept {
        return block.Begin();
    }

    template < class Unit >
    [[nodiscard]] constexpr decltype(auto) begin(const DataBlock< Unit >& block) noexcept {
        return block.Begin();
    }

    template < class Unit >
    [[nodiscard]] constexpr decltype(auto) end(DataBlock< Unit >& block) noexcept {
        return block.End();
    }

    template < class Unit >
    [[nodiscard]] constexpr decltype(auto) end(const DataBlock< Unit >& block) noexcept {
        return block.End();
    }

} // namespace arm_emu

#endif // !defined(DATAUNIT_H_INCLUDED_F109F336_457F_4137_A7FC_4408CACA84A8)
