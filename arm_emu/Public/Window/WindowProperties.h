#if !defined(WINDOWPROPERTIES_H_INCLUDED_828339EA_1C4C_4B2B_8A4D_C324DA13014C)
    #define WINDOWPROPERTIES_H_INCLUDED_828339EA_1C4C_4B2B_8A4D_C324DA13014C

    #include <Event/IEvent.h>
    #include <functional>

namespace arm_emu {

    struct WindowProperties {
        int         m_width;
        int         m_height;
        const char* m_title;
        bool        m_vSync;

        EventHandler* m_handler;
    };

} // namespace arm_emu

#endif // !defined(WINDOWPROPERTIES_H_INCLUDED_828339EA_1C4C_4B2B_8A4D_C324DA13014C)
