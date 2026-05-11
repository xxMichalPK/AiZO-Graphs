#include "GraphGenerator.hpp"
#include "DynamicArray.hpp"
#include "Logger.hpp"
#include <random>

/**
 * Generates a connected graph based on provided arguments and parsed parameters
 * 
 * @param graph the graph to generate the edges into
 * @param vertexCount number of vertices of the final graph
 * @param edgeCount number of edges in the final graph
 * @param directed if the graph should be a directed or undirected one
 * 
 * @returns true if generation was successful, false otherwise
 */
bool GraphGenerator::generate(GraphRepr &graph, size_t vertexCount, size_t edgeCount, bool directed) {
    if (directed) {
        return generateDirected(graph, vertexCount, edgeCount);
    }
    
    return generateUndirected(graph, vertexCount, edgeCount);
}

/**
 * Generates a directed graph based on provided arguments and parsed parameters
 * 
 * @param graph the graph to generate the edges into
 * @param vertexCount number of vertices of the final graph
 * @param edgeCount number of edges in the final graph
 * 
 * @returns true if generation was successful, false otherwise
 */
bool GraphGenerator::generateDirected(GraphRepr &graph, size_t vertexCount, size_t edgeCount) {
    if (edgeCount < vertexCount - 1) {
        Logger::logln(Logger::ERROR, "Impossible to generate a connected graph with ", vertexCount, " vertices and ",
                                    edgeCount, " edges!");
        return false;
    }

    std::srand(std::time(nullptr));

    // In a directed graph we have to connect all the points (maybe other algorithm in the future)
    size_t prevStart = 0;
    size_t currentVertex = 1;
    size_t remainingEdgeCount = edgeCount;
    for (size_t i = 0; i < vertexCount - 1; i++) {
        intmax_t maxWeight = (intmax_t)((double)edgeCount * (4. / 5.));
        maxWeight = maxWeight == 0 ? 1 : maxWeight;
        intmax_t weight = (std::rand() % maxWeight) + 1;
        graph.addEdge(prevStart, currentVertex, weight);
        
        prevStart++;
        currentVertex++;
        remainingEdgeCount--;
    }

    // Add the random connections between vertices that are not yet connected
    while (remainingEdgeCount > 0) {
        int startVertex = std::rand() % vertexCount;
        int endVertex = std::rand() % vertexCount;
        // We've got 2 the same vertices or the edge already exists
        if (startVertex == endVertex || graph.checkEdge(startVertex, endVertex)) continue;

        intmax_t maxWeight = (intmax_t)((double)edgeCount * (4. / 5.));
        maxWeight = maxWeight == 0 ? 1 : maxWeight;
        intmax_t weight = (std::rand() % maxWeight) + 1;
        graph.addEdge(startVertex, endVertex, weight);
        remainingEdgeCount--;
    }

    return true;
}

/**
 * Generates an undirected graph based on provided arguments and parsed parameters
 * 
 * @param graph the graph to generate the edges into
 * @param vertexCount number of vertices of the final graph
 * @param edgeCount number of edges in the final graph
 * 
 * @returns true if generation was successful, false otherwise
 */
bool GraphGenerator::generateUndirected(GraphRepr &graph, size_t vertexCount, size_t edgeCount) {
    // Check if it is even possible to generate such a graph (number of edges has to be at least V-1)
    if (edgeCount < vertexCount - 1) {
        Logger::logln(Logger::ERROR, "Impossible to generate a connected graph with ", vertexCount, " vertices and ",
                                    edgeCount, " edges!");
        return false;
    }

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

    size_t remainingEdgeCount = edgeCount;
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
        remainingEdgeCount--;
    }

    // Add the random connections between vertices that are not yet connected
    while (remainingEdgeCount > 0) {
        int startVertex = std::rand() % vertexCount;
        int endVertex = std::rand() % vertexCount;
        // We've got 2 the same vertices or the edge already exists
        if (startVertex == endVertex || graph.checkEdge(startVertex, endVertex)) continue;

        intmax_t maxWeight = (intmax_t)((double)edgeCount * (4. / 5.));
        maxWeight = maxWeight == 0 ? 1 : maxWeight;
        intmax_t weight = (std::rand() % maxWeight) + 1;
        graph.addEdge(startVertex, endVertex, weight);
        remainingEdgeCount--;
    }

    return true;
}