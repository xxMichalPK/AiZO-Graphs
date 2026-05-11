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

    int paramStatus = Parameters::readParameters(argc - 1, &argv[1]);
    if (paramStatus != 0) return paramStatus;
    
    Logger::logln(Logger::INFO, "Start: ", Timer::getCurrentDate(), " ", Timer::getCurrentTime());
    switch (Parameters::runMode) {
        case Parameters::RunModes::help:
            Parameters::help();
            return 0;
        case Parameters::RunModes::singleFile:
            return SingleFileMode::run();
        case Parameters::RunModes::benchmark:
            return BenchmarkMode::run();
        default:
            Logger::logln(Logger::ERROR, "Selected run mode does not exist or is not implemented yet!");
            return 1;
    }

    return 0;
}