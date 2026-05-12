#include <filesystem>
#include <fstream>

#include "SingleFileMode.hpp"

#include "Parameters.h"
#include "Logger.hpp"
#include "DataParser.hpp"

#include "GraphRepr.hpp"
#include "AdjacencyList.hpp"
#include "IncidenceMatrix.hpp"

/**
 * Runs program in the single file mode
 * 
 * @returns 0 on success, other value on failure
 */
int SingleFileMode::run() {
    // Check if the input file exists
    if (!std::filesystem::exists(Parameters::inputFile)) {
        Logger::logln(Logger::ERROR, "Input file does not exist!");
        return 1;
    }

    // Get the graph size
    DataParser::graphSize_t graphSize = DataParser::getGraphSize(Parameters::inputFile);
    if (graphSize.edges == 0 || graphSize.vertices == 0) {
        Logger::logln(Logger::ERROR, "Failed to retrieve graph size");
        return 1;
    }

    // Create all requested representations
    DynamicArray<GraphRepr*>* representations = createRepresentations(graphSize.vertices, graphSize.edges);
    if (representations == nullptr) {
        Logger::logln(Logger::ERROR, "Failed to create graph representations");
        return 1;
    }

    // Load data into all representations
    bool success = true;
    for (size_t i = 0; i < representations->size(); i++) {
        success = DataParser::loadGraph(Parameters::inputFile, *representations->get(i));
        if (!success) break;
    }

    // Check if the graph was loaded correctly
    if (!success) {
        Logger::logln(Logger::ERROR, "Failed to load graph data into representation");
        return 1;
    }

    // Do something with the representation...
#if GRAPHVIZ_SUPPORT
    if (graphSize.vertices <= 10) {
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