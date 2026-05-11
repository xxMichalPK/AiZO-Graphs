#ifndef GRAPHGENERATOR_HPP
#define GRAPHGENERATOR_HPP

#include "GraphRepr.hpp"

class GraphGenerator {
    public:
        GraphGenerator() = delete;
        static bool generate(GraphRepr &graph, size_t vertexCount, int density);
    
    private:
        static bool generateDirected(GraphRepr &graph, size_t vertexCount, int density);
        static bool generateUndirected(GraphRepr &graph, size_t vertexCount, int density);
};

#endif // GRAPHGENERATOR_HPP