#include "BenchmarkMode.hpp"
#include "Parameters.h"

#include "Logger.hpp"

#include "AdjacencyList.hpp"
#include "GraphGenerator.hpp"

int BenchmarkMode::run() {
    bool success = true;

    size_t edgeCount = calculateEdgeCount(Parameters::vertexCount, Parameters::density);
    Logger::getInstance()->log(Logger::logType_t::INFO, "The graph with ", Parameters::vertexCount, " vertices and density ",
                                Parameters::density, " should have ", edgeCount, " edges.\n");
    AdjacencyList graph(Parameters::vertexCount, edgeCount);
    success = GraphGenerator::generate(graph, Parameters::vertexCount, edgeCount, isDirected());
    if (!success) {
        Logger::getInstance()->log(Logger::logType_t::ERROR, "Failed to generate a graph with provided parameters!\n");
        return 1;
    }
    
    #if GRAPHVIZ_SUPPORT
        Logger::getInstance()->log(Logger::logType_t::INFO, "Graphviz support is enabled.\n");
        graph.exportToGraphviz("graph_output.dot");
    #else
        Logger::getInstance()->log(Logger::logType_t::WARNING, "Graphviz support is disabled.\n");
    #endif

    return 0;
}

size_t BenchmarkMode::calculateEdgeCount(size_t vertexCount, int density) {
    // Density should be between 0 and 100
    if (density < 0 || density > 100) return 0;
    double d_density = (double)density / 100.;
    
    // Check if the graph should be directed and calculate the number of edges accordingly
    if (isDirected()) {
        return (size_t)(d_density * vertexCount * (vertexCount - 1));
    }

    return (size_t)((d_density * vertexCount * (vertexCount - 1)) / 2.);
}

bool BenchmarkMode::isDirected() {
    if (Parameters::problem == Parameters::Problems::mst) return false;
    return true;
}