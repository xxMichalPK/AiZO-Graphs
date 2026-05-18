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
        bool m_directed;

    public:
        AdjacencyList(size_t vertexCount, size_t edgeCount, bool directed = false);
        ~AdjacencyList();

        virtual void addEdge(size_t startVertex, size_t endVertex, intmax_t weight) override;
        virtual bool checkEdge(size_t startVertex, size_t endVertex) override;
        virtual intmax_t getEdgeWeight(size_t startVertex, size_t endVertex) override;
        virtual size_t getEdgeCount() override;

        #if GRAPHVIZ_SUPPORT
            virtual void exportToGraphviz(const char* filename) const override;
        #endif
};

#endif // ADJACENCY_LIST_HPP