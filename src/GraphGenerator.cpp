#include "GraphGenerator.hpp"
#include "DynamicArray.hpp"
#include "Logger.hpp"
#include <random>

bool GraphGenerator::generate(GraphRepr &graph, size_t vertexCount, size_t edgeCount, bool directed) {
    if (directed) {
        return generateDirected(graph, vertexCount, edgeCount);
    }
    
    return generateUndirected(graph, vertexCount, edgeCount);
}

bool GraphGenerator::generateDirected(GraphRepr &graph, size_t vertexCount, size_t edgeCount) {
    (void)graph;
    (void)vertexCount;
    (void)edgeCount;
    return false;
}

bool GraphGenerator::generateUndirected(GraphRepr &graph, size_t vertexCount, size_t edgeCount) {
    DynamicArray<size_t> freeVertices(vertexCount + 1);
    DynamicArray<size_t> usedVertices(vertexCount + 1);
    
    // Add all the vertices to the array of free vertices
    for (size_t i = 0; i < vertexCount; i++) {
        freeVertices.insert(i);
    }

    std::srand(std::time(nullptr));
    // Insert the starting vertex manually
    int vertexIdx = std::rand() % freeVertices.size();
    size_t vertexValue = freeVertices.get(vertexIdx);
    freeVertices.removeAt(vertexIdx);
    usedVertices.insert(vertexValue);

    while (usedVertices.size() < vertexCount) {
        // Get the on of the previously added vertices
        int prevIdx = std::rand() % usedVertices.size();
        size_t prevValue = usedVertices.get(prevIdx);

        vertexIdx = std::rand() % freeVertices.size();
        vertexValue = freeVertices.get(vertexIdx);
        // Check if the edge exists and skip it
        if (graph.checkEdge(prevValue, vertexValue)) continue;

        // Add the new edge
        intmax_t maxWeight = (intmax_t)((double)edgeCount * (4. / 5.));
        maxWeight = maxWeight == 0 ? 1 : maxWeight;
        intmax_t weight = (std::rand() % maxWeight) + 1;
        graph.addEdge(prevValue, vertexValue, weight);

        // Remove the vertex from the free array and add to used
        usedVertices.insert(vertexValue);
        freeVertices.removeAt(vertexIdx);
    }

    return true;
}