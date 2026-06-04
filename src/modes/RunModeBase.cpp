#include "RunModeBase.hpp"

// Algorithms
#include "PrimMST.hpp"
#include "KruskalMST.hpp"

#include "DijkstraSP.hpp"
#include "BellmanFordSP.hpp"

#include "FordFulkersonMF.hpp"


/**
 * Checks if the graph should be directed based on the problem type
 * as specified in the instruction
 * 
 * @returns true if the graph should be directed, false otherwise
 */
bool RunModeBase::isDirected() {
    return (Parameters::problem != Parameters::Problems::mst);
}


/**
 * Creates all requested graph representations based on the provided parameters
 * 
 * @param vertexCount number of vertices in the graph
 * @param edgeCount number of edges in the graph
 * 
 * @returns an array of graph representations, nullptr on failure
 */
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
            
            if (!confirmMatrixSize(vertexCount, edgeCount)) break;
            representations->insert(new IncidenceMatrix(vertexCount, edgeCount, directed));
            break;
        case Parameters::Structures::allStructures:
            Logger::logln(Logger::INFO, "Using all available graph representations");
            representations->insert(new AdjacencyList(vertexCount, edgeCount, directed));
            // Always confirm the incidence matrix cause it can explode quickly
            if (!confirmMatrixSize(vertexCount, edgeCount)) break;
            representations->insert(new IncidenceMatrix(vertexCount, edgeCount, directed));
            break;
        default:
            Logger::logln(Logger::ERROR, "No representation selected");
            delete representations;
            return nullptr;
    }

    return representations;
}


/**
 * Safely deletes all graph representations and the array itself
 * 
 * @param representations the array of graph representations to delete
 */
void RunModeBase::deleteRepresentations(DynamicArray<GraphRepr*>* representations) {
    if (representations == nullptr) return;

    for (size_t i = 0; i < representations->size(); i++) {
        delete representations->get(i);
    }
    delete representations;
}


/**
 * With large vertex and edge counts, the matrix can swallow all the RAM and crash so ask the user to confirm
 * the size when it exceeds 4GB
 */
bool RunModeBase::confirmMatrixSize(size_t vertexCount, size_t edgeCount) {
    const size_t edgeSize = sizeof(intmax_t);
    const size_t totalSize = vertexCount * edgeCount * edgeSize;
    const size_t totalSizeGB = totalSize / (1024ull * 1024 * 1024);

    if (totalSizeGB < 4) return true;

    Logger::log(Logger::WARNING, "The matrix representation is about to use ", totalSizeGB, " GB of RAM. Do you want to continue? (y/n): ");
    char response;
    std::cin >> response;
    return (response == 'y' || response == 'Y');
}


/**
 * Creates all requested algorithms based on the provided parameters
 * 
 * @param graph the graph for which to create algorithms
 * 
 * @returns an array of algorithms, nullptr on failure
 */
DynamicArray<GraphAlgorithmBase*>* RunModeBase::createAlgorithms(GraphRepr& graph) {
    switch (Parameters::problem) {
        case Parameters::Problems::mst:
            return createMSTAlgorithms(graph);
        case Parameters::Problems::sp:
            return createSPAlgorithms(graph);
        case Parameters::Problems::mf:
            return createMFAlgorithms(graph);
        default:
            Logger::logln(Logger::ERROR, "No problem type selected");
    }

    return nullptr;
}


/**
 * Safely deletes all algorithms and the array itself
 * 
 * @param algorithms the array of algorithms to delete
 */
void RunModeBase::deleteAlgorithms(DynamicArray<GraphAlgorithmBase*>* algorithms) {
    if (algorithms == nullptr) return;

    for (size_t i = 0; i < algorithms->size(); i++) {
        delete algorithms->get(i);
    }
    delete algorithms;
}


/**
 * Creates all MST algorithms based on the provided parameters
 * 
 * @param graph the graph for which to create algorithms
 * 
 * @returns an array of algorithms, nullptr on failure
 */
DynamicArray<GraphAlgorithmBase*>* RunModeBase::createMSTAlgorithms(GraphRepr& graph) {
    if (Parameters::problem != Parameters::Problems::mst) return nullptr;

    switch (Parameters::algorithm) {
        case Parameters::Algorithms::allAlgorithms: {
            DynamicArray<GraphAlgorithmBase*>* algorithms = new DynamicArray<GraphAlgorithmBase*>(2);
            algorithms->insert(new PrimMST(graph));
            algorithms->insert(new KruskalMST(graph));
            return algorithms;
        }
        case Parameters::Algorithms::prim:
            return new DynamicArray<GraphAlgorithmBase*>(1, new PrimMST(graph));
        case Parameters::Algorithms::kruskal:
            return new DynamicArray<GraphAlgorithmBase*>(1, new KruskalMST(graph));
        default:
            Logger::logln(Logger::WARNING, "Can't create ", getSelectedAlgorithmName(), " algorithm for ", getSelectedProblemName(), " problem");
            return nullptr;
    }
}


/**
 * Creates all SP algorithms based on the provided parameters
 * 
 * @param graph the graph for which to create algorithms
 * 
 * @returns an array of algorithms, nullptr on failure
 */
DynamicArray<GraphAlgorithmBase*>* RunModeBase::createSPAlgorithms(GraphRepr& graph) {
    if (Parameters::problem != Parameters::Problems::sp) return nullptr;

    switch (Parameters::algorithm) {
        case Parameters::Algorithms::allAlgorithms: {
            DynamicArray<GraphAlgorithmBase*>* algorithms = new DynamicArray<GraphAlgorithmBase*>(2);
            algorithms->insert(new DijkstraSP(graph));
            algorithms->insert(new BellmanFordSP(graph));
            return algorithms;
        }
        case Parameters::Algorithms::dijkstra:
            return new DynamicArray<GraphAlgorithmBase*>(1, new DijkstraSP(graph));
        case Parameters::Algorithms::bellmanFord:
            return new DynamicArray<GraphAlgorithmBase*>(1, new BellmanFordSP(graph));
        default:
            Logger::logln(Logger::WARNING, "Can't create ", getSelectedAlgorithmName(), " algorithm for ", getSelectedProblemName(), " problem");
            return nullptr;
    }
}


/**
 * Creates all MF algorithms based on the provided parameters
 * 
 * @param graph the graph for which to create algorithms
 * 
 * @returns an array of algorithms, nullptr on failure
 */
DynamicArray<GraphAlgorithmBase*>* RunModeBase::createMFAlgorithms(GraphRepr& graph) {
    if (Parameters::problem != Parameters::Problems::mf) return nullptr;

    switch (Parameters::algorithm) {
        case Parameters::Algorithms::allAlgorithms:
        case Parameters::Algorithms::fordFulkerson:
            return new DynamicArray<GraphAlgorithmBase*>(1, new FordFulkersonMF(graph));
        default:
            Logger::logln(Logger::WARNING, "Can't create ", getSelectedAlgorithmName(), " algorithm for ", getSelectedProblemName(), " problem");
            return nullptr;
    }
}


/**
 * Returns the name of the selected algorithm based on the provided parameters
 * 
 * @returns the name of the selected algorithm
 */
std::string RunModeBase::getSelectedAlgorithmName() {
    switch (Parameters::algorithm) {
        case Parameters::Algorithms::prim:
            return "Prim's MST";
        case Parameters::Algorithms::kruskal:
            return "Kruskal's MST";
        case Parameters::Algorithms::dijkstra:
            return "Dijkstra's SP";
        case Parameters::Algorithms::bellmanFord:
            return "Bellman-Ford SP";
        case Parameters::Algorithms::fordFulkerson:
            return "Ford-Fulkerson MF";
        default:
            return "Unknown Algorithm";
    }
}


/**
 * Returns the name of the selected problem based on the provided parameters
 * 
 * @returns the name of the selected problem
 */
std::string RunModeBase::getSelectedProblemName() {
    switch (Parameters::problem) {
        case Parameters::Problems::mst:
            return "Minimum Spanning Tree";
        case Parameters::Problems::sp:
            return "Shortest Path";
        case Parameters::Problems::mf:
            return "Maximum Flow";
        default:
            return "Unknown Problem";
    }
}


/**
 * If the graphviz support is enabled and the graph is small enough it exports
 * it as an image using the graphviz tool
 */
void RunModeBase::exportGraphImages(DynamicArray<GraphRepr*>* representations) {
#if GRAPHVIZ_SUPPORT
    if (Parameters::vertexCount <= 10) {
        for (size_t i = 0; i < representations->size(); i++) {
            GraphRepr* representation = representations->get(i);
            // Don't export graphs that have more than 10 vertices because
            // the image is unreadable + it can crash the PC because of running out of memory
            if (representation->getVertexCount() > 10) continue;

            std::string graphName = "graph_repr" + std::to_string(i) + ".dot";
            representation->exportToGraphviz(graphName.c_str());
        }
    }
#endif
}


/**
 * Returns the number of requested graph representations based on the provided parameters
 * 
 * @returns the number of requested graph representations
 */
size_t RunModeBase::getRequestedRepresentationCount() {
    switch (Parameters::structure) {
        case Parameters::Structures::adjacencyList:
        case Parameters::Structures::incidenceMatrix:
            return 1;
        case Parameters::Structures::allStructures:
            return 2;
        default:
            break;
    }
    return 0;
}


/**
 * Returns the number of requested algorithms based on the provided parameters
 * 
 * @returns the number of requested algorithms
 */
size_t RunModeBase::getRequestedAlgorithmCount() {
    switch (Parameters::algorithm) {
        case Parameters::Algorithms::prim:
        case Parameters::Algorithms::kruskal:
        case Parameters::Algorithms::dijkstra:
        case Parameters::Algorithms::bellmanFord:
        case Parameters::Algorithms::fordFulkerson:
            return 1;
        case Parameters::Algorithms::allAlgorithms:
            if (Parameters::problem == Parameters::Problems::mst) return 2;
            if (Parameters::problem == Parameters::Problems::sp) return 2;
            if (Parameters::problem == Parameters::Problems::mf) return 1;
        default:
            break;
    }
    return 0;
}