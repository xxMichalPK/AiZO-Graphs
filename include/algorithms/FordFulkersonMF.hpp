#ifndef FORDFULKERSONMF_HPP
#define FORDFULKERSONMF_HPP

#include "GraphAlgorithmBase.hpp"

#include "GraphRepr.hpp"
#include "MFResult.hpp"

class FordFulkersonMF : public GraphAlgorithmBase {
    private:
        GraphRepr& m_graph;
        GraphRepr* m_residualGraph = nullptr;
        MFResult m_result {};

    public:
        FordFulkersonMF(GraphRepr& graph);
        ~FordFulkersonMF();

        virtual int run() override;
        virtual GraphAlgorithmResult& result() override;
        using GraphAlgorithmBase::resultReady;
    
    private:
        intmax_t dfsSolve(size_t source, size_t sink, DynamicArray<size_t>& parent);
        intmax_t bfsSolve(size_t source, size_t sink, DynamicArray<size_t>& parent);
};

#endif // FORDFULKERSONMF_HPP