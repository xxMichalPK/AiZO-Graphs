#include "DataParser.hpp"
#include "Logger.hpp"
#include <iostream>
#include <fstream>

/**
 * Gets the size of the graph represented by the number of vertices and edges
 * from the given file.
 * 
 * @param filename The path to the file containing the graph data
 * @return A struct containing the number of vertices and edges, or {0, 0} if there was an error
 */
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


/**
 * Loads the graph data from the given file into the given graph representation.
 * 
 * @param filename The path to the file containing the graph data
 * @param graph The graph representation to load the data into
 * @return true if the graph was loaded successfully, false otherwise
 */
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
            Logger::logln(Logger::ERROR, "Wrong data size.");
            return false;
        }

        // Validate if the vertices are in order as specified in the instruction
        if ((start < lastStartVertex) || (start > lastStartVertex + 1)) {
            Logger::logln(Logger::ERROR, "Edges are not in the correct order.");
            return false;
        }
        lastStartVertex = start;

        // Add the edge
        graph.addEdge(start, end, weight);
    }

    return validateAllRead(dataFile);
}

/**
 * Check if the given file exists, if not displays an error message
 * 
 * @param filename The path to the file to check
 * @return true if the file exists, false otherwise
 */
bool DataParser::fileExists(const std::filesystem::path& filename) {
    // Check if the file exists
    if (!std::filesystem::exists(filename)) {
        Logger::logln(Logger::ERROR, "Input file ", filename, " does not exist.");
        return false;
    }
    return true;
}

/**
 * Simple function that checks if the file is open, if not it displays an error
 * 
 * @param dataFile The file stream to check
 * @return true if the file is open, false otherwise
 */
bool DataParser::validateOpen(std::ifstream& dataFile) {
    if (!dataFile.is_open()) {
        Logger::logln(Logger::ERROR, "Could not open file.");
        return false;
    }
    return true;
}

/**
 * Simple function that checks if the file has been read completely, if not it displays an error
 * 
 * @param dataFile The file stream to check
 * @return true if the file has been read completely, false otherwise
 */
bool DataParser::validateAllRead(std::ifstream& dataFile) {
    if (dataFile >> std::ws && !dataFile.eof()) {
        Logger::logln(Logger::ERROR, "Too much data in file.");
        return false;
    }
    return true;
}