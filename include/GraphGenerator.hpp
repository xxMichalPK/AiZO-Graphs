#ifndef GRAPHGENERATOR_HPP
#define GRAPHGENERATOR_HPP

#include "GraphRepr.hpp"

class GraphGenerator {
    public:
        GraphGenerator() = delete;
        static bool generate(GraphRepr &graph, size_t vertexCount, size_t edgeCount, bool directed = false);
    
    private:
        static bool generateDirected(GraphRepr &graph, size_t vertexCount, size_t edgeCount);
        static bool generateUndirected(GraphRepr &graph, size_t vertexCount, size_t edgeCount);
};

#endif // GRAPHGENERATOR_HPP