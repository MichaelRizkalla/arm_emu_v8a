#if !defined(KEYEVENT_H_INCLUDED_FD19414A_02B2_44E7_8ADE_ABB47928C40D)
    #define KEYEVENT_H_INCLUDED_FD19414A_02B2_44E7_8ADE_ABB47928C40D

#include <Event/IEvent.h>

namespace arm_emu {

    struct KeyEvent : public IEvent {
        std::int16_t GetKeyCode() const noexcept {
            return mKeyCode;
        }

      protected:
        KeyEvent(std::int16_t code) : m_keyCode { code } {
        }
        ~KeyEvent() = default;

        std::int16_t m_keyCode;
    };

}

#endif // !defined(KEYEVENT_H_INCLUDED_FD19414A_02B2_44E7_8ADE_ABB47928C40D)
