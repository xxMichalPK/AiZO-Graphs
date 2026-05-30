#include "BenchmarkMode.hpp"
#include "BenchmarkResult.hpp"
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
    if (!validateParameters()) return 1;
    
    // Set the log file for benchmark results
    Logger::getInstance()->setLogFile(Parameters::resultsFile);
    
    // Calculate the number of edges based on the required density and vertex count
    size_t edgeCount = calculateEdgeCount(Parameters::vertexCount, Parameters::density);
    
    // Create representations
    bool directed = isDirected();
    DynamicArray<GraphRepr*>* graphs = createGraphs(edgeCount, directed);
    if (graphs == nullptr || graphs->size() == 0) return 1;

    // Prepare a 2D array to store benchmark results for each representation and algorithm
    DynamicArray<DynamicArray<BenchmarkResult>> results(getRequestedRepresentationCount(), 
        DynamicArray<BenchmarkResult>(getRequestedAlgorithmCount(), {})
    );

    // Initialize the graph generator
    GraphGenerator::initialize();

    // Run the main benchmark loop
    for (int i = 0; i < Parameters::iterations; i++) {
        // Generate new graphs for each iteration
        for (size_t gi = 0; gi < graphs->size(); gi++) {
            graphs->get(gi)->clear();
        }
        if (!GraphGenerator::generate(*graphs, Parameters::vertexCount, edgeCount, directed)) {
            Logger::logln(Logger::ERROR, "Failed to generate graph for iteration ", i + 1);
            return 1;
        }

        for (size_t ri = 0; ri < graphs->size(); ri++) {
            DynamicArray<BenchmarkResult> reprResults = results.get(ri);
            GraphRepr &representation = *graphs->get(ri);
            DynamicArray<GraphAlgorithmBase*>* algorithms = createAlgorithms(representation);
            if (algorithms == nullptr || algorithms->size() == 0) {
                Logger::logln(Logger::ERROR, "Failed to create algorithms for representation ", representation.name());
                return 1;
            }


            for (size_t ai = 0; ai < algorithms->size(); ai++) {
                GraphAlgorithmBase &alg = *algorithms->get(ai);

                Timer timer;
                timer.start();
                int success = alg.run();
                timer.stop();
                if (success != 0) {
                    Logger::logln(Logger::ERROR, "Benchmark failed for ", alg.name(), " on ", representation.name());
                    return 1;
                }

                Logger::getInstance()->logBenchmark(representation.id(), alg.id(), timer.getDuration());

                // Store the benchmark result
                BenchmarkResult result = reprResults.get(ai);
                result.append(timer.getDuration());
                result.setAlgorithmId(alg.id());
                result.setRepresentationId(representation.id());
                reprResults.set(ai, result);

                Logger::logln(Logger::INFO, "Iteration ", i + 1, "/", Parameters::iterations, ": Algorithm ", alg.name(),
                              " on representation ", representation.name(), " took ", timer.getDuration(), "us");
            }

            results.set(ri, reprResults);
            deleteAlgorithms(algorithms);
        }
    }

    for (size_t r1 = 0; r1 < results.size(); r1++) {
        const DynamicArray<BenchmarkResult> &reprResults = results.get(r1);

        for (size_t r2 = 0; r2 < reprResults.size(); r2++) {
            const BenchmarkResult &result = reprResults.get(r2);
            Logger::getInstance()->logBenchmark(result.getRepresentationId(), result.getAlgorithmId(), result.avg(), result.min(), result.max());
        }
    }

    // Clean up
    deleteRepresentations(graphs);
    return 0;
}


/**
 * Helper function that validates the provided benchmark parameters
 * 
 * @returns true if all parameters are valid, false otherwise
 */
bool BenchmarkMode::validateParameters() {
    if (Parameters::vertexCount <= 0 || Parameters::density <= 0) {
        Logger::logln(Logger::ERROR, "Incorrect vertex number or density specified!");
        return false;
    }

    if (Parameters::resultsFile.empty()) {
        Logger::logln(Logger::ERROR, "No results file provided!");
        return false;
    }
    return true;
}


/**
 * Creates all requested graph representations
 *
 * @returns a dynamic array of graph representations, or nullptr if something went wrong
 */
DynamicArray<GraphRepr*>* BenchmarkMode::createGraphs(size_t edgeCount, bool directed) {
    // Log information about the graph to be created
    const char* directedStr = directed ? "A directed" : "An undirected";
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