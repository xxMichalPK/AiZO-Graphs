#ifndef DIJKSTRASP_HPP
#define DIJKSTRASP_HPP

#include "GraphRepr.hpp"
#include "GraphAlgorithmBase.hpp"
#include "SPResult.hpp"

class DijkstraSP : public GraphAlgorithmBase {
    private:
        GraphRepr& m_graph;
        SPResult m_result {};

    public:
        DijkstraSP(GraphRepr& graph) : GraphAlgorithmBase("Dijkstra Shortest Path", "DIJKSTRA"), m_graph(graph) {}

        virtual int run() override;
        virtual GraphAlgorithmResult& result() override;
        using GraphAlgorithmBase::resultReady;
};

#endif // DIJKSTRASP_HPP