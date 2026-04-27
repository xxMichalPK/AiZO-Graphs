#ifndef ADJACENCY_LIST_HPP
#define ADJACENCY_LIST_HPP

#include "GraphRepr.hpp"

class AdjacencyList : public GraphRepr {
    private:
        size_t m_numVertices = 0;
        size_t m_numEdges = 0;
        struct Edge {
            size_t endVertex;
            intmax_t weight;
            Edge* next;
        };
        struct Edge** m_list = nullptr;

    public:
        AdjacencyList(size_t vertexCount, size_t edgeCount);
        ~AdjacencyList();

        virtual void addEdge(size_t startVertex, size_t endVertex, intmax_t weight) override;

        #if GRAPHVIZ_SUPPORT
            virtual void exportToGraphviz(const char* filename) const override;
        #endif
};

#endif // ADJACENCY_LIST_HPP