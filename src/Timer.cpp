#include "Timer.hpp"

/**
 * Record the start time of the timer
 */
void Timer::start() {
    m_startTime = std::chrono::high_resolution_clock::now();
    m_endTime = {}; // Reset end time
}

/**
 * Record the end time of the timer
 */
void Timer::stop() {
    m_endTime = std::chrono::high_resolution_clock::now();
}

/**
 * Get the current date as a string
 * 
 * @return Current date in the format "YYYY-MM-DD"
 */
std::string Timer::getCurrentDate() {
    std::time_t currentTime = std::time(nullptr);
    std::tm* localTime = std::localtime(&currentTime);

    char dateString[sizeof("YYYY-MM-DD")];
    std::strftime(dateString, sizeof(dateString), "%Y-%m-%d", localTime);
    return std::string(dateString);
}

/**
 * Get the current time as a string
 * 
 * @return Current time in the format "HH:MM:SS"
 */
std::string Timer::getCurrentTime() {
    std::time_t currentTime = std::time(nullptr);
    std::tm* localTime = std::localtime(&currentTime);

    char timeString[sizeof("HH:MM:SS")];
    std::strftime(timeString, sizeof(timeString), "%H:%M:%S", localTime);
    return std::string(timeString);
}