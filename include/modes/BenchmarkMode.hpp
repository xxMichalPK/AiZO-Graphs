#ifndef BENCHMARKMODE_HPP
#define BENCHMARKMODE_HPP

#include <cstddef>
#include "RunModeBase.hpp"

class BenchmarkMode : public RunModeBase {
    public:
        BenchmarkMode() = delete;

        static int run();
    
    private:
        static bool validateParameters();
        static size_t calculateEdgeCount(size_t vertexCount, int density);
        static DynamicArray<GraphRepr*>* createGraphs(size_t edgeCount, bool directed);
};

#endif // BENCHMARKMODE_HPP