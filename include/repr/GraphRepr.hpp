#ifndef GRAPH_REPR_HPP
#define GRAPH_REPR_HPP

#include <cstddef>
#include <cstdint>
#include "DynamicArray.hpp"
#include "Pair.hpp"

class GraphRepr {
    protected:
        GraphRepr() = default;
    
    public:
        virtual ~GraphRepr() = default;

        virtual void addEdge(size_t startVertex, size_t endVertex, intmax_t weight) = 0;
        virtual bool checkEdge(size_t startVertex, size_t endVertex) = 0;
        virtual intmax_t getEdgeWeight(size_t startVertex, size_t endVertex) = 0;
        virtual void setEdgeWeight(size_t startVertex, size_t endVertex, intmax_t weight) = 0;
        virtual size_t getEdgeCount() = 0;
        virtual size_t getVertexCount() = 0;
        virtual DynamicArray<size_t> getAdjacentVertices(size_t vertex) = 0;
        virtual DynamicArray<Pair<intmax_t, Pair<size_t, size_t>>> getAllEdges() = 0;

        #if GRAPHVIZ_SUPPORT
            virtual void exportToGraphviz(const char* filename) const = 0;
        #endif
};

#endif // GRAPH_REPR_HPP