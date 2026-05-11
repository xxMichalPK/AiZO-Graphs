#ifndef BENCHMARKMODE_HPP
#define BENCHMARKMODE_HPP

#include <cstddef>

class BenchmarkMode {
    public:
        BenchmarkMode() = delete;

        static int run();
    
    private:
        static size_t calculateEdgeCount(size_t vertexCount, int density);
        static bool isDirected();
};

#endif // BENCHMARKMODE_HPP