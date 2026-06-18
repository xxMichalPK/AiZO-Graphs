#include "IncidenceMatrix.hpp"

#include "Logger.hpp"

/**
 * Constructs an object of the incidence matrix representation of the graph
 * 
 * @param vertexCount number of all possible vertices
 * @param edgeCount number of all possible edges
 * @param directed if the graph should be a directed or undirected one
 */
IncidenceMatrix::IncidenceMatrix(size_t vertexCount, size_t edgeCount, bool directed) : 
    GraphRepr("Incidence Matrix", "INCIDENCE_MATRIX"), m_numVertices(vertexCount), m_numEdges(edgeCount), m_directed(directed) {

    // The incidence matrix has numVertices rows and numEdges columns
    // so we need to allocate memory for it so m_matrix[row][col] points to the corrent weight
    m_matrix = new intmax_t*[m_numVertices];
    for (size_t i = 0; i < m_numVertices; i++) {
        m_matrix[i] = new intmax_t[m_numEdges]();
    }
    usageBitmap = new uint8_t[m_numEdges*m_numVertices/8 + 1]();
}


/**
 * Clears the memory of the incidence matrix
 */
IncidenceMatrix::~IncidenceMatrix() {
    for (size_t i = 0; i < m_numVertices; i++) {
        delete[] m_matrix[i];
    }
    delete[] m_matrix;
    delete[] usageBitmap;
}


/**
 * Clears the graph representation by setting all weights to 0
 */
void IncidenceMatrix::clear() {
    for (size_t i = 0; i < m_numVertices; i++) {
        for (size_t j = 0; j < m_numEdges; j++) {
            m_matrix[i][j] = 0;
        }
    }
    m_currentEdgeIndex = 0;

    // Recreate the usage bitmap
    delete[] usageBitmap;
    usageBitmap = new uint8_t[m_numEdges*m_numVertices/8 + 1]();
}


/**
 * Adds an edge to the graph representation
 * 
 * @param startVertex start of the edge
 * @param endVertex end of the edge
 * @param weight the weight of the edge
 */
void IncidenceMatrix::addEdge(size_t startVertex, size_t endVertex, intmax_t weight) {
    // The weights are in range rand(1,k*4/5) so we can use 0 as the indicator of no edge
    // Now the case of undirected or directed graph... If it's undirected, just use the same weight
    // else the negative weight will be used as the end of the edge
    m_matrix[startVertex][m_currentEdgeIndex] = weight;
    if (!m_directed) { // undirected
        m_matrix[endVertex][m_currentEdgeIndex] = weight;
    } else { // directed
        m_matrix[endVertex][m_currentEdgeIndex] = -weight; // negative weight to indicate the end of the edge
    }
    setEdgeUsed(m_currentEdgeIndex, startVertex);
    setEdgeUsed(m_currentEdgeIndex, endVertex);
    m_currentEdgeIndex++;
}


/**
 * Checks if an edge is defined in the graph (if the graph contains the specified edge)
 * 
 * @param startVertex start of the edge
 * @param endVertex end of the edge
 * 
 * @returns true when the edge belongs to the graph, false otherwise
 */
bool IncidenceMatrix::checkEdge(size_t startVertex, size_t endVertex) {
    if (startVertex >= m_numVertices || endVertex >= m_numVertices) {
        return false;
    }

    for (size_t i = 0; i < m_numEdges; i++) {
        intmax_t startWeight = m_matrix[startVertex][i];
        if (!edgeExists(i, startVertex) || !edgeExists(i, endVertex) || startWeight < 0) continue;
        return true;
    }
    return false;
}


/**
 * Returns the weight of the specified edge
 * 
 * @param startVertex start of the edge
 * @param endVertex end of the edge
 * 
 * @returns the weight of the edge, or 0 if the edge does not exist
 */
intmax_t IncidenceMatrix::getEdgeWeight(size_t startVertex, size_t endVertex) {
    if (startVertex >= m_numVertices || endVertex >= m_numVertices) {
        Logger::logln(Logger::ERROR, "Vertex out of bounds.");
        return 0;
    }

    for (size_t i = 0; i < m_numEdges; i++) {
        intmax_t startWeight = m_matrix[startVertex][i];
        if (!edgeExists(i, startVertex) || !edgeExists(i, endVertex) || startWeight < 0) continue;
        return startWeight;
    }
    return 0;
}


/**
 * Returns number of edges in this graph representation
 * 
 * @returns the number of edges
 */
size_t IncidenceMatrix::getEdgeCount() {
    return m_numEdges;
}


/**
 * Returns the number of vertices in this graph representation
 * 
 * @returns the number of vertices
 */
size_t IncidenceMatrix::getVertexCount() {
    return m_numVertices;
}


/**
 * Returns a list of all vertices that are adjacent to the specified vertex
 * (all vertices that are connected to the specified vertex by an edge)
 * 
 * @param vertex the vertex for which the adjacent vertices should be returned
 * 
 * @returns a list of all adjacent vertices
 */
DynamicArray<size_t> IncidenceMatrix::getAdjacentVertices(size_t vertex) {
    DynamicArray<size_t> adjacentVertices;
    for (size_t edgeIndex = 0; edgeIndex < m_numEdges; edgeIndex++) {
        if (m_matrix[vertex][edgeIndex] <= 0) continue; // Skip end of edge and non-edges

        // Get the adjacent vertex for this edge
        for (size_t adjacentVertex = 0; adjacentVertex < m_numVertices; adjacentVertex++) {
            if (adjacentVertex == vertex || m_matrix[adjacentVertex][edgeIndex] == 0) continue;
            
            adjacentVertices.push(adjacentVertex);
            break;
        }
    }
    return adjacentVertices;
}


/**
 * Returns a list of all edges in the graph in the format (weight, (startVertex, endVertex))
 * 
 * @returns a list of all edges
 */
DynamicArray<Pair<intmax_t, Pair<size_t, size_t>>> IncidenceMatrix::getAllEdges() {
    DynamicArray<Pair<intmax_t, Pair<size_t, size_t>>> edges;
    for (size_t edgeIndex = 0; edgeIndex < m_numEdges; edgeIndex++) {
        bool startFound = false;
        size_t startVertex = 0;
        size_t endVertex = 0;
        intmax_t weight = 0;

        for (size_t vertex = 0; vertex < m_numVertices; vertex++) {
            if (m_matrix[vertex][edgeIndex] > 0 && !startFound) {
                startVertex = vertex;
                weight = m_matrix[vertex][edgeIndex];
                startFound = true;
            } else if (edgeExists(edgeIndex, vertex)) {
                endVertex = vertex;
            }
        }

        if (startFound) {
            edges.push({weight, {startVertex, endVertex}});
        }
    }
    return edges;
}


/**
 * Sets the weight of the specified edge
 * 
 * @param startVertex start of the edge
 * @param endVertex end of the edge
 * @param weight the new weight of the edge
 */
void IncidenceMatrix::setEdgeWeight(size_t startVertex, size_t endVertex, intmax_t weight) {
    if (startVertex >= m_numVertices || endVertex >= m_numVertices) {
        Logger::logln(Logger::ERROR, "Vertex out of bounds.");
        return;
    }

    for (size_t i = 0; i < m_numEdges; i++) {
        intmax_t startWeight = m_matrix[startVertex][i];
        if (!edgeExists(i, startVertex) || !edgeExists(i, endVertex) || startWeight < 0) continue;
        
        m_matrix[startVertex][i] = weight;
        if (!m_directed) { // undirected
            m_matrix[endVertex][i] = weight;
        } else { // directed
            m_matrix[endVertex][i] = -weight; // negative weight to indicate the end of the edge
        }
        return;
    }
}


/**
 * Returns a string representation of the incidence matrix
 * 
 * @returns a string representing the matrix
 */
std::string IncidenceMatrix::toString() {
    const int width = 5;
    const int labelWidth = 4;

    std::ostringstream out;
    out << std::setw(labelWidth) << " ";

    for (size_t edge = 0; edge < m_numEdges; edge++) {
        out << std::setw(width) << ("e" + std::to_string(edge));
    }

    out << "\n";
    for (size_t vertex = 0; vertex < m_numVertices; vertex++) {
        out << std::left << std::setw(labelWidth) << ("v" + std::to_string(vertex)) << std::right;

        for (size_t edge = 0; edge < m_numEdges; edge++) {
            out << std::setw(width) << m_matrix[vertex][edge];
        }
        if (vertex < m_numVertices - 1) {
            out << "\n";
        }
    }

    return out.str();
}

bool IncidenceMatrix::edgeExists(size_t edgeIndex, size_t vertex) const {
    size_t bitIndex = edgeIndex * m_numVertices + vertex;
    size_t byteIndex = bitIndex / 8;
    uint8_t bitMask = 1 << (bitIndex % 8);
    return (usageBitmap[byteIndex] & bitMask) != 0;
}

void IncidenceMatrix::setEdgeUsed(size_t edgeIndex, size_t vertex) {
    size_t bitIndex = edgeIndex * m_numVertices + vertex;
    size_t byteIndex = bitIndex / 8;
    uint8_t bitMask = 1 << (bitIndex % 8);
    usageBitmap[byteIndex] |= bitMask;
}

void IncidenceMatrix::setEdgeUnused(size_t edgeIndex, size_t vertex) {
    size_t bitIndex = edgeIndex * m_numVertices + vertex;
    size_t byteIndex = bitIndex / 8;
    uint8_t bitMask = ~(1 << (bitIndex % 8));
    usageBitmap[byteIndex] &= bitMask;
}

#if GRAPHVIZ_SUPPORT
#include <fstream>

/**
 * Exports the graph data into a nice graphical representation using the graphviz program
 * 
 * @param filename path where the exported image should be saved (preferably ending in *.dot) 
 */
void IncidenceMatrix::exportToGraphviz(const char* filename) const {
    // Specs: https://graphviz.org/doc/info/lang.html
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        Logger::logln(Logger::ERROR, "Could not open file. Graph was not exported.");
        return;
    }

    outFile << (m_directed ? "digraph" : "graph") << " G {\n";
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
        if (m_directed) {
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