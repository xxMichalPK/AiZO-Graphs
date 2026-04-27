#include "DataParser.hpp"
#include <iostream>
#include <fstream>

DataParser::graphSize_t DataParser::getGraphSize(const std::filesystem::path& filename) {
    if (!fileExists(filename)) return {0, 0};

    // Open the file
    std::ifstream dataFile(filename);
    if (!validateOpen(dataFile)) return {0, 0};

    // Read the number of vertices and edges
    size_t numVertices, numEdges;
    dataFile >> numVertices >> numEdges;

    // Return the size
    return {numVertices, numEdges};
}

bool DataParser::loadGraph(const std::filesystem::path& filename, GraphRepr& graph) {
    if (!fileExists(filename)) return false;

    // Open the file
    std::ifstream dataFile(filename);
    if (!validateOpen(dataFile)) return false;

    // Read the number of vertices and edges
    size_t numVertices, numEdges;
    dataFile >> numVertices >> numEdges;

    // For each edge read the start, end end weight of the edge and add it to the graph representation
    size_t lastStartVertex = 0;
    for (size_t i = 0; i < numEdges; ++i) {
        size_t start, end;
        intmax_t weight;

        dataFile >> start >> end >> weight;
        // Check for any errors when reading indicating we don't have
        // enough edges specified
        if (dataFile.fail()) {
            std::cerr << "Error: Wrong data size.\n";
            return false;
        }

        // Validate if the vertices are in order as specified in the instruction
        if ((start < lastStartVertex) || (start > lastStartVertex + 1)) {
            std::cerr << "Error: Edges are not in the correct order.\n";
            return false;
        }
        lastStartVertex = start;

        // Add the edge
        graph.addEdge(start, end, weight);
    }

    return true;
}

bool DataParser::fileExists(const std::filesystem::path& filename) {
    // Check if the file exists
    if (!std::filesystem::exists(filename)) {
        std::cerr << "Error: Input file "<< filename << " does not exist.\n";
        return false;
    }
    return true;
}

bool DataParser::validateOpen(std::ifstream& dataFile) {
    if (!dataFile.is_open()) {
        std::cerr << "Error: Could not open file.\n";
        return false;
    }
    return true;
}