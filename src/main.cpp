#include <iostream>
#include "DataParser.hpp"
#include "IncidenceMatrix.hpp"
#include "AdjacencyList.hpp"

int main() {
    // Get the size and check if it is valid
    DataParser::graphSize_t graphSize = DataParser::getGraphSize("graph_data.csv");
    if (graphSize.vertices == 0 && graphSize.edges == 0) return 1;

    // Create and load the graph
    AdjacencyList graph(graphSize.vertices, graphSize.edges);
    if (!DataParser::loadGraph("graph_data.csv", graph)) return 1;
    
    #if GRAPHVIZ_SUPPORT
        std::cout << "Graphviz support is enabled.\n";
        graph.exportToGraphviz("graph_output.dot");
    #else
        std::cout << "Graphviz support is disabled.\n";
    #endif
    return 0;
}