#ifndef INCIDENCE_MATRIX_HPP
#define INCIDENCE_MATRIX_HPP

#include "GraphRepr.hpp"

class IncidenceMatrix : public GraphRepr {
    private:
        size_t m_numVertices = 0;
        size_t m_numEdges = 0;
        size_t m_currentEdgeIndex = 0;
        intmax_t** m_matrix = nullptr;
        bool m_directed;

    public:
        IncidenceMatrix(size_t vertexCount, size_t edgeCount, bool directed = false);
        ~IncidenceMatrix();

        virtual void addEdge(size_t startVertex, size_t endVertex, intmax_t weight) override;
        virtual bool checkEdge(size_t startVertex, size_t endVertex) override;

        virtual intmax_t getEdgeWeight(size_t startVertex, size_t endVertex) override;
        virtual void setEdgeWeight(size_t startVertex, size_t endVertex, intmax_t weight) override;

        virtual size_t getEdgeCount() override;
        virtual size_t getVertexCount() override;

        virtual DynamicArray<size_t> getAdjacentVertices(size_t vertex) override;
        virtual DynamicArray<Pair<intmax_t, Pair<size_t, size_t>>> getAllEdges() override;

        virtual std::string toString() override;

        #if GRAPHVIZ_SUPPORT
            virtual void exportToGraphviz(const char* filename) const override;
        #endif
};

#endif // INCIDENCE_MATRIX_HPP