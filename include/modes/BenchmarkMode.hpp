#ifndef BENCHMARKMODE_HPP
#define BENCHMARKMODE_HPP

#include <cstddef>
#include "RunModeBase.hpp"

class BenchmarkMode : public RunModeBase {
    public:
        BenchmarkMode() = delete;

        static int run();
    
    private:
        static size_t calculateEdgeCount(size_t vertexCount, int density);
        static bool benchmarkAlgorithm(GraphAlgorithmBase& algorithm, GraphRepr& representation);
        static DynamicArray<GraphRepr*>* createAndGenerateGraphs();
};

#endif // BENCHMARKMODE_HPP