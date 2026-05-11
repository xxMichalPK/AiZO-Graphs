#include <iostream>
#include "Parameters.h"
#include "DataParser.hpp"
#include "Timer.hpp"
#include "Logger.hpp"
#include "IncidenceMatrix.hpp"
#include "AdjacencyList.hpp"
#include "Queue.hpp"
#include "PriorityQueue.hpp"
#include "DynamicArray.hpp"
#include "GraphGenerator.hpp"
#include "SingleFileMode.hpp"
#include "BenchmarkMode.hpp"

int main(int argc, char* argv[]) {
    Logger::initialize(argc, argv);
    Logger* logger = Logger::getInstance();

    int paramStatus = Parameters::readParameters(argc - 1, &argv[1]);
    if (paramStatus != 0) return paramStatus;
    
    logger->log(Logger::logType_t::INFO, "Start: ", Timer::getCurrentDate(), " ", Timer::getCurrentTime(), "\n");
    switch (Parameters::runMode) {
        case Parameters::RunModes::help:
            Parameters::help();
            return 0;
        case Parameters::RunModes::singleFile:
            return SingleFileMode::run();
        case Parameters::RunModes::benchmark:
            return BenchmarkMode::run();
        default:
            logger->log(Logger::logType_t::ERROR, "Selected run mode does not exist or is not implemented yet!\n");
            return 1;
    }

    return 0;
}