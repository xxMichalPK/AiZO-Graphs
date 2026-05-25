#ifndef FORDFULKERSONMF_HPP
#define FORDFULKERSONMF_HPP

#include "GraphRepr.hpp"
#include "GraphAlgorithmBase.hpp"
#include "MFResult.hpp"

class FordFulkersonMF : public GraphAlgorithmBase {
    private:
        GraphRepr& m_graph;
        GraphRepr* m_residualGraph = nullptr;
        MFResult m_result {};
        size_t m_visitedToken = 1;

    public:
        FordFulkersonMF(GraphRepr& graph);
        ~FordFulkersonMF();

        virtual int run() override;
        virtual GraphAlgorithmResult& result() override;
        using GraphAlgorithmBase::resultReady;
    
    private:
        intmax_t dfsSolve(size_t vertex, intmax_t flow, DynamicArray<size_t>& visited);
};

#endif // FORDFULKERSONMF_HPP