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
#include "ReportBuilder.hpp"
#include "Timer.hpp"

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

    // Check if the output file is provided
    if (Parameters::outputFile.empty()) {
        Logger::logln(Logger::ERROR, "No output file provided!");
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
    if (representations == nullptr || representations->size() == 0) {
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
        Logger::logln(Logger::ERROR, "Failed to load graph data into representation(s)");
        return 1;
    }
    Logger::logln(Logger::OK, "Loaded graph data into representation(s)");

    // Do something with the representation...
    // Open the output file
    std::ofstream output(Parameters::outputFile);
    if (!output.is_open()) {
        Logger::logln(Logger::ERROR, "Failed to open output file!");
        return 1;
    }

    for (size_t i = 0; i < representations->size(); i++) {
        GraphRepr& currentRepr = *representations->get(i);
        DynamicArray<GraphAlgorithmBase*>* algorithms = createAlgorithms(currentRepr);
        if (algorithms == nullptr) {
            Logger::logln(Logger::ERROR, "Failed to create algorithms for ", currentRepr.name());
            return 1;
        }

        for (size_t j = 0; j < algorithms->size(); j++) {
            GraphAlgorithmBase& currentAlg = *algorithms->get(j);
            Timer timer;
            timer.start();
            int runResult = currentAlg.run();
            timer.stop();
            if (runResult != 0) {
                Logger::logln(Logger::ERROR, currentAlg.name(), " failed to run on ", currentRepr.name());
                return 1;
            }
            GraphAlgorithmResult& algResult = currentAlg.result();
            size_t duration = timer.getDuration();
            Logger::logln(Logger::OK, currentAlg.name(), " ran successfully on ", currentRepr.name(), " in: ", duration, "us");
            Logger::logln(Logger::INFO, "The total cost of the result is: ", algResult.cost());

            std::string report = ReportBuilder::buildReport(i * algorithms->size() + j + 1, &currentRepr, &currentAlg, &algResult, duration);
            output << report << "\n\n";
        }
        deleteAlgorithms(algorithms);
    }

    exportGraphImages(representations);

    deleteRepresentations(representations);
    return 0;
}