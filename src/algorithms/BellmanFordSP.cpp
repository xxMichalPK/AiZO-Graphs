#include "Parameters.h"
#include "BellmanFordSP.hpp"
#include "DynamicArray.hpp"
#include "PriorityQueue.hpp"
#include "Pair.hpp"
#include "Logger.hpp"

/**
 * Run the Bellman-Ford algorithm for shortest path
 * 
 * @returns 0 on success, 1 on failure
 */
int BellmanFordSP::run() {
    // Initialize the result
    m_result.pathLength = 0;

    // Create required data structures for storing the state of the algorithm
    size_t vertexCount = m_graph.getVertexCount();
    DynamicArray<intmax_t> distance(vertexCount, INTMAX_MAX);
    DynamicArray<size_t> previous(vertexCount, SIZE_MAX);
    
    // (weight, (startVertex, endVertex))
    DynamicArray<Pair<intmax_t, Pair<size_t, size_t>>> edges = m_graph.getAllEdges();

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

    // Set the distance to the start vertex as 0
    distance.set(startVertex, 0);

    // Run the algorithm for vertexCount - 1 iterations
    bool checkNegativeCycle = true;
    for (size_t i = 0; i < vertexCount - 1; i++) {
        bool updated = false;
        for (size_t j = 0; j < edges.size(); j++) {
            Pair<intmax_t, Pair<size_t, size_t>> edge = edges.get(j);
            intmax_t weight = edge.first();
            size_t u = edge.second().first();
            size_t v = edge.second().second();

            if (distance.get(u) != INTMAX_MAX && distance.get(v) > distance.get(u) + weight) {
                distance.set(v, distance.get(u) + weight);
                previous.set(v, u);
                updated = true;
            }
        }
        if (!updated) {
            checkNegativeCycle = false;
            break;
        }
    }

    if (checkNegativeCycle) {
        // Check for negative cycle
        for (size_t j = 0; j < edges.size(); j++) {
            Pair<intmax_t, Pair<size_t, size_t>> edge = edges.get(j);
            intmax_t weight = edge.first();
            size_t u = edge.second().first();
            size_t v = edge.second().second();

            if (distance.get(u) != INTMAX_MAX && distance.get(v) > distance.get(u) + weight) {
                Logger::logln(Logger::ERROR, "Negative cycle detected in the graph!");
                return 1;
            }
        }
    }

    // Get the path and distance to the end vertex
    if (distance.get(endVertex) == INTMAX_MAX) {
        Logger::logln(Logger::ERROR, "No path found from vertex ", startVertex, " to vertex ", endVertex);
        return 1;
    }

    m_result.pathLength = distance.get(endVertex);
    size_t pathVertex = (size_t)endVertex;
    while (pathVertex != SIZE_MAX) {
        m_result.path.push(pathVertex);
        pathVertex = previous.get(pathVertex);
    }
    return 0;
}

/**
 * Returns the result of the algorithm
 * 
 * @returns the result of the algorithm
 */
GraphAlgorithmResult& BellmanFordSP::result() {
    return m_result;
}