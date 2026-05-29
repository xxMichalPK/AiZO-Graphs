#ifndef RUNMODEBASE_HPP
#define RUNMODEBASE_HPP

#include <string>
#include "Parameters.h"
#include "Logger.hpp"
#include "DynamicArray.hpp"
#include "AdjacencyList.hpp"
#include "IncidenceMatrix.hpp"
#include "GraphAlgorithmBase.hpp"
#include "GraphRepr.hpp"

class RunModeBase {
    public:
        RunModeBase() = delete;
        virtual ~RunModeBase() = default;

        static bool isDirected();

    protected:
        static DynamicArray<GraphRepr*>* createRepresentations(size_t vertexCount, size_t edgeCount);
        static void deleteRepresentations(DynamicArray<GraphRepr*>* representations);

        static DynamicArray<GraphAlgorithmBase*>* createAlgorithms(GraphRepr& graph);
        static void deleteAlgorithms(DynamicArray<GraphAlgorithmBase*>* algorithms);
    
        static void exportGraphImages(DynamicArray<GraphRepr*>* representations);
        
    private:
        static bool confirmMatrixSize(size_t vertexCount, size_t edgeCount);
        static DynamicArray<GraphAlgorithmBase*>* createMSTAlgorithms(GraphRepr& graph);
        static DynamicArray<GraphAlgorithmBase*>* createSPAlgorithms(GraphRepr& graph);
        static DynamicArray<GraphAlgorithmBase*>* createMFAlgorithms(GraphRepr& graph);

        static std::string getSelectedAlgorithmName();
        static std::string getSelectedProblemName();
};

#endif // RUNMODEBASE_HPP