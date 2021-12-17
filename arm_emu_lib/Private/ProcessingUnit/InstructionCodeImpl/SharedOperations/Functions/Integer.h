// AddWithCarry()
// Integer addition with carry input, returning result and NZCV flags
template < std::size_t N >
[[nodiscard]] auto AddWithCarry(std::bitset< N > x, std::bitset< N > y, bool carry_in) {
    using T  = std::uint64_t;
    using ST = std::make_signed_t< T >;

    T  unsigned_sum = static_cast< T >(x.to_ullong()) + static_cast< T >(y.to_ullong()) + static_cast< T >(carry_in);
    ST signed_sum   = static_cast< ST >(x.to_ullong()) + static_cast< ST >(y.to_ullong()) + static_cast< T >(carry_in);
    std::bitset< N > result = unsigned_sum;

    std::bitset< 4 > nzcv { 0000 };
    nzcv |= static_cast< std::uint8_t >(result[N - 1] & true) << 3;
    // static_cast< std::uint8_t >((result.to_ullong() >> (N - 1)) & 1) << 3;
    nzcv |= result.none() ? 1 << 2 : 0;
    auto result_as_ull = result.to_ullong();
    nzcv |= static_cast< T >(result_as_ull) == unsigned_sum ? 0 : 1 << 1;
    nzcv |= static_cast< ST >(result_as_ull) == signed_sum ? 0 : 1 << 0;
    return std::make_pair(result, nzcv);
}
