#include "BenchmarkMode.hpp"
#include "Parameters.h"

#include "Logger.hpp"

#include "AdjacencyList.hpp"
#include "GraphGenerator.hpp"
#include "Timer.hpp"

/**
 * Runs the benchmark
 * 
 * @returns 0 on success, other value on failure
 */
int BenchmarkMode::run() {
    // Validate parameters
    if (Parameters::vertexCount <= 0 || Parameters::density <= 0) {
        Logger::logln(Logger::ERROR, "Incorrect vertex number or density specified!");
        return 1;
    }

    if (Parameters::resultsFile.empty()) {
        Logger::logln(Logger::ERROR, "No results file provided!");
        return 1;
    }

    // Set the log file for benchmark results
    Logger::getInstance()->setLogFile(Parameters::resultsFile);

    // Create and generate the graph
    DynamicArray<GraphRepr*>* representations = createAndGenerateGraphs();
    if (representations == nullptr || representations->size() == 0) return 1;

    // Perform benchmarks for all representations and algorithms
    for (size_t ri = 0; ri < representations->size(); ri++) {
        GraphRepr& currentRepr = *representations->get(ri);

        // Create all requested algorithms
        DynamicArray<GraphAlgorithmBase*>* algorithms = createAlgorithms(currentRepr);
        if (algorithms == nullptr) {
            Logger::logln(Logger::ERROR, "Failed to create algorithms for ", currentRepr.name());
            return 1;
        }

        // Run benchmarks for all algorithms
        for (size_t ai = 0; ai < algorithms->size(); ai++) {
            GraphAlgorithmBase& algorithm = *algorithms->get(ai);

            // Banchmark the algorithm on the current representation and check the result
            bool success = benchmarkAlgorithm(algorithm, currentRepr);
            if (!success) {
                Logger::logln(Logger::ERROR, "Failed to benchmark ", algorithm.name(), " on ", currentRepr.name());
                return 1;
            }
        }

        // Safely delete all algorithms
        deleteAlgorithms(algorithms);
    }

    // If it's supported and the graph is small export it as an image
    exportGraphImages(representations);

    // Cleanup representations and return
    deleteRepresentations(representations);
    return 0;
}


/**
 * Benchmarks the specified algorithm on the specified graph representation
 * 
 * @param algorithm the algorithm to benchmark
 * @param representation the graph representation on which to benchmark the algorithm
 * 
 * @returns true if the benchmark was successful, false otherwise
 */
bool BenchmarkMode::benchmarkAlgorithm(GraphAlgorithmBase& algorithm, GraphRepr& representation) {
    Timer timer;

    size_t totalDuration = 0;
    size_t minDuration = SIZE_MAX;
    size_t maxDuration = 0;

    GraphAlgorithmResult* initialResult = nullptr;

    // Loop for the specified number of iterations
    for (int i = 0; i < Parameters::iterations; i++) {
        timer.start();
        int runResult = algorithm.run();
        timer.stop();
        if (runResult != 0) {
            Logger::logln(Logger::ERROR, algorithm.name(), " failed to run on ", representation.name());
            return false;
        }

        GraphAlgorithmResult& algResult = algorithm.result();
        if (initialResult == nullptr) {
            initialResult = &algResult;
        } else if (initialResult->cost() != algResult.cost()) {
            Logger::logln(Logger::ERROR, algorithm.name(), " produced different results on iteration ", i);
            return false;
        }

        size_t duration = timer.getDuration();
        Logger::getInstance()->logBenchmark(duration);

        totalDuration += duration;
        minDuration = duration < minDuration ? duration : minDuration;
        maxDuration = duration > maxDuration ? duration : maxDuration;
    }

    Logger::logln(Logger::OK, algorithm.name(), " ran successfully on ", representation.name(), ". ", *initialResult);
    Logger::logln(Logger::INFO, "Average execution time: ", totalDuration / Parameters::iterations, "us");
    Logger::logln(Logger::INFO, "Minimum execution time: ", minDuration, "us");
    Logger::logln(Logger::INFO, "Maximum execution time: ", maxDuration, "us");

    // Write the benchmark results to the log file
    Logger::getInstance()->logBenchmark(totalDuration / Parameters::iterations, minDuration, maxDuration);

    return true;
}


/**
 * Creates and generates data for all requested graph representations
 *
 * @returns a dynamic array of graph representations, or nullptr if something went wrong
 */
DynamicArray<GraphRepr*>* BenchmarkMode::createAndGenerateGraphs() {
    bool directed = isDirected();
    const char* directedStr = directed ? "A directed" : "An undirected";

    // Calculate the number of edges based on the required density and vertex count
    size_t edgeCount = calculateEdgeCount(Parameters::vertexCount, Parameters::density);
    Logger::logln(Logger::INFO, directedStr, " graph with ", Parameters::vertexCount, " vertices and density ",
                                Parameters::density, " should have ", edgeCount, " edges.");
    

    // Create all requested representations
    DynamicArray<GraphRepr*>* representations = createRepresentations(Parameters::vertexCount, edgeCount);
    if (representations == nullptr) {
        Logger::logln(Logger::ERROR, "Failed to create graph representations");
        return nullptr;
    }
    Logger::logln(Logger::OK, "Created representation(s) for a graph with ",
                  Parameters::vertexCount, " vertices and ", edgeCount, " edges");


    // Generate graph data
    bool success = GraphGenerator::generate(*representations, Parameters::vertexCount, edgeCount, directed);
    if (!success) {
        Logger::logln(Logger::ERROR, "Failed to generate a graph with provided parameters!");
        return nullptr;
    }
    Logger::logln(Logger::OK, "Generated random graph data into representation(s)");
    return representations;
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