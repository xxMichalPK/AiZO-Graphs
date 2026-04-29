#include <iostream>
#include "DataParser.hpp"
#include "Timer.hpp"
#include "Logger.hpp"
#include "IncidenceMatrix.hpp"
#include "AdjacencyList.hpp"

int main(int argc, char* argv[]) {
    Logger::initialize(argc, argv);
    Logger* logger = Logger::getInstance();

    logger->log(Logger::logType_t::INFO, "Start: ", Timer::getCurrentDate(), " ", Timer::getCurrentTime(), "\n");

    // Get the size and check if it is valid
    DataParser::graphSize_t graphSize = DataParser::getGraphSize("graph_data.csv");
    if (graphSize.vertices == 0 && graphSize.edges == 0) return 1;

    // Create and load the graph
    AdjacencyList graph(graphSize.vertices, graphSize.edges);
    if (!DataParser::loadGraph("graph_data.csv", graph)) return 1;
    
    #if GRAPHVIZ_SUPPORT
        logger->log(Logger::logType_t::INFO, "Graphviz support is enabled.\n");
        graph.exportToGraphviz("graph_output.dot");
    #else
        logger->log(Logger::logType_t::WARNING, "Graphviz support is disabled.\n");
    #endif
    return 0;
}