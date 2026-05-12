#include "BenchmarkMode.hpp"
#include "Parameters.h"

#include "Logger.hpp"

#include "AdjacencyList.hpp"
#include "GraphGenerator.hpp"

/**
 * Runs the benchmark
 * 
 * @returns 0 on success, other value on failure
 */
int BenchmarkMode::run() {
    bool success = true;
    bool directed = isDirected();
    const char* directedStr = directed ? "A directed" : "An undirected";

    if (Parameters::vertexCount <= 0 || Parameters::density <= 0) {
        Logger::logln(Logger::ERROR, "Incorrect vertex number or density specified!");
        return 1;
    }

    size_t edgeCount = calculateEdgeCount(Parameters::vertexCount, Parameters::density);
    Logger::logln(Logger::INFO, directedStr, " graph with ", Parameters::vertexCount, " vertices and density ",
                                Parameters::density, " should have ", edgeCount, " edges.");
    
    // Create all requested representations
    DynamicArray<GraphRepr*>* representations = createRepresentations(Parameters::vertexCount, edgeCount);
    if (representations == nullptr) {
        Logger::logln(Logger::ERROR, "Failed to create graph representations");
        return 1;
    }

    success = GraphGenerator::generate(*representations, Parameters::vertexCount, edgeCount, directed);
    if (!success) {
        Logger::logln(Logger::ERROR, "Failed to generate a graph with provided parameters!");
        return 1;
    }
    
#if GRAPHVIZ_SUPPORT
    if (Parameters::vertexCount <= 10) {
        for (size_t i = 0; i < representations->size(); i++) {
            std::string graphName = "graph_repr" + std::to_string(i) + ".dot";
            representations->get(i)->exportToGraphviz(graphName.c_str());
        }
    }
#endif

    for (size_t i = 0; i < representations->size(); i++) {
        delete representations->get(i);
    }
    delete representations;
    return 0;
}

/**
 * Calculates the number of edges in a graph based on the provided graph density
 * 
 * @param vertexCount number of all vertices in the graph
 * @param density the density of the graph (between 0 and 100 inclusive)
 * 
 * @returns number of edges in a graph with provided parameters
 */
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