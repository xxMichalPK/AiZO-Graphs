#ifndef PRIMMST_HPP
#define PRIMMST_HPP

#include "GraphRepr.hpp"
#include "GraphAlgorithmBase.hpp"
#include "MSTResult.hpp"

class PrimMST : public GraphAlgorithmBase {
    private:
        GraphRepr& m_graph;
        MSTResult m_result {};

    public:
        PrimMST(GraphRepr& graph) : GraphAlgorithmBase("Prim Minimum Spanning Tree", "PRIM"), m_graph(graph) {}

        virtual int run() override;
        virtual GraphAlgorithmResult& result() override;
        using GraphAlgorithmBase::resultReady;
};

#endif // PRIMMST_HPP