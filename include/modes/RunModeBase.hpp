#ifndef RUNMODEBASE_HPP
#define RUNMODEBASE_HPP

#include "Parameters.h"
#include "Logger.hpp"
#include "DynamicArray.hpp"
#include "AdjacencyList.hpp"
#include "IncidenceMatrix.hpp"

class RunModeBase {
    public:
        RunModeBase() = delete;
        virtual ~RunModeBase() = default;

        /**
         * Tells you if the graph should be directed or not
         * 
         * @returns true if it should be directed, false otherwise
         */
        static bool isDirected();

        /**
         * Creates all requested graph representations
         * 
         * @param vertexCount number of vertices in the graph
         * @param edgeCount number of edges in the graph
         * 
         * @returns a dynamic array of graph representations, nullptr on failure
         */
        static DynamicArray<GraphRepr*>* createRepresentations(size_t vertexCount, size_t edgeCount);
};

#endif // RUNMODEBASE_HPP