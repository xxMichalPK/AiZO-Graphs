#ifndef MSTRESULT_HPP
#define MSTRESULT_HPP

#include "GraphAlgorithmResult.hpp"
#include <climits>
#include <iostream>

class MSTResult : public GraphAlgorithmResult {
    public:
        intmax_t pathLength = INTMAX_MAX;
        DynamicArray<Pair<intmax_t, Pair<size_t, size_t>>> edges; // Stores the edges in the MST (weight, (startVertex, endVertex))

    public:
        MSTResult() = default;
        
        virtual void print(std::ostream& os) const override {
            os << "Path length: " << pathLength;
            os << "\nEdges in the MST:\n";
            for (size_t i = 0; i < edges.size(); i++) {
                auto edge = edges.get(i);
                intmax_t weight = edge.first();
                size_t startVertex = edge.second().first();
                size_t endVertex = edge.second().second();
                os << startVertex << " -- " << endVertex << " (" << weight << ")" "\n";
            }
        }

        virtual intmax_t cost() override {
            return pathLength;
        }

        virtual std::string resultStringRepresentation() override {
            std::string result = "";
            for (size_t i = 0; i < edges.size(); i++) {
                auto edge = edges.get(i);
                intmax_t weight = edge.first();
                size_t startVertex = edge.second().first();
                size_t endVertex = edge.second().second();
                result += std::to_string(startVertex) + " -- " + std::to_string(endVertex) + " (" + std::to_string(weight) + ")";

                if (i < edges.size() - 1) {
                    result += "\n";
                }
            }
            return result;
        }
};

#endif // MSTRESULT_HPP