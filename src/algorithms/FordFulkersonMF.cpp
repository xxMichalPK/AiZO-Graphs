#include <algorithm>

#include "Parameters.h"
#include "FordFulkersonMF.hpp"
#include "AdjacencyList.hpp"
#include "IncidenceMatrix.hpp"
#include "Queue.hpp"

#include "Logger.hpp"


#define USE_EDMONDS_KARP_OPTIMIZATION 1


/**
 * Initializes the Ford-Fulkerson algorithm by creating a residual graph based on the original graph
 * 
 * @param graph the original graph to find the maximum flow for
 */
FordFulkersonMF::FordFulkersonMF(GraphRepr& graph) : GraphAlgorithmBase("Ford-Fulkerson Maximum Flow", "FORD_FULKERSON"), m_graph(graph) {
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

        // There's an edge case if the original graph contains edges in both directions
        // If this happens try to work around it by changing the residual weight
        if (m_residualGraph->checkEdge(startVertex, endVertex)) {
            intmax_t existingWeight = m_residualGraph->getEdgeWeight(startVertex, endVertex);
            if (existingWeight == 0) {
                m_residualGraph->setEdgeWeight(startVertex, endVertex, weight);
            } else {
                Logger::logln(Logger::WARNING, "Duplicate edge found in the original graph: ", startVertex, " -> ", endVertex,
                                               ". Expect problems!");
            }
            continue;
        }

        // Add the edge to the residual graph with the same capacity
        m_residualGraph->addEdge(startVertex, endVertex, weight);
        // Add the reverse edge with 0 capacity for the residual graph
        m_residualGraph->addEdge(endVertex, startVertex, 0);
    }
}


/**
 * Frees the memory allocated for the residual graph
 */
FordFulkersonMF::~FordFulkersonMF() {
    delete m_residualGraph;
}


/**
 * Run the Ford-Fulkerson algorithm for maximum flow
 * 
 * @returns 0 on success, 1 on failure
 */
int FordFulkersonMF::run() {
    if (!m_residualGraph) {
        Logger::logln(Logger::ERROR, "Residual graph not initialized");
        return 1;
    }

    m_result.maxFlow = 0;

    size_t vertexCount = m_residualGraph->getVertexCount();

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
#if !USE_EDMONDS_KARP_OPTIMIZATION
    DynamicArray<size_t> visited(vertexCount, 0);
    do {
        flow = dfsSolve(source, sink, INTMAX_MAX, visited);
        m_result.maxFlow += flow;
        m_visitedToken++;
    } while (flow > 0);
#else
    DynamicArray<size_t> parent(vertexCount, SIZE_MAX);
    while ((flow = bfsSolve(source, sink, parent)) > 0) {
        m_result.maxFlow += flow;

        size_t current = sink;
        while (current != (size_t)source) {
            size_t previous = parent.get(current);
            intmax_t forward = m_residualGraph->getEdgeWeight(previous, current);
            intmax_t reverse = m_residualGraph->getEdgeWeight(current, previous);

            m_residualGraph->setEdgeWeight(previous, current, forward - flow);
            m_residualGraph->setEdgeWeight(current, previous, reverse + flow);
            current = previous;
        }
    }
#endif

    m_resultReady = true;
    return 0;
}


/**
 * Performs a depth-first search to find an augmenting path in the residual graph and returns the bottleneck flow of that path
 * Based on: https://www.youtube.com/watch?v=Xu8jjJnwvxE
 * 
 * @param source the source vertex
 * @param sink the sink vertex
 * @param flow the current flow being sent through the path
 * @param visited an array to keep track of visited vertices during the DFS
 * 
 * @returns the bottleneck flow of the augmenting path found, or 0 if no augmenting path is found
 */
intmax_t FordFulkersonMF::dfsSolve(size_t source, size_t sink, intmax_t flow, DynamicArray<size_t>& visited) {
    // Check if the vertex is the sink, if yes, return the current flow
    if (source == sink) {
        return flow;
    }

    // Mark the vertex as visited
    visited.set(source, m_visitedToken);

    // Get the neighbors of the vertex
    DynamicArray<size_t> neighbors = m_residualGraph->getAdjacentVertices(source);
    for (size_t i = 0; i < neighbors.size(); i++) {
        const size_t& neighbor = neighbors.get(i);
        const size_t& neighborToken = visited.get(neighbor);

        // Check if the neighbor is not visited and there is available capacity
        intmax_t capacity = m_residualGraph->getEdgeWeight(source, neighbor);
        if (capacity > 0 && neighborToken != m_visitedToken) {
            // Get the minimum flow and calculate the bottleneck flow
            intmax_t minFlow = flow < capacity ? flow : capacity;
            intmax_t bottleneck = dfsSolve(neighbor, sink, minFlow, visited);

            // If there's a bottleneck, update the flow in the graph
            if (bottleneck > 0) {
                intmax_t residualCapacity = m_residualGraph->getEdgeWeight(neighbor, source);
                m_residualGraph->setEdgeWeight(source, neighbor, capacity - bottleneck);
                m_residualGraph->setEdgeWeight(neighbor, source, residualCapacity + bottleneck);
                return bottleneck;
            }
        }
    }

    return 0;
}


/**
 * Performs a breadth-first search to find an augmenting path in the residual graph and returns the bottleneck flow of that path
 * 
 * @param source the source vertex
 * @param sink the sink vertex
 * @param flow the current flow being sent through the path
 * @param visited an array to keep track of visited vertices during the BFS
 * 
 * @returns the bottleneck flow of the augmenting path found, or 0 if no augmenting path is found
 */
intmax_t FordFulkersonMF::bfsSolve(size_t source, size_t sink, DynamicArray<size_t>& parent) {
    const size_t vertexCount = m_residualGraph->getVertexCount();

    // SIZE_MAX means "unvisited"
    for (size_t i = 0; i < vertexCount; i++) {
        parent.set(i, SIZE_MAX);
    }

    Queue<Pair<size_t, intmax_t>> queue;

    parent.set(source, source);
    queue.push({source, INTMAX_MAX});

    while (!queue.isEmpty()) {
        Pair<size_t, intmax_t> current = queue.getFront();
        queue.pop();

        size_t vertex = current.first();
        intmax_t flow = current.second();

        DynamicArray<size_t> neighbors = m_residualGraph->getAdjacentVertices(vertex);
        for (size_t i = 0; i < neighbors.size(); i++) {
            size_t neighbor = neighbors.get(i);

            intmax_t capacity = m_residualGraph->getEdgeWeight(vertex, neighbor);

            if (capacity <= 0) continue;
            if (parent.get(neighbor) != SIZE_MAX) continue;

            parent.set(neighbor, vertex);

            intmax_t newFlow = flow < capacity ? flow : capacity;

            if (neighbor == sink) return newFlow;

            queue.push({neighbor, newFlow});
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