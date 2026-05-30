#ifndef GRAPHGENERATOR_HPP
#define GRAPHGENERATOR_HPP

#include "DynamicArray.hpp"
#include "GraphRepr.hpp"
#include <ctime>

class GraphGenerator {
    public:
        GraphGenerator() = delete;
        static void initialize(unsigned int seed = std::time(nullptr));
        static bool generate(DynamicArray<GraphRepr*> &representations, size_t vertexCount, size_t edgeCount, bool directed = false);
    
    private:
        static bool generateDirected(DynamicArray<GraphRepr*> &representations, size_t vertexCount, size_t edgeCount);
        static bool generateUndirected(DynamicArray<GraphRepr*> &representations, size_t vertexCount, size_t edgeCount);
};

#endif // GRAPHGENERATOR_HPP