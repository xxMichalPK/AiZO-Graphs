#include "RunModeBase.hpp"

#include "PrimMST.hpp"
#include "KruskalMST.hpp"

#include "DijkstraSP.hpp"
#include "BellmanFordSP.hpp"

#include "FordFulkersonMF.hpp"


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
            delete representations;
            return nullptr;
    }

    return representations;
}

void RunModeBase::deleteRepresentations(DynamicArray<GraphRepr*>* representations) {
    if (representations == nullptr) return;

    for (size_t i = 0; i < representations->size(); i++) {
        delete representations->get(i);
    }
    delete representations;
}

DynamicArray<GraphAlgorithmBase*>* RunModeBase::createAlgorithms(GraphRepr& graph) {
    if (Parameters::algorithm == Parameters::Algorithms::allAlgorithms) return createPossibleAlgorithms(graph);

    DynamicArray<GraphAlgorithmBase*>* algorithms = new DynamicArray<GraphAlgorithmBase*>(1);

    switch (Parameters::algorithm) {
        case Parameters::Algorithms::allAlgorithms:
            delete algorithms;
            return createPossibleAlgorithms(graph);
        case Parameters::Algorithms::prim:
            algorithms->insert(new PrimMST(graph));
            break;
        case Parameters::Algorithms::kruskal:
            algorithms->insert(new KruskalMST(graph));
            break;
        case Parameters::Algorithms::dijkstra:
            algorithms->insert(new DijkstraSP(graph));
            break;
        case Parameters::Algorithms::bellmanFord:
            algorithms->insert(new BellmanFordSP(graph));
            break;
        case Parameters::Algorithms::fordFulkerson:
            algorithms->insert(new FordFulkersonMF(graph));
            break;
        default:
            Logger::logln(Logger::WARNING, "No algorithm selected");
            delete algorithms;
            return nullptr;
    }

    return algorithms;
}

DynamicArray<GraphAlgorithmBase*>* RunModeBase::createPossibleAlgorithms(GraphRepr& graph) {
    DynamicArray<GraphAlgorithmBase*>* algorithms = new DynamicArray<GraphAlgorithmBase*>(2);

    switch (Parameters::problem) {
        case Parameters::Problems::mst:
            algorithms->insert(new PrimMST(graph));
            algorithms->insert(new KruskalMST(graph));
            break;
        case Parameters::Problems::sp:
            algorithms->insert(new DijkstraSP(graph));
            algorithms->insert(new BellmanFordSP(graph));
            break;
        case Parameters::Problems::mf:
            algorithms->insert(new FordFulkersonMF(graph));
            break;
        default:
            Logger::logln(Logger::WARNING, "No problem type selected");
            delete algorithms;
            return nullptr;
    }

    return algorithms;
}

void RunModeBase::deleteAlgorithms(DynamicArray<GraphAlgorithmBase*>* algorithms) {
    if (algorithms == nullptr) return;

    for (size_t i = 0; i < algorithms->size(); i++) {
        delete algorithms->get(i);
    }
    delete algorithms;
}