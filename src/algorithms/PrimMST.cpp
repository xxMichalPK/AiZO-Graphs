#include "PrimMST.hpp"
#include "DynamicArray.hpp"
#include "PriorityQueue.hpp"
#include "Pair.hpp"
#include "Logger.hpp"

/**
 * Run the Prim's algorithm for minimum spanning tree
 * 
 * @returns 0 on success
 */
int PrimMST::run() {
    // Initialize the result
    m_result.pathLength = 0;

    // Create required data structures for storing the state of the algorithm
    size_t vertexCount = m_graph.getVertexCount();
    DynamicArray<intmax_t> key(vertexCount, INTMAX_MAX);
    DynamicArray<intmax_t> p(vertexCount, -1);
    DynamicArray<bool> inMST(vertexCount, false);

    // Start with the first vertex
    size_t startVertex = 0;
    key.set(startVertex, 0);

    // Create a min-heap based priority and add the first vertex with weight 0
    PriorityQueue<Pair<intmax_t, size_t>> pq;
    pq.push(Pair<intmax_t, size_t>(0, startVertex));

    while (!pq.isEmpty()) {
        // Get the vertex with the smallest weight and remove it from the queue
        Pair<intmax_t, size_t> currentPair = pq.getFront();
        size_t currentVertex = currentPair.second();
        intmax_t currentWeight = currentPair.first();
        pq.pop();

        // Skip if the vertex is already in the MST
        if (inMST.get(currentVertex)) continue;

        // Add the vertex to the MST and update the result
        inMST.set(currentVertex, true);
        m_result.pathLength += currentWeight;

        // Push the edge to the result if it's not the starting vertex
        if (p.get(currentVertex) != -1) {
            size_t parentVertex = (size_t)p.get(currentVertex);
            m_result.edges.push({currentWeight, {parentVertex, currentVertex}});
        }

        // Find all the neighbors of the current vertex and add them to the pq if needed
        DynamicArray<size_t> adjacentVertices = m_graph.getAdjacentVertices(currentVertex);
        for (size_t i = 0; i < adjacentVertices.size(); i++) {
            size_t adjacentVertex = adjacentVertices.get(i);
            intmax_t adjacentWeight = m_graph.getEdgeWeight(currentVertex, adjacentVertex);

            // If the adjacent vertex is already in the MST or the weight of the edge is greater than
            // (or equal) the current key value for that vertex, skip it
            if (inMST.get(adjacentVertex) || adjacentWeight >= key.get(adjacentVertex)) continue;

            // Otherwise update the key and parent for that vertex and add it to the priority queue
            key.set(adjacentVertex, adjacentWeight);
            p.set(adjacentVertex, currentVertex);
            pq.push(Pair<intmax_t, size_t>(adjacentWeight, adjacentVertex));
        }
    }

    this->m_resultReady = true;
    return 0;
}

/**
 * Returns the result of the algorithm
 * 
 * @returns the result of the algorithm
 */
GraphAlgorithmResult& PrimMST::result() {
    return m_result;
}