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
            auto duration = std::chrono::duration_cast<Ratio>(m_endTime - m_startTime);
            return duration.count();
        }

        // Functions for getting the system date and time as strings
        static std::string getCurrentDate();
        static std::string getCurrentTime();
};

#endif // TIMER_HPP