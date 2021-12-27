#if !defined(EXCEPTIONS_H_INCLUDED_E7753758_10AB_409E_BFD3_999E31104969)
    #define EXCEPTIONS_H_INCLUDED_E7753758_10AB_409E_BFD3_999E31104969

    #include <API/Api.h>
    #include <exception>

BEGIN_NAMESPACE

class undefined_register_access : public std::exception {
    [[nodiscard]] virtual const char* what() const noexcept override {
        return "Register is in undefined state";
    }
};
class undefined_behaviour : public std::exception {
    [[nodiscard]] virtual const char* what() const noexcept override {
        return "Undefined behaviour";
    }
};
class not_implemented_feature : public std::exception {
    [[nodiscard]] virtual const char* what() const noexcept override {
        return "Not implemented yet!";
    }
};
class unsupported_instruction : public std::exception {
    [[nodiscard]] virtual const char* what() const noexcept override {
        return "unsupported instruction due to unsupported feature";
    }
};
class undefined_instruction : public std::exception {
    [[nodiscard]] virtual const char* what() const noexcept override {
        return "Undefined instruction";
    }
};

END_NAMESPACE

#endif // !defined(EXCEPTIONS_H_INCLUDED_E7753758_10AB_409E_BFD3_999E31104969)
