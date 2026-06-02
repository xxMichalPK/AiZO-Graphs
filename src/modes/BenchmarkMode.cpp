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
    uint64_t generatorSeed = static_cast<uint64_t>(std::time(nullptr));
    if (Parameters::seed != -1) {
        generatorSeed = static_cast<uint64_t>(Parameters::seed);
    } else {
        // Save the generated seed for logging purposes
        Parameters::seed = (int64_t)generatorSeed;
    }
    GraphGenerator::initialize((unsigned int)generatorSeed);

    // Run the main benchmark loop
    size_t currentRun = 1;
    size_t totalRuns = Parameters::iterations * getRequestedRepresentationCount() * getRequestedAlgorithmCount();
    for (int i = 0; i < Parameters::iterations; i++) {
        // Run the benchmark for single iteration and continue on success
        if (benchmarkIteration(graphs, results, currentRun, totalRuns)) continue;
        
        // If it failed, log the error and exit
        Logger::logln(Logger::ERROR, "Benchmark iteration ", i + 1, " failed!");
        deleteRepresentations(graphs);
        return 1;
    }

    // Log the summary
    Logger::logln(Logger::OK, "Benchmark completed! Logging summary...");
    logSummary(results);

    exportGraphImages(graphs);

    // Clean up
    deleteRepresentations(graphs);
    return 0;
}


/**
 * Benchmarks a single iteration of representation-algorithm combinations and updates the benchmark results
 * 
 * @param graphs the graph representations to be benchmarked on
 * @param results the benchmark results to be updated with the new measurements
 * @param currentRun the current run number
 * @param totalRuns the total number of runs
 * 
 * @returns true if the benchmark was successful, false otherwise
 */
bool BenchmarkMode::benchmarkIteration(DynamicArray<GraphRepr*>* graphs, DynamicArray<DynamicArray<BenchmarkResult>>& results, size_t& currentRun, size_t totalRuns) {
    // Regenerate the graphs for the current iteration
    if (!regenerateGraphs(graphs)) {
        Logger::logln(Logger::ERROR, "Failed to regenerate graphs for iteration ", currentRun / Parameters::iterations + 1);
        return false;
    }

    // Run the benchmark for each representation and update results
    for (size_t ri = 0; ri < graphs->size(); ri++) {
        GraphRepr &representation = *graphs->get(ri);
        
        // Benchmark current representation and update results
        DynamicArray<BenchmarkResult> reprResults = results.get(ri);
        if (!benchmarkRepresentation(representation, reprResults, currentRun, totalRuns)) return false;
        results.set(ri, reprResults);
    }
    return true;
}


/**
 * Benchmarks all algorithms on a single representation and updates the benchmark results
 * 
 * @param representation the graph representation to be benchmarked on
 * @param results the benchmark results to be updated with the new measurements
 * @param currentRun the current run number
 * @param totalRuns the total number of runs
 * 
 * @returns true if the benchmark was successful, false otherwise
 */
bool BenchmarkMode::benchmarkRepresentation(GraphRepr& representation, DynamicArray<BenchmarkResult>& results, size_t& currentRun, size_t totalRuns) {
    DynamicArray<GraphAlgorithmBase*>* algorithms = createAlgorithms(representation);
    if (algorithms == nullptr || algorithms->size() == 0) {
        Logger::logln(Logger::ERROR, "Failed to create algorithms for representation ", representation.name());
        return false;
    }

    // Run the benchmark for each algorithm on the current representation
    for (size_t ai = 0; ai < algorithms->size(); ai++) {
        GraphAlgorithmBase &algorithm = *algorithms->get(ai);
        // Log the progress of the benchmark
        Logger::logProgress(currentRun, totalRuns, "Running ", algorithm.name(), " on ", representation.name(), " (", currentRun, "/", totalRuns, ")");
        
        // Run the benchmark and update results
        BenchmarkResult result = results.get(ai);
        if (!benchmarkAlgorithm(algorithm, representation, result)) {
            deleteAlgorithms(algorithms);   
            return false;
        }
        results.set(ai, result);
        currentRun++;
    }

    deleteAlgorithms(algorithms);
    return true;
}


/**
 * Benchmarks a single iteration of one algorithm on one representation and updates the benchmark results
 * 
 * @param algorithm the algorithm to be benchmarked
 * @param representation the graph representation to be benchmarked on
 * @param result the benchmark result to be updated with the new measurement
 * 
 * @returns true if the benchmark was successful, false otherwise
 */
bool BenchmarkMode::benchmarkAlgorithm(GraphAlgorithmBase& algorithm, GraphRepr& representation, BenchmarkResult& result) {
    Timer timer;
    timer.start();
    int success = algorithm.run();
    timer.stop();
    if (success != 0) {
        Logger::logln(Logger::ERROR, "Benchmark failed for ", algorithm.name(), " on ", representation.name());
        return false;
    }

    Logger::getInstance()->logBenchmark(representation.id(), algorithm.id(), timer.getDuration());

    // Store the benchmark result
    result.append(timer.getDuration());
    result.setAlgorithmId(algorithm.id());
    result.setRepresentationId(representation.id());
    return true;
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


bool BenchmarkMode::regenerateGraphs(DynamicArray<GraphRepr*>* graphs) {
    size_t edgeCount = graphs->get(0)->getEdgeCount();
    bool directed = isDirected();

    // Clear existing data from the graphs
    for (size_t gi = 0; gi < graphs->size(); gi++) {
        graphs->get(gi)->clear();
    }

    // Generate new graphs with the same parameters
    if (!GraphGenerator::generate(*graphs, Parameters::vertexCount, edgeCount, directed)) {
        Logger::logln(Logger::ERROR, "Failed to generate graph");
        return false;
    }
    return true;
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


void BenchmarkMode::logSummary(const DynamicArray<DynamicArray<BenchmarkResult>>& results) {
    for (size_t r1 = 0; r1 < results.size(); r1++) {
        const DynamicArray<BenchmarkResult> &reprResults = results.get(r1);

        for (size_t r2 = 0; r2 < reprResults.size(); r2++) {
            const BenchmarkResult &result = reprResults.get(r2);
            Logger::getInstance()->logBenchmark(result.getRepresentationId(), result.getAlgorithmId(), result.avg(), result.min(), result.max());
            Logger::logln(Logger::OK, "Summary for ", result.getAlgorithmId(), " on ", result.getRepresentationId(), ": avg = ", result.avg(),
                          "us, min = ", result.min(), "us, max = ", result.max(), "us");
        }
    }
}