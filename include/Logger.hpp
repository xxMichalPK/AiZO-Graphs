#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <filesystem>
#include <iostream>

// Colors from https://github.com/kuroidoruido/ColorLog/blob/master/colorlog.h
#define LOG_COLOR_RESET "\033[m"
#define LOG_COLOR_RED "\033[1;31m"
#define LOG_COLOR_GREEN "\033[0;32m"
#define LOG_COLOR_BLUE "\033[1;34m"
#define LOG_COLOR_YELLOW "\033[0;33m"
#define LOG_COLOR_WHITE "\033[0;37m"

class Logger {
    private:
        static Logger* m_instance;
        
        int m_argc;
        char** m_argv;
        std::filesystem::path m_logFile;
    
    public:
        typedef enum logType {
            NONE,
            INDENT,
            INFO,
            WARNING,
            ERROR
        } logType_t;

    private:
        Logger() : m_argc(0), m_argv(nullptr), m_logFile("log.csv") {}
        Logger(int argc, char** argv, const std::filesystem::path& logFile = "log.csv") : 
            m_argc(argc), m_argv(argv), m_logFile(logFile) {}

    public:
        static void initialize(int argc, char** argv, const std::filesystem::path& logFile = "log.csv");
        static Logger* getInstance();

        template<typename Type, typename... Args>
        void log(logType_t type, Type arg1, Args... arg2);
        void logBenchmark(size_t executionTime);
        void logBenchmark(size_t averageTime, size_t minTime, size_t maxTime);
    
    private:
        void log(logType_t) {}; // Base case for the templated variadic function
        void logBenchmarkCommon(std::ofstream& logFile);
        void writeBenchmarkHeader(std::ofstream& logFile);
        bool validateOpen(std::ofstream& logFile);

};

// Implement the templated variadic function as shown at https://www.geeksforgeeks.org/cpp/variadic-function-templates-c/
template<typename Type, typename... Args>
void Logger::log(Logger::logType_t type, Type arg1, Args... arg2) {
    // Print the initial type
    if (type == Logger::logType_t::INDENT) {
        std::cout << "         ";
    } else if (type == Logger::logType_t::INFO) {
        std::cout << "[ INFO ] ";
    } else if (type == Logger::logType_t::WARNING) {
        std::cout << "[ " << LOG_COLOR_YELLOW << "WARN" << LOG_COLOR_RESET << " ] ";
    } else if (type == Logger::logType_t::ERROR) {
        std::cout << "[ " << LOG_COLOR_RED << "ERRO" << LOG_COLOR_RESET << " ] ";
    }

    // Print the first argument
    std::cout << arg1;

    // Print the other arguments recursively
    log(Logger::logType_t::NONE, arg2...);
}

#endif // LOGGER_HPP