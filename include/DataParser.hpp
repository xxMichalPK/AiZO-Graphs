#ifndef DATA_PARSER_HPP
#define DATA_PARSER_HPP

#include "GraphRepr.hpp"
#include <filesystem>

class DataParser {
    public:
        // Use a custom struct to represent the size of the graph
        typedef struct graphSize {
            size_t vertices;
            size_t edges;
        } graphSize_t;
    
    public:
        static graphSize_t getGraphSize(const std::filesystem::path& filename);
        static bool loadGraph(const std::filesystem::path& filename, GraphRepr& graph);

    private:
        static bool validateOpen(std::ifstream& dataFile);
        static bool fileExists(const std::filesystem::path& filename);
};

#endif // DATA_PARSER_HPP
