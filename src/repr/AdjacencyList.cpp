#include "AdjacencyList.hpp"
#include "Logger.hpp"
#include <iostream>

/**
 * Constructs an object of the adjacency list representation of the graph
 * 
 * @param vertexCount number of all possible vertices
 * @param edgeCount number of all possible edges
 * @param directed if the graph should be a directed or undirected one
 */
AdjacencyList::AdjacencyList(size_t vertexCount, size_t edgeCount, bool directed) : 
    GraphRepr("Adjacency List"), m_numVertices(vertexCount), m_numEdges(edgeCount), m_directed(directed) {
    
    //
    m_list = new Edge*[m_numVertices]();
}

/**
 * Clears the memory of the adjacency list
 */
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

/**
 * Adds an edge to the graph representation
 * 
 * @param startVertex start of the edge
 * @param endVertex end of the edge
 * @param weight the weight of the edge
 */
void AdjacencyList::addEdge(size_t startVertex, size_t endVertex, intmax_t weight) {
    if (startVertex >= m_numVertices || endVertex >= m_numVertices) {
        Logger::logln(Logger::ERROR, "Vertex out of bounds.");
        return;
    }

    // Create a new edge to insert at the begining of the list for given start vertex
    // So the next edge is the current head and we can do it fast
    Edge* newEdge = new Edge{endVertex, weight, m_list[startVertex]};
    m_list[startVertex] = newEdge;

    // If the graph is directed that's all
    if (m_directed) return;

    // In an undirected graph we need another edge in the opposite direction
    newEdge = new Edge{startVertex, weight, m_list[endVertex]};
    m_list[endVertex] = newEdge;
}

/**
 * Checks if an edge is defined in the graph (if the graph contains the specified edge)
 * 
 * @param startVertex start of the edge
 * @param endVertex end of the edge
 * 
 * @returns true when the edge belongs to the graph, false otherwise
 */
bool AdjacencyList::checkEdge(size_t startVertex, size_t endVertex) {
    if (startVertex >= m_numVertices || endVertex >= m_numVertices) {
        return false;
    }

    Edge* currentEdge = m_list[startVertex];
    while (currentEdge != nullptr) {
        if (currentEdge->endVertex == endVertex) return true;
        currentEdge = currentEdge->next;
    }

    return false;
}

intmax_t AdjacencyList::getEdgeWeight(size_t startVertex, size_t endVertex) {
    if (startVertex >= m_numVertices || endVertex >= m_numVertices) {
        Logger::logln(Logger::ERROR, "Vertex out of bounds.");
        return 0;
    }

    Edge* currentEdge = m_list[startVertex];
    while (currentEdge != nullptr) {
        if (currentEdge->endVertex == endVertex) return currentEdge->weight;
        currentEdge = currentEdge->next;
    }

    return 0;
}

size_t AdjacencyList::getEdgeCount() {
    return m_numEdges;
}

size_t AdjacencyList::getVertexCount() {
    return m_numVertices;
}

DynamicArray<size_t> AdjacencyList::getAdjacentVertices(size_t vertex) {
    DynamicArray<size_t> adjacentVertices;
    Edge* currentEdge = m_list[vertex];
    while (currentEdge != nullptr) {
        adjacentVertices.push(currentEdge->endVertex);
        currentEdge = currentEdge->next;
    }
    return adjacentVertices;
}

DynamicArray<Pair<intmax_t, Pair<size_t, size_t>>> AdjacencyList::getAllEdges() {
    DynamicArray<Pair<intmax_t, Pair<size_t, size_t>>> edges;
    for (size_t vertex = 0; vertex < m_numVertices; vertex++) {
        Edge* currentEdge = m_list[vertex];
        while (currentEdge != nullptr) {
            if (m_directed || vertex <= currentEdge->endVertex) { // Only add each edge once for undirected graph
                edges.push({currentEdge->weight, {vertex, currentEdge->endVertex}});
            }
            currentEdge = currentEdge->next;
        }
    }
    return edges;
}

void AdjacencyList::setEdgeWeight(size_t startVertex, size_t endVertex, intmax_t weight) {
    if (startVertex >= m_numVertices || endVertex >= m_numVertices) {
        Logger::logln(Logger::ERROR, "Vertex out of bounds.");
        return;
    }

    Edge* currentEdge = m_list[startVertex];
    while (currentEdge != nullptr) {
        if (currentEdge->endVertex == endVertex) {
            currentEdge->weight = weight;
            return;
        }
        currentEdge = currentEdge->next;
    }

    Logger::logln(Logger::ERROR, "Edge not found.");
}

#if GRAPHVIZ_SUPPORT
#include <fstream>

/**
 * Exports the graph data into a nice graphical representation using the graphviz program
 * 
 * @param filename path where the exported image should be saved (preferably ending in *.dot) 
 */
void AdjacencyList::exportToGraphviz(const char* filename) const {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        Logger::logln(Logger::ERROR, "Could not open file. Graph was not exported.\n");
        return;
    }

    outFile << (m_directed ? "digraph" : "graph") << " G {\n";
    for (size_t vertex = 0; vertex < m_numVertices; vertex++) {
        Edge* current = m_list[vertex];
        while (current != nullptr) {
            // Write the edge based on the direction of the graph
            if (m_directed) {
                // Directed graph
                outFile << "    " << vertex << " -> " << current->endVertex << " [label=\"" << current->weight << "\"];\n";
            } else {
                // Undirected graph
                if (vertex <= current->endVertex) { // Only write each edge once
                    outFile << "    " << vertex << " -- " << current->endVertex << " [label=\"" << current->weight << "\"];\n";
                }
            }
            current = current->next;
        }
    }
    outFile << "}\n";

    outFile.close();
    system(("dot -Tsvg " + std::string(filename) + " -o " + std::string(filename) + ".svg").c_str());
};

#endif