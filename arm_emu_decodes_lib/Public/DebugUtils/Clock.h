#if !defined(CLOCK_H_INCLUDED_367A7CB7_C580_4BF4_86CA_976A7A6A65A1)
    #define CLOCK_H_INCLUDED_367A7CB7_C580_4BF4_86CA_976A7A6A65A1

    #include <API/Api.h>
    #include <chrono>
    #include <ostream>

namespace arm_emu {

    class Clock {
      private:
        struct TimeStamp {
            std::chrono::time_point< std::chrono::milliseconds > m_time;
        };

        using clock_type = std::chrono::high_resolution_clock;

      public:
        Clock() = delete;

        static TimeStamp Stamp() {
            /* auto   time = std::chrono::milliseconds { std::chrono::time_point_cast< std::chrono::milliseconds >(
                clock_type::now()) };
            TimeStamp stamp { time };
            return stamp;*/
            return TimeStamp {};
        }
    };

} // namespace arm_emu

#endif // !defined(CLOCK_H_INCLUDED_367A7CB7_C580_4BF4_86CA_976A7A6A65A1)
