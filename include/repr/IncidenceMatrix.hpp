#ifndef INCIDENCE_MATRIX_HPP
#define INCIDENCE_MATRIX_HPP

#include "GraphRepr.hpp"

class IncidenceMatrix : public GraphRepr {
    private:
        size_t m_numVertices = 0;
        size_t m_numEdges = 0;
        size_t m_currentEdgeIndex = 0;
        intmax_t** m_matrix = nullptr;

    public:
        IncidenceMatrix(size_t vertexCount, size_t edgeCount);
        ~IncidenceMatrix();

        virtual void addEdge(size_t startVertex, size_t endVertex, intmax_t weight) override;

        #if GRAPHVIZ_SUPPORT
            virtual void exportToGraphviz(const char* filename) const override;
        #endif
};

#endif // INCIDENCE_MATRIX_HPP