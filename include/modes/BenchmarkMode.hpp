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
};

#endif // BENCHMARKMODE_HPP