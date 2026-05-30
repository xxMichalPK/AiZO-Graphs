#ifndef BENCHMARKMODE_HPP
#define BENCHMARKMODE_HPP

#include <cstddef>
#include "RunModeBase.hpp"
#include "BenchmarkResult.hpp"

class BenchmarkMode : public RunModeBase {
    public:
        BenchmarkMode() = delete;

        static int run();
    
    private:
        static bool validateParameters();
        static size_t calculateEdgeCount(size_t vertexCount, int density);
        static DynamicArray<GraphRepr*>* createGraphs(size_t edgeCount, bool directed);
        static bool benchmarkAlgorithm(GraphAlgorithmBase& algorithm, GraphRepr& representation, BenchmarkResult& result);
        static bool benchmarkRepresentation(GraphRepr& representation, DynamicArray<BenchmarkResult>& results, size_t& currentRun, size_t totalRuns);
        static bool benchmarkIteration(DynamicArray<GraphRepr*>* graphs, DynamicArray<DynamicArray<BenchmarkResult>>& results, size_t& currentRun, size_t totalRuns);
        static bool regenerateGraphs(DynamicArray<GraphRepr*>* graphs);
        static void logSummary(const DynamicArray<DynamicArray<BenchmarkResult>>& results);
};

#endif // BENCHMARKMODE_HPP