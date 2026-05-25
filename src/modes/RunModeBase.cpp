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
            Logger::logln(Logger::INFO, "Using Prim's algorithm");
            algorithms->insert(new PrimMST(graph));
            break;
        case Parameters::Algorithms::kruskal:
            Logger::logln(Logger::INFO, "Using Kruskal's algorithm");
            algorithms->insert(new KruskalMST(graph));
            break;
        case Parameters::Algorithms::dijkstra:
            Logger::logln(Logger::INFO, "Using Dijkstra's algorithm");
            algorithms->insert(new DijkstraSP(graph));
            break;
        case Parameters::Algorithms::bellmanFord:
            Logger::logln(Logger::INFO, "Using Bellman-Ford algorithm");
            algorithms->insert(new BellmanFordSP(graph));
            break;
        case Parameters::Algorithms::fordFulkerson:
            Logger::logln(Logger::INFO, "Using Ford-Fulkerson algorithm");
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
            Logger::logln(Logger::INFO, "Using Prim's and Kruskal's algorithms");
            algorithms->insert(new PrimMST(graph));
            algorithms->insert(new KruskalMST(graph));
            break;
        case Parameters::Problems::sp:
            Logger::logln(Logger::INFO, "Using Dijkstra's and Bellman-Ford algorithms");
            algorithms->insert(new DijkstraSP(graph));
            algorithms->insert(new BellmanFordSP(graph));
            break;
        case Parameters::Problems::mf:
            Logger::logln(Logger::INFO, "Using Ford-Fulkerson algorithm");
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