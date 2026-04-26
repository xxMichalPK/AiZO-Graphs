#include <iostream>
#include "IncidenceMatrix.hpp"

int main() {
    std::cout << "Hello, AiZO-Graphs!\n";
    #if GRAPHVIZ_SUPPORT
        std::cout << "Graphviz support is enabled.\n";
        IncidenceMatrix graph(4, 6);
        graph.addEdge(0, 1, 10);
        graph.addEdge(0, 2, 10);
        graph.addEdge(1, 3, 5);
        graph.addEdge(2, 3, 12);
        graph.addEdge(3, 0, 1);
        graph.addEdge(3, 2, 7);
        graph.exportToGraphviz("graph_output.dot");
    #else
        std::cout << "Graphviz support is disabled.\n";
    #endif
    return 0;
}