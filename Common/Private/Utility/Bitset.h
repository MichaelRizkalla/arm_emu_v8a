#if !defined(DYNBITSET_H_INCLUDED_A0863C53_28F4_403D_A2F2_CA32D9D2D51B)
    #define DYNBITSET_H_INCLUDED_A0863C53_28F4_403D_A2F2_CA32D9D2D51B

    #include <API/Api.h>
    #include <array>
    #include <bit>
    #include <bitset>
    #include <boost/dynamic_bitset.hpp>
    #include <cstdint>
    #include <memory_resource>
    #include <variant>

BEGIN_NAMESPACE

class Bitset {
    static constexpr std::size_t Small_bit_set_size = 32;

    static constexpr std::size_t Invalid_bitset_index = 0;
    static constexpr std::size_t Static_bitset_index  = 1;
    static constexpr std::size_t Dynamic_bitset_index = 2;

    using dynamic_bitset = ::boost::dynamic_bitset< std::uint64_t, std::pmr::polymorphic_allocator< std::uint64_t > >;
    using static_bitset  = std::bitset< Small_bit_set_size >;

    using dynamic_reference = typename dynamic_bitset::reference;
    using static_reference  = typename static_bitset::reference;

    using bitset_type = std::variant< std::monostate, static_bitset, dynamic_bitset >;

  public:
    class reference {
      public:
        reference(static_reference ref) : m_reference(std::move(ref)) {
        }

        reference(dynamic_reference ref) : m_reference(std::move(ref)) {
        }

        reference& operator=(bool value) noexcept {
            if (m_reference.index() == Static_reference_index) {
                std::get< Static_reference_index >(m_reference) = value;
            } else {
                std::get< Dynamic_reference_index >(m_reference) = value;
            }
            return *this;
        }

        reference& operator=(const reference& ref) noexcept {
            if (m_reference.index() == Static_reference_index) {
                std::get< Static_reference_index >(m_reference) = ref;
            } else {
                std::get< Dynamic_reference_index >(m_reference) = ref;
            }
            return *this;
        }

        reference& flip() noexcept {
            if (m_reference.index() == Static_reference_index) {
                std::get< Static_reference_index >(m_reference).flip();
            } else {
                std::get< Dynamic_reference_index >(m_reference).flip();
            }
            return *this;
        }

        [[nodiscard]] bool operator~() const noexcept {
            if (m_reference.index() == Static_reference_index) {
                return ~std::get< Static_reference_index >(m_reference);
            } else {
                return ~std::get< Dynamic_reference_index >(m_reference);
            }
        }

        operator bool() const noexcept {
            if (m_reference.index() == Static_reference_index) {
                return static_cast< bool >(std::get< Static_reference_index >(m_reference));
            } else {
                return static_cast< bool >(std::get< Dynamic_reference_index >(m_reference));
            }
        }

      private:
        static constexpr std::size_t Static_reference_index  = 0;
        static constexpr std::size_t Dynamic_reference_index = 1;

        std::variant< static_reference, dynamic_reference > m_reference;
    };

    /// <summary>
    /// Construct empty bitset
    /// </summary>
    explicit Bitset() = default;

    explicit Bitset(dynamic_bitset dynBitset) : m_bitset(std::move(dynBitset)) {
        m_size = Size();
    }

    explicit Bitset(static_bitset staticBitset) : m_size(Small_bit_set_size), m_bitset(std::move(staticBitset)) {
    }

    explicit Bitset(std::size_t num_bits, unsigned long long value = 0) :
        m_size(num_bits), m_bitset(ConstructBitset(num_bits, value)) {
    }

    template < typename CharT, typename Traits, typename Alloc >
    explicit Bitset(const std::basic_string< CharT, Traits, Alloc >&              str,
                    typename std::basic_string< CharT, Traits, Alloc >::size_type pos = 0,
                    typename std::basic_string< CharT, Traits, Alloc >::size_type n =
                        std::basic_string< CharT, Traits, Alloc >::npos) :
        m_size(std::min(str.size() - pos, n)), m_bitset(ConstructBitset(str, pos, n)) {
    }

    Bitset& operator=(const Bitset&) = default;
    Bitset& operator=(Bitset&&) = default;
    Bitset(const Bitset&)       = default;
    Bitset(Bitset&&)            = default;

    Bitset& operator=(std::uint32_t value) {
        if (m_bitset.index() == Static_bitset_index) {
            std::get< Static_bitset_index >(m_bitset) = value;
        } else {
            auto& dynSet = std::get< Dynamic_bitset_index >(m_bitset);
            dynSet.clear();
            for (std::size_t idx = 0; idx < dynSet.size(); ++idx) {
                dynSet[idx] |= ((static_cast< std::uint32_t >(1) << idx) & value);
            }
        }

        return *this;
    }

    void swap(Bitset& other) noexcept {
        std::swap(m_bitset, other.m_bitset);
        std::swap(m_size, other.m_size);
    }

    bool operator==(const Bitset& rhs) const {
        assert(IsValid() && rhs.IsValid());
        assert(Size() == rhs.Size());

        if (m_bitset.index() == Static_bitset_index && rhs.m_bitset.index() == Static_bitset_index) {
            return std::get< Static_bitset_index >(m_bitset) == std::get< Static_bitset_index >(rhs.m_bitset);
        } else if (m_bitset.index() == Dynamic_bitset_index && rhs.m_bitset.index() == Dynamic_bitset_index) {
            return std::get< Dynamic_bitset_index >(m_bitset) == std::get< Dynamic_bitset_index >(rhs.m_bitset);
        } else if (m_bitset.index() == Static_bitset_index && rhs.m_bitset.index() == Dynamic_bitset_index) {
            auto& dynSet    = std::get< Dynamic_bitset_index >(rhs.m_bitset);
            auto& staticSet = std::get< Static_bitset_index >(m_bitset);

            auto staticSetValue = staticSet.to_ulong();

            auto diff = rhs.GetUpperUInt32() & staticSetValue;
            auto pos  = dynSet.find_next(Small_bit_set_size);
            return diff == 0 && pos == dynamic_bitset::npos;
        } else {
            auto& dynSet    = std::get< Dynamic_bitset_index >(m_bitset);
            auto& staticSet = std::get< Static_bitset_index >(rhs.m_bitset);

            auto staticSetValue = staticSet.to_ulong();

            auto diff = GetUpperUInt32() & staticSetValue;
            auto pos  = dynSet.find_next(Small_bit_set_size);
            return diff == 0 && pos == dynamic_bitset::npos;
        }
    }

    bool operator!=(const Bitset& rhs) const {
        return !(*this == rhs);
    }

    bool operator==(std::uint64_t rhs) const {
        std::uint64_t myself = ToULLong();
        return myself == rhs;
    }

    bool operator!=(std::uint64_t rhs) const {
        return !(*this == rhs);
    }

    bool operator==(std::int64_t rhs) const {
        std::uint64_t myself = ToULLong();
        return myself == std::bit_cast< std::uint64_t >(rhs);
    }

    bool operator!=(std::int64_t rhs) const {
        return !(*this == rhs);
    }

    bool operator==(std::uint32_t rhs) const {
        std::uint32_t myself = ToULong();
        return myself == rhs;
    }

    bool operator!=(std::uint32_t rhs) const {
        return !(*this == rhs);
    }

    bool operator==(std::int32_t rhs) const {
        std::uint32_t myself = ToULong();
        return myself == std::bit_cast< std::uint32_t >(rhs);
    }

    bool operator!=(std::int32_t rhs) const {
        return !(*this == rhs);
    }

    bool operator[](std::size_t pos) const {
        assert(IsValid());
        assert(m_size > pos);

        if (m_bitset.index() == Static_bitset_index) {
            return std::get< Static_bitset_index >(m_bitset)[pos];
        } else {
            return std::get< Dynamic_bitset_index >(m_bitset)[pos];
        }
    }

    reference operator[](std::size_t pos) {
        assert(IsValid());
        assert(m_size > pos);

        if (m_bitset.index() == Static_bitset_index) {
            return reference { std::get< Static_bitset_index >(m_bitset)[pos] };
        } else {
            return reference { std::get< Dynamic_bitset_index >(m_bitset)[pos] };
        }
    }

    bool Test(std::size_t pos) const {
        assert(IsValid());
        assert(m_size > pos);

        if (m_bitset.index() == Static_bitset_index) {
            return std::get< Static_bitset_index >(m_bitset).test(pos);
        } else {
            return std::get< Dynamic_bitset_index >(m_bitset).test(pos);
        }
    }

    bool All() const {
        assert(IsValid());

        if (m_bitset.index() == Static_bitset_index) {
            return std::get< Static_bitset_index >(m_bitset).all();
        } else {
            return std::get< Dynamic_bitset_index >(m_bitset).all();
        }
    }

    bool Any() const {
        assert(IsValid());

        if (m_bitset.index() == Static_bitset_index) {
            return std::get< Static_bitset_index >(m_bitset).any();
        } else {
            return std::get< Dynamic_bitset_index >(m_bitset).any();
        }
    }

    bool None() const {
        assert(IsValid());

        if (m_bitset.index() == Static_bitset_index) {
            return std::get< Static_bitset_index >(m_bitset).none();
        } else {
            return std::get< Dynamic_bitset_index >(m_bitset).none();
        }
    }

    std::size_t Count() const noexcept {
        assert(IsValid());

        if (m_bitset.index() == Static_bitset_index) {
            return std::get< Static_bitset_index >(m_bitset).count();
        } else {
            return std::get< Dynamic_bitset_index >(m_bitset).count();
        }
    }

    std::size_t Size() const noexcept {
        assert(IsValid());

        if (m_bitset.index() == Static_bitset_index) {
            return m_size;
        } else {
            return std::get< Dynamic_bitset_index >(m_bitset).size();
        }
    }

    Bitset& operator&=(const Bitset& rhs) {
        assert(IsValid() && rhs.IsValid());
        assert(Size() == rhs.Size());

        if (m_bitset.index() == Static_bitset_index && rhs.m_bitset.index() == Static_bitset_index) {
            auto& staticSet    = std::get< Static_bitset_index >(m_bitset);
            auto& rhsStaticSet = std::get< Static_bitset_index >(rhs.m_bitset);

            staticSet &= rhsStaticSet;
            m_size = std::max(m_size, rhs.m_size);
            return *this;
        } else if (m_bitset.index() == Dynamic_bitset_index && rhs.m_bitset.index() == Dynamic_bitset_index) {
            auto& dynSet    = std::get< Dynamic_bitset_index >(m_bitset);
            auto& rhsDynSet = std::get< Dynamic_bitset_index >(m_bitset);

            dynSet &= rhsDynSet;
            m_size = dynSet.size();
            return *this;
        } else if (m_bitset.index() == Static_bitset_index && rhs.m_bitset.index() == Dynamic_bitset_index) {
            const auto& dynSet    = std::get< Dynamic_bitset_index >(rhs.m_bitset);
            auto&       staticSet = std::get< Static_bitset_index >(m_bitset);

            auto dynSetValue = rhs.GetUpperUInt32();

            staticSet &= dynSetValue;
            // m_size = m_size;
            return *this;
        } else {
            auto& dynSet    = std::get< Dynamic_bitset_index >(m_bitset);
            auto& staticSet = std::get< Static_bitset_index >(rhs.m_bitset);

            auto dynSetValue    = GetUpperUInt32();
            auto staticSetValue = staticSet.to_ulong();
            dynSet.reset();
            std::uint32_t result = dynSetValue & staticSetValue;

            for (auto idx = 0; idx < Small_bit_set_size; ++idx) {
                dynSet[idx] = static_cast< bool >(result & std::uint32_t { 1 } << idx);
            }

            m_size = dynSet.size();
            return *this;
        }
    }

    Bitset& operator|=(const Bitset& rhs) {
        assert(IsValid() && rhs.IsValid());
        assert(Size() == rhs.Size());

        if (m_bitset.index() == Static_bitset_index && rhs.m_bitset.index() == Static_bitset_index) {
            auto& staticSet    = std::get< Static_bitset_index >(m_bitset);
            auto& rhsStaticSet = std::get< Static_bitset_index >(rhs.m_bitset);

            staticSet |= rhsStaticSet;
            m_size = std::max(m_size, rhs.m_size);
            return *this;
        } else if (m_bitset.index() == Dynamic_bitset_index && rhs.m_bitset.index() == Dynamic_bitset_index) {
            auto& dynSet    = std::get< Dynamic_bitset_index >(m_bitset);
            auto& rhsDynSet = std::get< Dynamic_bitset_index >(m_bitset);

            dynSet |= rhsDynSet;
            m_size = dynSet.size();
            return *this;
        } else if (m_bitset.index() == Static_bitset_index && rhs.m_bitset.index() == Dynamic_bitset_index) {
            auto& rhsDynSet      = std::get< Dynamic_bitset_index >(rhs.m_bitset);
            auto& staticSet      = std::get< Static_bitset_index >(m_bitset);
            auto  staticSetValue = staticSet.to_ulong();

            m_bitset       = dynamic_bitset { Small_bit_set_size, staticSetValue };
            auto& myDynSet = std::get< Dynamic_bitset_index >(m_bitset);

            myDynSet |= rhsDynSet;
            m_size = myDynSet.size();
            return *this;
        } else {
            auto& dynSet    = std::get< Dynamic_bitset_index >(m_bitset);
            auto& staticSet = std::get< Static_bitset_index >(rhs.m_bitset);

            auto          dynSetValue    = GetUpperUInt32();
            auto          staticSetValue = staticSet.to_ulong();
            std::uint32_t result         = dynSetValue | staticSetValue;

            for (auto idx = 0; idx < Small_bit_set_size; ++idx) {
                dynSet[idx] = static_cast< bool >(result & std::uint32_t { 1 } << idx);
            }

            m_size = dynSet.size();
            return *this;
        }
    }

    Bitset& operator^=(const Bitset& rhs) {
        assert(IsValid() && rhs.IsValid());
        assert(Size() == rhs.Size());

        if (m_bitset.index() == Static_bitset_index && rhs.m_bitset.index() == Static_bitset_index) {
            auto& staticSet    = std::get< Static_bitset_index >(m_bitset);
            auto& rhsStaticSet = std::get< Static_bitset_index >(rhs.m_bitset);

            staticSet ^= rhsStaticSet;
            m_size = std::max(m_size, rhs.m_size);
            return *this;
        } else if (m_bitset.index() == Dynamic_bitset_index && rhs.m_bitset.index() == Dynamic_bitset_index) {
            auto& dynSet    = std::get< Dynamic_bitset_index >(m_bitset);
            auto& rhsDynSet = std::get< Dynamic_bitset_index >(m_bitset);

            dynSet ^= rhsDynSet;
            m_size = dynSet.size();
            return *this;
        } else if (m_bitset.index() == Static_bitset_index && rhs.m_bitset.index() == Dynamic_bitset_index) {
            auto& rhsDynSet      = std::get< Dynamic_bitset_index >(rhs.m_bitset);
            auto& staticSet      = std::get< Static_bitset_index >(m_bitset);
            auto  staticSetValue = staticSet.to_ulong();

            m_bitset       = dynamic_bitset { Small_bit_set_size, staticSetValue };
            auto& myDynSet = std::get< Dynamic_bitset_index >(m_bitset);

            myDynSet ^= rhsDynSet;
            m_size = myDynSet.size();
            return *this;
        } else {
            auto& rhsStaticSet   = std::get< Static_bitset_index >(rhs.m_bitset);
            auto& myDynSet       = std::get< Dynamic_bitset_index >(m_bitset);
            auto  staticSetValue = rhsStaticSet.to_ulong();

            auto aDynSet = dynamic_bitset { Small_bit_set_size, staticSetValue };

            myDynSet ^= aDynSet;
            m_size = myDynSet.size();
            return *this;
        }
    }

    Bitset operator~() const {
        assert(IsValid());

        if (m_bitset.index() == Static_bitset_index) {
            auto& staticSet = std::get< Static_bitset_index >(m_bitset);
            auto result     = staticSet.operator~();
            return Bitset { result };
        } else {
            auto& dynSet = std::get< Dynamic_bitset_index >(m_bitset);
            auto result  = dynSet.operator~();
            return Bitset { result };
        }
    }

    Bitset operator<<(std::size_t pos) const {
        Bitset result = *this;
        result <<= pos;
        return result;
    }

    Bitset& operator<<=(std::size_t pos) {
        assert(IsValid());

        if (m_bitset.index() == Static_bitset_index) {
            auto& staticSet = std::get< Static_bitset_index >(m_bitset);
            staticSet <<= pos;
            return *this;
        } else {
            auto& dynSet = std::get< Dynamic_bitset_index >(m_bitset);
            dynSet <<= pos;
            return *this;
        }
    }

    Bitset operator>>(std::size_t pos) const {
        Bitset result = *this;
        result >>= pos;
        return result;
    }

    Bitset& operator>>=(std::size_t pos) {
        assert(IsValid());

        if (m_bitset.index() == Static_bitset_index) {
            auto& staticSet = std::get< Static_bitset_index >(m_bitset);
            staticSet >>= pos;
            return *this;
        } else {
            auto& dynSet = std::get< Dynamic_bitset_index >(m_bitset);
            dynSet >>= pos;
            return *this;
        }
    }

    Bitset& Set() {
        assert(IsValid());

        if (m_bitset.index() == Static_bitset_index) {
            auto& staticSet = std::get< Static_bitset_index >(m_bitset);
            staticSet.set();
            return *this;
        } else {
            auto& dynSet = std::get< Dynamic_bitset_index >(m_bitset);
            dynSet.set();
            return *this;
        }
    }

    Bitset& Set(std::size_t pos, bool value = true) {
        assert(IsValid());
        assert(m_size > pos);

        if (m_bitset.index() == Static_bitset_index) {
            auto& staticSet = std::get< Static_bitset_index >(m_bitset);
            staticSet.set(pos, value);
            return *this;
        } else {
            auto& dynSet = std::get< Dynamic_bitset_index >(m_bitset);
            dynSet.set(pos, value);
            return *this;
        }
    }

    Bitset& Reset() {
        assert(IsValid());

        if (m_bitset.index() == Static_bitset_index) {
            auto& staticSet = std::get< Static_bitset_index >(m_bitset);
            staticSet.reset();
            return *this;
        } else {
            auto& dynSet = std::get< Dynamic_bitset_index >(m_bitset);
            dynSet.reset();
            return *this;
        }
    }

    Bitset& Reset(std::size_t pos) {
        assert(IsValid());
        assert(m_size > pos);

        if (m_bitset.index() == Static_bitset_index) {
            auto& staticSet = std::get< Static_bitset_index >(m_bitset);
            staticSet.reset(pos);
            return *this;
        } else {
            auto& dynSet = std::get< Dynamic_bitset_index >(m_bitset);
            dynSet.reset(pos);
            return *this;
        }
    }

    Bitset& Flip() {
        assert(IsValid());

        if (m_bitset.index() == Static_bitset_index) {
            auto& staticSet = std::get< Static_bitset_index >(m_bitset);
            staticSet.flip();
            return *this;
        } else {
            auto& dynSet = std::get< Dynamic_bitset_index >(m_bitset);
            dynSet.flip();
            return *this;
        }
    }

    Bitset& Flip(std::size_t pos) {
        assert(IsValid());
        assert(m_size > pos);

        if (m_bitset.index() == Static_bitset_index) {
            auto& staticSet = std::get< Static_bitset_index >(m_bitset);
            staticSet.flip(pos);
            return *this;
        } else {
            auto& dynSet = std::get< Dynamic_bitset_index >(m_bitset);
            dynSet.flip(pos);
            return *this;
        }
    }

    unsigned long ToULong() const {
        assert(IsValid());

        if (m_bitset.index() == Static_bitset_index) {
            auto& staticSet = std::get< Static_bitset_index >(m_bitset);
            return staticSet.to_ulong();
        } else {
            return GetUpperUInt32();
        }
    }

    unsigned long long ToULLong() const {
        assert(IsValid());

        if (m_bitset.index() == Static_bitset_index) {
            auto& staticSet = std::get< Static_bitset_index >(m_bitset);
            return staticSet.to_ullong();
        } else {
            return GetUpperUInt64();
        }
    }

    std::string ToString() const {
        assert(IsValid());

        if (m_bitset.index() == Static_bitset_index) {
            auto&          staticSet = std::get< Static_bitset_index >(m_bitset);
            dynamic_bitset dynSet { m_size, staticSet.to_ulong() };
            std::string    result {};
            boost::to_string(dynSet, result);
            return result;
        } else {
            auto&       dynSet = std::get< Dynamic_bitset_index >(m_bitset);
            std::string result {};
            boost::to_string(dynSet, result);
            return result;
        }
    }

    void Resize(std::size_t num_bits, bool value = 0) {
        if (IsValid()) {
            if (num_bits <= Small_bit_set_size) {
                if (m_bitset.index() == Dynamic_bitset_index) {
                    auto& dynSet = std::get< Dynamic_bitset_index >(m_bitset);
                    dynSet.resize(num_bits, value);
                    auto dynSetValue = dynSet.to_ulong();
                    m_bitset         = static_bitset { dynSetValue };
                    m_size           = num_bits;
                } else {
                    auto& staticSet = std::get< Static_bitset_index >(m_bitset);
                    if (num_bits >= m_size) {
                        while (m_size < num_bits) {
                            staticSet[m_size] = value;
                            ++m_size;
                        }
                    } else {
                        while (m_size > num_bits) {
                            staticSet[m_size - 1] = 0;
                            --m_size;
                        }
                    }
                }
            } else {
                if (m_bitset.index() == Dynamic_bitset_index) {
                    auto& dynSet = std::get< Dynamic_bitset_index >(m_bitset);
                    dynSet.resize(num_bits, value);
                    m_size = dynSet.size();
                } else {
                    auto& staticSet      = std::get< Static_bitset_index >(m_bitset);
                    auto  staticSetValue = staticSet.to_ulong();

                    m_bitset = dynamic_bitset { m_size, staticSetValue };

                    auto& dynSet = std::get< Dynamic_bitset_index >(m_bitset);
                    dynSet.resize(num_bits, value);
                    m_size = dynSet.size();
                }
            }
        } else {
            m_size   = num_bits;
            m_bitset = ConstructBitset(num_bits, value);
        }
    }

    Bitset& Concat(Bitset rhs) {
        auto newSize = Size() + rhs.Size();
        auto rhsSize = rhs.Size();

        Resize(newSize);
        rhs.Resize(newSize);
        *this <<= rhsSize;
        *this |= rhs;

        return *this;
    }

    explicit operator bool() const {
        return Any();
    }

    explicit operator std::uint8_t() const {
        assert(m_size <= 8);
        return static_cast< std::uint8_t >(ToULong());
    }

    explicit operator std::uint32_t() const {
        assert(m_size <= 32);
        return static_cast< std::uint32_t >(ToULong());
    }

    explicit operator std::uint64_t() const {
        assert(m_size <= 64);
        return static_cast< std::uint64_t >(ToULLong());
    }

  private:
    bool IsValid() const noexcept {
        return m_bitset.index() != Invalid_bitset_index;
    }

    static bitset_type ConstructBitset(std::size_t size, unsigned long long value) {
        if (size > Small_bit_set_size) {
            return bitset_type { std::in_place_type< dynamic_bitset >, size, value };
        } else {
            return bitset_type { std::in_place_type< static_bitset >, value };
        }
    }

    template < typename CharT, typename Traits, typename Alloc >
    static bitset_type ConstructBitset(const std::basic_string< CharT, Traits, Alloc >&              str,
                                       typename std::basic_string< CharT, Traits, Alloc >::size_type pos,
                                       typename std::basic_string< CharT, Traits, Alloc >::size_type n) {
        auto size = std::min(str.size() - pos, n);
        if (size > Small_bit_set_size) {
            return bitset_type { std::in_place_type< dynamic_bitset >, str, pos, n };
        } else {
            return bitset_type { std::in_place_type< static_bitset >, str, pos, n };
        }
    }

    std::uint32_t GetUpperUInt32() const {
        assert(m_bitset.index() == Dynamic_bitset_index);

        auto&         set    = std::get< Dynamic_bitset_index >(m_bitset);
        std::uint32_t result = 0;
        for (auto idx = 0; idx < Small_bit_set_size; ++idx) {
            idx |= set.test(idx) ? std::uint32_t { 1 } << idx : 0;
        }

        return result;
    }

    std::uint64_t GetUpperUInt64() const {
        assert(m_bitset.index() == Dynamic_bitset_index);

        auto&         set    = std::get< Dynamic_bitset_index >(m_bitset);
        std::uint64_t result = 0;
        for (auto idx = 0; idx < 64; ++idx) {
            idx |= set.test(idx) ? std::uint64_t { 1 } << idx : 0;
        }

        return result;
    }

    static constexpr std::array< std::uint32_t, Small_bit_set_size + 1 > Small_bit_set_masks {
        0b0,
        0b1,
        0b11,
        0b111,
        0b1111,
        0b11111,
        0b111111,
        0b1111111,
        0b11111111,
        0b111111111,
        0b1111111111,
        0b11111111111,
        0b111111111111,
        0b1111111111111,
        0b11111111111111,
        0b111111111111111,
        0b1111111111111111,
        0b11111111111111111,
        0b111111111111111111,
        0b1111111111111111111,
        0b11111111111111111111,
        0b111111111111111111111,
        0b1111111111111111111111,
        0b11111111111111111111111,
        0b111111111111111111111111,
        0b1111111111111111111111111,
        0b11111111111111111111111111,
        0b111111111111111111111111111,
        0b1111111111111111111111111111,
        0b11111111111111111111111111111,
        0b111111111111111111111111111111,
        0b1111111111111111111111111111111,
        0b11111111111111111111111111111111,
    };

    bitset_type m_bitset {};
    std::size_t m_size { 0 };

    template < class CharT, class Traits >
    friend std::basic_ostream< CharT, Traits >& operator<<(std::basic_ostream< CharT, Traits >& os, const Bitset& x);
    template < class CharT, class Traits >
    friend std::basic_istream< CharT, Traits >& operator>>(std::basic_istream< CharT, Traits >& is, Bitset& x);
};

static void swap(Bitset& lhs, Bitset& rhs) noexcept {
    lhs.swap(rhs);
}

static Bitset operator&(const Bitset& lhs, const Bitset& rhs) {
    return Bitset { lhs } &= rhs;
}

static Bitset operator|(const Bitset& lhs, const Bitset& rhs) {
    return Bitset { lhs } |= rhs;
}

static Bitset operator^(const Bitset& lhs, const Bitset& rhs) {
    return Bitset { lhs } ^= rhs;
}

static Bitset Concat(Bitset lhs, Bitset rhs) {
    return lhs.Concat(rhs);
}

template < class CharT, class Traits >
std::basic_ostream< CharT, Traits >& operator<<(std::basic_ostream< CharT, Traits >& os, const Bitset& in) {
    assert(in.IsValid());

    if (in.m_bitset.index() == Bitset::Static_bitset_index) {
        const auto& staticSet = std::get< Bitset::Static_bitset_index >(in.m_bitset);
        const auto  str       = staticSet.to_string();
        return os << str.substr(str.size() - in.m_size, in.m_size);
    } else {
        const auto& dynSet = std::get< Bitset::Dynamic_bitset_index >(in.m_bitset);
        return os << dynSet;
    }
}

template < class CharT, class Traits >
std::basic_istream< CharT, Traits >& operator>>(std::basic_istream< CharT, Traits >& is, Bitset& in) {
    assert(in.IsValid());

    if (in.m_bitset.index() == Bitset::Static_bitset_index) {
        auto& staticSet = std::get< Bitset::Static_bitset_index >(in.m_bitset);
        return is >> staticSet;
    } else {
        auto& dynSet = std::get< Bitset::Dynamic_bitset_index >(in.m_bitset);
        return is >> dynSet;
    }
}

// Figure a way to write 1 version of concate that mimics the behaviour of the 4
template < std::size_t S1, std::size_t S2 >
auto Concat(const std::bitset< S1 >& set1, const std::bitset< S2 >& set2) noexcept {
    auto set1String = set1.to_string();
    auto set2String = set2.to_string();

    return std::bitset< S1 + S2 >(std::move(std::move(set1String) + std::move(set2String)));
}

END_NAMESPACE

#endif // !defined(DYNBITSET_H_INCLUDED_A0863C53_28F4_403D_A2F2_CA32D9D2D51B)
