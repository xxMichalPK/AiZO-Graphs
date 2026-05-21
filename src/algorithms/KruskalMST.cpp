#include "KruskalMST.hpp"
#include "DisjointSet.hpp"
#include "MergeSort.hpp"
#include "Logger.hpp"

/**
 * Run Kruskal's algorithm for minimum spanning tree
 * 
 * @returns 0 on success
 */
int KruskalMST::run() {
    m_result.pathLength = 0;
    
    DynamicArray<Pair<intmax_t, Pair<size_t, size_t>>> edges = m_graph.getAllEdges();
    DisjointSet set(m_graph.getVertexCount());

    // Sort the edges by weight
    MergeSort<Pair<intmax_t, Pair<size_t, size_t>>>::sort(edges);

    for (size_t i = 0; i < edges.size(); i++) {
        intmax_t weight = edges.get(i).first();
        size_t startVertex = edges.get(i).second().first();
        size_t endVertex = edges.get(i).second().second();

        if (set.find(startVertex) != set.find(endVertex)) {
            set.unionSets(startVertex, endVertex);
            // m_result.edges.push({weight, {startVertex, endVertex}});
            m_result.pathLength += weight;
        }
    }

    return 0;
}

/**
 * Returns the result of the algorithm
 * 
 * @returns the result of the algorithm
 */
GraphAlgorithmResult& KruskalMST::result() {
    return m_result;
}