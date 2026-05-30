#include "Parameters.h"
#include "DijkstraSP.hpp"
#include "DynamicArray.hpp"
#include "PriorityQueue.hpp"
#include "Pair.hpp"
#include "Logger.hpp"

/**
 * Run the Dijkstra's algorithm for shortest path
 * 
 * @returns 0 on success, 1 on failure
 */
int DijkstraSP::run() {
    // Initialize the result
    m_result.pathLength = 0;

    // Create required data structures for storing the state of the algorithm
    size_t vertexCount = m_graph.getVertexCount();
    DynamicArray<intmax_t> distance(vertexCount, INTMAX_MAX);
    DynamicArray<size_t> previous(vertexCount, SIZE_MAX);
    DynamicArray<bool> explored(vertexCount, false);
    PriorityQueue<Pair<intmax_t, size_t>> pq; // (distance, vertex)

    // Check if the requested start and end vertices are valid
    int startVertex = Parameters::vertexStart;
    if (startVertex < 0 || (size_t)startVertex >= vertexCount) {
        Logger::logln(Logger::ERROR, "Invalid start vertex: ", startVertex);
        return 1;
    }
    int endVertex = Parameters::vertexEnd;
    if (endVertex < 0 || (size_t)endVertex >= vertexCount) {
        Logger::logln(Logger::ERROR, "Invalid end vertex: ", endVertex);
        return 1;
    }

    // Add the start vertex as the first vertex to explore
    distance.set(startVertex, 0);
    pq.push({0, (size_t)startVertex});
    
    // Run the algorithm
    while (!pq.isEmpty()) {
        // Get the vertex with the smallest distance
        Pair<intmax_t, size_t> current = pq.getFront();
        intmax_t currentDistance = current.first();
        size_t currentVertex = current.second();
        pq.pop();

        // Check if we've explored it already
        if (explored.get(currentVertex)) continue;
        
        // Set it as explored
        explored.set(currentVertex, true);

        // Check if we reached the end vertex for early exit
        if (currentVertex == (size_t)endVertex) {
            m_result.pathLength = currentDistance;
            // Backtrack the path from end to start
            size_t pathVertex = (size_t)endVertex;
            while (pathVertex != SIZE_MAX) {
                m_result.path.push(pathVertex);
                pathVertex = previous.get(pathVertex);
            }
            break;
        }

        // Get the neighbors of the current vertex
        DynamicArray<size_t> neighbors = m_graph.getAdjacentVertices(currentVertex);
        for (size_t i = 0; i < neighbors.size(); i++) {
            size_t neighbor = neighbors.get(i);
            intmax_t edgeWeight = m_graph.getEdgeWeight(currentVertex, neighbor);
            if (edgeWeight < 0) {
                Logger::logln(Logger::ERROR, "Negative edge weight detected from vertex ", currentVertex, " to vertex ", neighbor);
                return 1;
            }
            
            intmax_t newDistance = currentDistance + edgeWeight;

            // Check if we found a shorter path to the neighbor
            if (newDistance < distance.get(neighbor)) {
                distance.set(neighbor, newDistance);
                previous.set(neighbor, currentVertex);
                pq.push({newDistance, neighbor});
            }
        }
    }

    this->m_resultReady = true;
    if (m_result.path.isEmpty()) {
        Logger::logln(Logger::ERROR, "No path found from vertex ", startVertex, " to vertex ", endVertex);
        return 1;
    }
    return 0;
}

/**
 * Returns the result of the algorithm
 * 
 * @returns the result of the algorithm
 */
GraphAlgorithmResult& DijkstraSP::result() {
    return m_result;
}