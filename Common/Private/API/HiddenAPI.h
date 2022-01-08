#if !defined(HIDDENAPI_H_INCLUDED_114779A8_FCCB_4898_B431_F0F4FDA871DA)
    #define HIDDENAPI_H_INCLUDED_114779A8_FCCB_4898_B431_F0F4FDA871DA

    #define BEGIN_NAMESPACE namespace arm_emu {

    #define END_NAMESPACE }

    // Class declaration helpers
    #define CTOR(name, flag)        name() = flag;
    #define COPY_CTOR(name, flag)   name(const name&) = flag;
    #define COPY_ASSIGN(name, flag) name& operator=(const name&) = flag;
    #define MOVE_CTOR(name, flag)   name(name&&) noexcept = flag;
    #define MOVE_ASSIGN(name, flag) name& operator=(name&&) noexcept = flag;
    #define DTOR(name, flag)        ~name() = flag;
    #define VDTOR(name, flag)       virtual ~name() = flag;

    #define CONSTEXPR_CTOR(name, flag)        constexpr name() = flag;
    #define CONSTEXPR_COPY_CTOR(name, flag)   constexpr name(const name&) = flag;
    #define CONSTEXPR_COPY_ASSIGN(name, flag) constexpr name& operator=(const name&) = flag;
    #define CONSTEXPR_MOVE_CTOR(name, flag)   constexpr name(name&&) noexcept = flag;
    #define CONSTEXPR_MOVE_ASSIGN(name, flag) constexpr name& operator=(name&&) noexcept = flag;
    #define CONSTEXPR_DTOR(name, flag)        constexpr ~name() = flag;

    #define DEFAULT_CTOR(name)        CTOR(name, default)
    #define DEFAULT_COPY_CTOR(name)   COPY_CTOR(name, default)
    #define DEFAULT_COPY_ASSIGN(name) COPY_ASSIGN(name, default)
    #define DEFAULT_MOVE_CTOR(name)   MOVE_CTOR(name, default)
    #define DEFAULT_MOVE_ASSIGN(name) MOVE_ASSIGN(name, default)
    #define DEFAULT_DTOR(name)        DTOR(name, default)
    #define VIRTUAL_DTOR(name)        VDTOR(name, default)

    #define DEFAULT_CONSTEXPR_CTOR(name)        CONSTEXPR_CTOR(name, default)
    #define DEFAULT_CONSTEXPR_COPY_CTOR(name)   CONSTEXPR_COPY_CTOR(name, default)
    #define DEFAULT_CONSTEXPR_COPY_ASSIGN(name) CONSTEXPR_COPY_ASSIGN(name, default)
    #define DEFAULT_CONSTEXPR_MOVE_CTOR(name)   CONSTEXPR_MOVE_CTOR(name, default)
    #define DEFAULT_CONSTEXPR_MOVE_ASSIGN(name) CONSTEXPR_MOVE_ASSIGN(name, default)
    #define DEFAULT_CONSTEXPR_DTOR(name)        CONSTEXPR_DTOR(name, default)

    #define DELETE_CTOR(name)        CTOR(name, delete)
    #define DELETE_COPY_CTOR(name)   COPY_CTOR(name, delete)
    #define DELETE_COPY_ASSIGN(name) COPY_ASSIGN(name, delete)
    #define DELETE_MOVE_CTOR(name)   MOVE_CTOR(name, delete)
    #define DELETE_MOVE_ASSIGN(name) MOVE_ASSIGN(name, delete)
    #define DELETE_DTOR(name)        DTOR(name, delete)

    #define DEFAULT_COPY_CLASS(name) \
        DEFAULT_COPY_CTOR(name)      \
        DEFAULT_COPY_ASSIGN(name)

    #define DEFAULT_MOVE_CLASS(name) \
        DEFAULT_MOVE_CTOR(name)      \
        DEFAULT_MOVE_ASSIGN(name)

    #define DEFAULT_CONSTRUCTED_CLASS(name) \
        DEFAULT_CTOR(name)                  \
        DEFAULT_COPY_CLASS(name)            \
        DEFAULT_MOVE_CLASS(name)

    #define DEFAULT_CONSTEXPR_COPY_CLASS(name) \
        DEFAULT_CONSTEXPR_COPY_CTOR(name)      \
        DEFAULT_CONSTEXPR_COPY_ASSIGN(name)

    #define DEFAULT_CONSTEXPR_MOVE_CLASS(name) \
        DEFAULT_CONSTEXPR_MOVE_CTOR(name)      \
        DEFAULT_CONSTEXPR_MOVE_ASSIGN(name)

    #define DEFAULT_CONSTEXPR_CONSTRUCTED_CLASS(name) \
        DEFAULT_CONSTEXPR_CTOR(name)                  \
        DEFAULT_CONSTEXPR_COPY_CLASS(name)            \
        DEFAULT_CONSTEXPR_MOVE_CLASS(name)

    #define DELETE_COPY_CLASS(name) \
        DELETE_COPY_CTOR(name)      \
        DELETE_COPY_ASSIGN(name)

    #define DELETE_MOVE_CLASS(name) \
        DELETE_MOVE_CTOR(name)      \
        DELETE_MOVE_ASSIGN(name)

    #define DEFAULT_CLASS(name)         \
        DEFAULT_CONSTRUCTED_CLASS(name) \
        DEFAULT_DTOR(name)

    #define DEFAULT_CONSTEXPR_CLASS(name)         \
        DEFAULT_CONSTEXPR_CONSTRUCTED_CLASS(name) \
        DEFAULT_CONSTEXPR_DTOR(name)

    #define DEFAULT_INTERFACE(name)     \
        DEFAULT_CONSTRUCTED_CLASS(name) \
        VIRTUAL_DTOR(name)

    #define DEFAULT_CONSTEXPR_INTERFACE(name)     \
        DEFAULT_CONSTEXPR_CONSTRUCTED_CLASS(name) \
        VIRTUAL_DTOR(name)

    #define STATIC_CLASS(name)  \
        DELETE_CTOR(name)       \
        DELETE_COPY_CLASS(name) \
        DELETE_MOVE_CLASS(name) \
        DELETE_DTOR(name)

#endif // !defined(HIDDENAPI_H_INCLUDED_114779A8_FCCB_4898_B431_F0F4FDA871DA)
