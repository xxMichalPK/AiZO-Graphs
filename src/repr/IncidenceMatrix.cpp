#include "IncidenceMatrix.hpp"
#include "Logger.hpp"

constexpr int directed = 1; // 0 - undirected, 1 - directed (later changed by the parameters library)

IncidenceMatrix::IncidenceMatrix(size_t vertexCount, size_t edgeCount) : 
    m_numVertices(vertexCount), m_numEdges(edgeCount) {

    // The incidence matrix has numVertices rows and numEdges columns
    // so we need to allocate memory for it so m_matrix[row][col] points to the corrent weight
    m_matrix = new intmax_t*[m_numVertices];
    for (size_t i = 0; i < m_numVertices; i++) {
        m_matrix[i] = new intmax_t[m_numEdges]();
    }
}

IncidenceMatrix::~IncidenceMatrix() {
    for (size_t i = 0; i < m_numVertices; i++) {
        delete[] m_matrix[i];
    }
    delete[] m_matrix;
}

void IncidenceMatrix::addEdge(size_t startVertex, size_t endVertex, intmax_t weight) {
    // The weights are in range rand(1,k*4/5) so we can use 0 as the indicator of no edge
    // Now the case of undirected or directed graph... If it's undirected, just use the same weight
    // else the negative weight will be used as the end of the edge
    switch (directed) {
        case 0: // undirected
            m_matrix[startVertex][m_currentEdgeIndex] = weight;
            m_matrix[endVertex][m_currentEdgeIndex] = weight;
            break;
        case 1: // directed
            m_matrix[startVertex][m_currentEdgeIndex] = weight;
            m_matrix[endVertex][m_currentEdgeIndex] = -weight; // negative weight to indicate the end of the edge
            break;
    }
    m_currentEdgeIndex++;
}

#if GRAPHVIZ_SUPPORT
#include <fstream>

void IncidenceMatrix::exportToGraphviz(const char* filename) const {
    // Specs: https://graphviz.org/doc/info/lang.html
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        Logger::getInstance()->log(Logger::logType_t::ERROR, "Could not open file. Graph was not exported.\n");
        return;
    }

    outFile << (directed ? "digraph" : "graph") << " G {\n";
    for (size_t edge = 0; edge < m_numEdges; edge++) {
        bool startFound = false;
        size_t startVertex = 0;
        size_t endVertex = 0;
        intmax_t startWeight = 0;
        intmax_t endWeight = 0;
        for (size_t vertex = 0; vertex < m_numVertices; vertex++) {
            if (m_matrix[vertex][edge] == 0) continue; // no edge

            // We've found the start vertex
            if (m_matrix[vertex][edge] > 0 && startVertex == 0 && !startFound) {
                startVertex = vertex;
                startWeight = m_matrix[vertex][edge];
                startFound = true;
            } else {
                // Here we either have a negative weight (end) or we've already found the start
                // which means this must be the end and it's an undirected graph
                endVertex = vertex;
                endWeight = m_matrix[vertex][edge];

                // Remember to check if we have the start index
                if (startFound)
                    break; // we can break here because we only have one edge per column
            }
        }

        if (startWeight == 0 || endWeight == 0) continue; // invalid edge, skip
        
        // Write the endge based on the direction of the graph
        if (directed) {
            // Directed graph
            outFile << "    " << startVertex << " -> " << endVertex << " [label=\"" << startWeight << "\"];\n";
        } else {
            // Undirected graph
            outFile << "    " << startVertex << " -- " << endVertex << " [label=\"" << startWeight << "\"];\n";
        }

    }
    outFile << "}\n";

    outFile.close();
    system(("dot -Tsvg " + std::string(filename) + " -o " + std::string(filename) + ".svg").c_str());
}
#endif