#ifndef BELLMANFORDSP_HPP
#define BELLMANFORDSP_HPP

#include "GraphRepr.hpp"
#include "GraphAlgorithmBase.hpp"
#include "SPResult.hpp"

class BellmanFordSP : public GraphAlgorithmBase {
    private:
        GraphRepr& m_graph;
        SPResult m_result {};

    public:
        BellmanFordSP(GraphRepr& graph) : m_graph(graph) {}

        virtual int run() override;
        virtual GraphAlgorithmResult& result() override;
        using GraphAlgorithmBase::resultReady;
};

#endif // BELLMANFORDSP_HPP