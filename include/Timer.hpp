#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
#include <string>

class Timer {
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime = {};
        std::chrono::time_point<std::chrono::high_resolution_clock> m_endTime = {};

    public:
        Timer() = default;

        void start();
        void stop();

        template<typename Ratio = std::micro>
        size_t getDuration() const {
            using DurationType = std::chrono::duration<size_t, Ratio>;
            DurationType duration = std::chrono::duration_cast<DurationType>(m_endTime - m_startTime);
            return static_cast<size_t>(duration.count());
        }

        // Functions for getting the system date and time as strings
        static std::string getCurrentDate();
        static std::string getCurrentTime();
};

#endif // TIMER_HPP