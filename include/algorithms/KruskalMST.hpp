#ifndef KRUSKALMST_HPP
#define KRUSKALMST_HPP

#include "GraphRepr.hpp"
#include "GraphAlgorithmBase.hpp"
#include "MSTResult.hpp"

class KruskalMST : public GraphAlgorithmBase {
    private:
        GraphRepr& m_graph;
        MSTResult m_result {};

    public:
        KruskalMST(GraphRepr& graph) : GraphAlgorithmBase("Kruskal Minimum Spanning Tree"), m_graph(graph) {}

        virtual int run() override;
        virtual GraphAlgorithmResult& result() override;
        using GraphAlgorithmBase::resultReady;
};

#endif // KRUSKALMST_HPP