#include "AdjacencyList.hpp"
#include "Logger.hpp"
#include <iostream>

constexpr int directed = 1; // 0 - undirected, 1 - directed (later changed by the parameters library)

AdjacencyList::AdjacencyList(size_t vertexCount, size_t edgeCount) : m_numVertices(vertexCount), m_numEdges(edgeCount) {
    m_list = new Edge*[m_numVertices]();
}

AdjacencyList::~AdjacencyList() {
    for (size_t i = 0; i < m_numVertices; i++) {
        Edge* current = m_list[i];
        while (current != nullptr) {
            Edge* del = current;
            current = current->next;
            delete del;
        }
    }
    delete[] m_list;
}

void AdjacencyList::addEdge(size_t startVertex, size_t endVertex, intmax_t weight) {
    if (startVertex >= m_numVertices || endVertex >= m_numVertices) {
        Logger::getInstance()->log(Logger::logType_t::ERROR, "Vertex out of bounds.\n");
        return;
    }

    // Create a new edge to insert at the begining of the list for given start vertex
    // So the next edge is the current head and we can do it fast
    Edge* newEdge = new Edge{endVertex, weight, m_list[startVertex]};
    m_list[startVertex] = newEdge;
}

#if GRAPHVIZ_SUPPORT
#include <fstream>

void AdjacencyList::exportToGraphviz(const char* filename) const {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        Logger::getInstance()->log(Logger::logType_t::ERROR, "Could not open file. Graph was not exported.\n");
        return;
    }

    outFile << (directed ? "digraph" : "graph") << " G {\n";
    for (size_t vertex = 0; vertex < m_numVertices; vertex++) {
        Edge* current = m_list[vertex];
        while (current != nullptr) {
            // Write the edge based on the direction of the graph
            if (directed) {
                // Directed graph
                outFile << "    " << vertex << " -> " << current->endVertex << " [label=\"" << current->weight << "\"];\n";
            } else {
                // Undirected graph
                outFile << "    " << vertex << " -- " << current->endVertex << " [label=\"" << current->weight << "\"];\n";
            }
            current = current->next;
        }
    }
    outFile << "}\n";

    outFile.close();
    system(("dot -Tsvg " + std::string(filename) + " -o " + std::string(filename) + ".svg").c_str());
};

#endif