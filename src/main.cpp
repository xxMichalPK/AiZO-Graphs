#include "Parameters.h"

// Utilities
#include "Logger.hpp"
#include "Timer.hpp"

// Run modes
#include "SingleFileMode.hpp"
#include "BenchmarkMode.hpp"

/**
 * The main entry point of the program
 * 
 * @param argc number of arguments passed
 * @param argv pointer to the list of arguments
 * 
 * @returns 0 on success, any other number on failure
 */
int main(int argc, char* argv[]) {
    Logger::initialize(argc, argv);

    // Read parameters and check if we can go further
    int paramStatus = Parameters::readParameters(argc - 1, &argv[1]);
    if (paramStatus != 0) return paramStatus;
    
    // Run the program based on the requested run mode
    Logger::logln(Logger::INFO, "Start: ", Timer::getCurrentDate(), " ", Timer::getCurrentTime());
    switch (Parameters::runMode) {
        case Parameters::RunModes::help:
            Parameters::help();
            return 0;
        case Parameters::RunModes::singleFile:
            Logger::logln(Logger::INFO, "Running in single file mode...");
            return SingleFileMode::run();
        case Parameters::RunModes::benchmark:
            Logger::logln(Logger::INFO, "Running in benchmark mode...");
            return BenchmarkMode::run();
        default:
            break;
    }

    // If we're here the user did not provide the run mode
    Logger::logln(Logger::ERROR, "No run mode was selected. See \'", argv[0], " --help\' for help");
    return 1;
}