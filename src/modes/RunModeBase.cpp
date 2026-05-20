#include "RunModeBase.hpp"

bool RunModeBase::isDirected() {
    return (Parameters::problem != Parameters::Problems::mst);
}

DynamicArray<GraphRepr*>* RunModeBase::createRepresentations(size_t vertexCount, size_t edgeCount) {
    DynamicArray<GraphRepr*>* representations = new DynamicArray<GraphRepr*>(2);
    bool directed = isDirected();

    switch (Parameters::structure) {
        case Parameters::Structures::adjacencyList:
            Logger::logln(Logger::INFO, "Using adjacency list for graph representation");
            representations->insert(new AdjacencyList(vertexCount, edgeCount, directed));
            break;
        case Parameters::Structures::incidenceMatrix:
            Logger::logln(Logger::INFO, "Using incidence matrix for graph representation");
            representations->insert(new IncidenceMatrix(vertexCount, edgeCount, directed));
            break;
        case Parameters::Structures::allStructures:
            Logger::logln(Logger::INFO, "Using all available graph representations");
            representations->insert(new AdjacencyList(vertexCount, edgeCount, directed));
            representations->insert(new IncidenceMatrix(vertexCount, edgeCount, directed));
            break;
        default:
            Logger::logln(Logger::ERROR, "No representation selected");
            return nullptr;
    }

    return representations;
}