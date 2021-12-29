#if !defined(LEXICALCAST_H_INCLUDED_67183790_1D3B_4926_A4D3_B503B1C24898)
    #define LEXICALCAST_H_INCLUDED_67183790_1D3B_4926_A4D3_B503B1C24898

    #include <API/Api.h>
    #include <Boost/lexical_cast.hpp>
    #include <string>
    #include <utility>

BEGIN_NAMESPACE

struct LexicalCast {
    template < class Target, class Source >
    [[nodiscard]] static Target Cast(Source&& type) {
        return boost::lexical_cast< Target >(std::move(type));
    }

    template < class Target, class Source >
    [[nodiscard]] static Target Cast(const Source& type) {
        return boost::lexical_cast< Target >(type);
    }
};

END_NAMESPACE

#endif // !defined(LEXICALCAST_H_INCLUDED_67183790_1D3B_4926_A4D3_B503B1C24898)
