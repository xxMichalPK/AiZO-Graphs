#include "BenchmarkMode.hpp"
#include "Parameters.h"

#include "Logger.hpp"

#include "AdjacencyList.hpp"
#include "GraphGenerator.hpp"

int BenchmarkMode::run() {
    bool success = true;
    bool directed = isDirected();

    size_t edgeCount = calculateEdgeCount(Parameters::vertexCount, Parameters::density);
    Logger::logln(Logger::INFO, "The graph with ", Parameters::vertexCount, " vertices and density ",
                                Parameters::density, " should have ", edgeCount, " edges.");
    
    AdjacencyList graph(Parameters::vertexCount, edgeCount, directed);
    success = GraphGenerator::generate(graph, Parameters::vertexCount, edgeCount, directed);
    if (!success) {
        Logger::logln(Logger::ERROR, "Failed to generate a graph with provided parameters!");
        return 1;
    }
    
    #if GRAPHVIZ_SUPPORT
        Logger::logln(Logger::INFO, "Graphviz support is enabled.");
        graph.exportToGraphviz("graph_output.dot");
    #else
        Logger::logln(Logger::WARNING, "Graphviz support is disabled.");
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