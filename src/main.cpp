#include <iostream>

int main() {
    std::cout << "Hello, AiZO-Graphs!\n";
    #if GRAPHVIZ_SUPPORT
        std::cout << "Graphviz support is enabled.\n";
    #else
        std::cout << "Graphviz support is disabled.\n";
    #endif
    return 0;
}