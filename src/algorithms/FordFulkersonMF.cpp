#include <algorithm>

#include "Parameters.h"
#include "FordFulkersonMF.hpp"
#include "AdjacencyList.hpp"
#include "IncidenceMatrix.hpp"

#include "Logger.hpp"

FordFulkersonMF::FordFulkersonMF(GraphRepr& graph) : m_graph(graph) {
    // Create a residual graph based on the original graph
    size_t vertexCount = m_graph.getVertexCount();
    size_t edgeCount = m_graph.getEdgeCount();

    if (dynamic_cast<AdjacencyList*>(&graph) != nullptr) {
        m_residualGraph = new AdjacencyList(vertexCount, edgeCount * 2, true); // Each edge can be represented in both directions
    } else if (dynamic_cast<IncidenceMatrix*>(&graph) != nullptr) {
        m_residualGraph = new IncidenceMatrix(vertexCount, edgeCount * 2, true);
    } else {
        Logger::logln(Logger::ERROR, "Unsupported graph representation!");
        return;
    }

    // Get all the edges from the original graph
    DynamicArray<Pair<intmax_t, Pair<size_t, size_t>>> edges = m_graph.getAllEdges();
    for (size_t i = 0; i < edges.size(); i++) {
        intmax_t weight = edges.get(i).first();
        size_t startVertex = edges.get(i).second().first();
        size_t endVertex = edges.get(i).second().second();

        // Add the edge to the residual graph with the same capacity
        m_residualGraph->addEdge(startVertex, endVertex, weight);
        // Add the reverse edge with 0 capacity for the residual graph
        m_residualGraph->addEdge(endVertex, startVertex, 0);
    }
}

FordFulkersonMF::~FordFulkersonMF() {
    delete m_residualGraph;
}

int FordFulkersonMF::run() {
    if (!m_residualGraph) {
        Logger::logln(Logger::ERROR, "Residual graph not initialized");
        return 1;
    }

    m_residualGraph->exportToGraphviz("residual_graph_initial.dot");

    m_result.maxFlow = 0;

    size_t vertexCount = m_residualGraph->getVertexCount();
    DynamicArray<size_t> visited(vertexCount, 0);

    int source = Parameters::vertexStart;
    if (source < 0 || (size_t)source >= vertexCount) {
        Logger::logln(Logger::ERROR, "Invalid source vertex: ", source);
        return 1;
    }

    int sink = Parameters::vertexEnd;
    if (sink < 0 || (size_t)sink >= vertexCount) {
        Logger::logln(Logger::ERROR, "Invalid sink vertex: ", sink);
        return 1;
    }

    // Run the Ford-Fulkerson algorithm using DFS
    intmax_t flow = 0;
    do {
        flow = dfsSolve(source, INTMAX_MAX, visited);
        m_result.maxFlow += flow;
        m_visitedToken++;
    } while (flow > 0);

    m_resultReady = true;
    return 0;
}

intmax_t FordFulkersonMF::dfsSolve(size_t vertex, intmax_t flow, DynamicArray<size_t>& visited) {
    // Check if the vertex is the sink, if yes, return the current flow
    if (vertex == (size_t)Parameters::vertexEnd) {
        return flow;
    }

    // Mark the vertex as visited
    visited.set(vertex, m_visitedToken);

    // Get the neighbors of the vertex
    DynamicArray<size_t> neighbors = m_residualGraph->getAdjacentVertices(vertex);
    for (size_t i = 0; i < neighbors.size(); i++) {
        size_t neighbor = neighbors.get(i);

        // Check if the neighbor is not visited and there is available capacity
        intmax_t capacity = m_residualGraph->getEdgeWeight(vertex, neighbor);
        if (capacity > 0 && visited.get(neighbor) != m_visitedToken) {
            // Get the minimum flow and calculate the bottleneck flow
            intmax_t minFlow = std::min(flow, capacity);
            intmax_t bottleneck = dfsSolve(neighbor, minFlow, visited);

            // If there's a bottleneck, update the flow in the graph
            if (bottleneck > 0) {
                intmax_t residualCapacity = m_residualGraph->getEdgeWeight(neighbor, vertex);
                m_residualGraph->setEdgeWeight(vertex, neighbor, capacity - bottleneck);
                m_residualGraph->setEdgeWeight(neighbor, vertex, residualCapacity + bottleneck);
                return bottleneck;
            }
        }
    }

    return 0;
}

/**
 * Returns the result of the algorithm
 * 
 * @returns the result of the algorithm
 */
GraphAlgorithmResult& FordFulkersonMF::result() {
    return m_result;
}