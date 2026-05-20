#include "PrimMST.hpp"
#include "DynamicArray.hpp"
#include "PriorityQueue.hpp"
#include "Pair.hpp"
#include "Logger.hpp"

int PrimMST::run() {
    m_result.pathLength = 0;

    size_t vertexCount = m_graph.getVertexCount();
    DynamicArray<intmax_t> key(vertexCount, INTMAX_MAX);
    DynamicArray<intmax_t> p(vertexCount, -1);
    DynamicArray<bool> inMST(vertexCount, false);

    size_t startVertex = 0;
    key.set(startVertex, 0);

    PriorityQueue<Pair<intmax_t, size_t>> pq;
    pq.push(Pair<intmax_t, size_t>(0, startVertex));

    while (!pq.isEmpty()) {
        Pair<intmax_t, size_t> currentPair = pq.getFront();
        pq.pop();

        size_t currentVertex = currentPair.second();
        if (inMST.get(currentVertex)) continue; // Skip if the vertex is already in the MST

        inMST.set(currentVertex, true);
        m_result.pathLength += currentPair.first();

        Logger::logln(Logger::WARNING, "Adding vertex ", currentVertex, " to the MST with edge weight ", currentPair.first());
        std::cin.get(); // Wait for user input to proceed

        DynamicArray<size_t> adjacentVertices = m_graph.getAdjacentVertices(currentVertex);
        for (size_t i = 0; i < adjacentVertices.size(); i++) {
            size_t adjacentVertex = adjacentVertices.get(i);
            intmax_t weight = m_graph.getEdgeWeight(currentVertex, adjacentVertex);
            Logger::logln(Logger::INFO, "Checking adjacent vertex ", adjacentVertex, " with edge weight ", weight);

            if (!inMST.get(adjacentVertex) && weight < key.get(adjacentVertex)) {
                key.set(adjacentVertex, weight);
                p.set(adjacentVertex, currentVertex);
                pq.push(Pair<intmax_t, size_t>(weight, adjacentVertex));
            }
        }
    }

    return 0;
}

GraphAlgorithmResult& PrimMST::result() {
    return m_result;
}