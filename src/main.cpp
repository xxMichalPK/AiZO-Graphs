#include <iostream>
#include "DataParser.hpp"
#include "Timer.hpp"
#include "Logger.hpp"
#include "IncidenceMatrix.hpp"
#include "AdjacencyList.hpp"

int main(int argc, char* argv[]) {
    std::cout << "Starting at: " << Timer::getCurrentDate() << " " << Timer::getCurrentTime() << "\n";

    Logger::initialize(argc, argv);
    Logger* logger = Logger::getInstance();

    logger->logBenchmark(100); // Example benchmark log
    logger->logBenchmark(300); // Example benchmark log
    logger->logBenchmark(600, 100, 1200); // Example benchmark log

    // Get the size and check if it is valid
    DataParser::graphSize_t graphSize = DataParser::getGraphSize("graph_data.csv");
    if (graphSize.vertices == 0 && graphSize.edges == 0) return 1;

    // Create and load the graph
    AdjacencyList graph(graphSize.vertices, graphSize.edges);
    if (!DataParser::loadGraph("graph_data.csv", graph)) return 1;
    
    #if GRAPHVIZ_SUPPORT
        std::cout << "Graphviz support is enabled.\n";
        graph.exportToGraphviz("graph_output.dot");
    #else
        std::cout << "Graphviz support is disabled.\n";
    #endif
    return 0;
}