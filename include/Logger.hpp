#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <filesystem>

class Logger {
    private:
        static Logger* m_instance;
        
        int m_argc;
        char** m_argv;
        std::filesystem::path m_logFile;
    
    public:
        typedef enum logType {
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

        void log(logType_t type, const char* format, ...);
        void logBenchmark(size_t executionTime);
        void logBenchmark(size_t averageTime, size_t minTime, size_t maxTime);
    
    private:
        void logBenchmarkCommon(std::ofstream& logFile);
        void writeBenchmarkHeader(std::ofstream& logFile);
        bool validateOpen(std::ofstream& logFile);

};

#endif // LOGGER_HPP