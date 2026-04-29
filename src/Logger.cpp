#include "Logger.hpp"
#include "Timer.hpp"
#include <fstream>
#include <iostream>

Logger* Logger::m_instance = nullptr;

/**
 * Initialize the logger with the given arguments and log file
 * 
 * @param argc The number of command line arguments
 * @param argv The command line arguments
 * @param logFile The path to the log file
 */
void Logger::initialize(int argc, char** argv, const std::filesystem::path& logFile) {
    if (m_instance == nullptr) {
        m_instance = new Logger(argc, argv, logFile);
    }
}

/**
 * Get the singleton instance of the logger
 * 
 * @return The logger instance
 */
Logger* Logger::getInstance() {
    if (m_instance == nullptr) {
        m_instance = new Logger();
    }
    return m_instance;
}

/**
 * Log a single benchmark execution time to the log file
 * 
 * @param executionTime The execution time in microseconds
 */
void Logger::logBenchmark(size_t executionTime) {
    std::ofstream logFile(m_logFile, std::ios::app);
    if (!validateOpen(logFile)) return;

    // Log the common stats
    logBenchmarkCommon(logFile);

    // Log the execution time
    logFile << ",SINGLE_RUN," << executionTime << "us,,,\n";
}

/**
 * Log the benchmark summary (average, min, max) to the log file
 * 
 * @param averageTime The average execution time in microseconds
 * @param minTime The minimum execution time in microseconds
 * @param maxTime The maximum execution time in microseconds
 */
void Logger::logBenchmark(size_t averageTime, size_t minTime, size_t maxTime) {
    std::ofstream logFile(m_logFile, std::ios::app);
    if (!validateOpen(logFile)) return;

    // Log the common stats
    logBenchmarkCommon(logFile);

    // Log the benchmark times
    logFile << ",SUMMARY,," << averageTime << "us," << minTime << "us," << maxTime << "us\n";
}

/**
 * Log the common benchmark information (date, time, arguments) to the log file
 * 
 * @param logFile The ofstream object to the log file
 */
void Logger::logBenchmarkCommon(std::ofstream& logFile) {
    // Write the header if the file is new
    writeBenchmarkHeader(logFile);

    // Log the current date and time
    std::string currentDate = Timer::getCurrentDate();
    std::string currentTime = Timer::getCurrentTime();
    logFile << currentDate << "," << currentTime << ",";

    // Log every argument (skip the program name)
    for (int i = 1; i < m_argc; i++) {
        logFile << m_argv[i];
        if (i < m_argc - 1) logFile << " ";
    }
}

/**
 * Write the benchmark header to the log file if it is new
 * 
 * @param logFile The ofstream object to the log file
 */
void Logger::writeBenchmarkHeader(std::ofstream& logFile) {
    // Check if the file is open or has content, if not write the header
    if (!logFile.is_open() || logFile.tellp() != 0) return;

    logFile << "DATE,TIME,ARGUMENTS,TYPE,EXECUTION TIME (us),AVERAGE (us),MIN (us),MAX (us)\n";
}

/**
 * Check if the log file is open, if not print an error message
 * 
 * @param logFile The ofstream object to the log file
 * @return true if the file is open, false otherwise
 */
bool Logger::validateOpen(std::ofstream& logFile) {
    if (logFile.is_open()) return true;

    std::cerr << "Failed to open log file: " << m_logFile << "\n";
    return false;
}