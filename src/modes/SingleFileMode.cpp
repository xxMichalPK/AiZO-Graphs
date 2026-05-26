#include <filesystem>
#include <fstream>

#include "SingleFileMode.hpp"

#include "Parameters.h"
#include "DataParser.hpp"

#include "GraphRepr.hpp"
#include "AdjacencyList.hpp"
#include "IncidenceMatrix.hpp"

#include "GraphAlgorithmBase.hpp"
#include "PrimMST.hpp"
#include "KruskalMST.hpp"
#include "DijkstraSP.hpp"
#include "BellmanFordSP.hpp"
#include "FordFulkersonMF.hpp"

#include "GraphAlgorithmResult.hpp"
#include "MSTResult.hpp"
#include "SPResult.hpp"

#include "Logger.hpp"

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
    Logger::logln(Logger::OK, "Created representation(s) for a graph with ",
                  graphSize.vertices, " vertices and ", graphSize.edges, " edges");

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
    Logger::logln(Logger::OK, "Loaded graph data into representation(s)");

    // Do something with the representation...
    for (size_t i = 0; i < representations->size(); i++) {
        GraphRepr& currentRepr = *representations->get(i);
        DynamicArray<GraphAlgorithmBase*>* algorithms = createAlgorithms(currentRepr);
        if (algorithms == nullptr) {
            Logger::logln(Logger::ERROR, "Failed to create algorithms for representation ", i);
            return 1;
        }

        for (size_t j = 0; j < algorithms->size(); j++) {
            GraphAlgorithmBase& currentAlg = *algorithms->get(j);
            if (currentAlg.run() != 0) {
                Logger::logln(Logger::ERROR, "Algorithm ", j, " failed to run on representation ", i);
                return 1;
            }
            GraphAlgorithmResult& algResult = currentAlg.result();
            Logger::logln(Logger::OK, currentAlg.name(), " ran successfully on ", currentRepr.name(), " result:");
            Logger::logln(Logger::INFO, algResult);
        }
        deleteAlgorithms(algorithms);
    }

#if GRAPHVIZ_SUPPORT
    if (graphSize.vertices <= 10) {
        for (size_t i = 0; i < representations->size(); i++) {
            std::string graphName = "graph_repr" + std::to_string(i) + ".dot";
            representations->get(i)->exportToGraphviz(graphName.c_str());
        }
    }
#endif

    deleteRepresentations(representations);
    return 0;
}