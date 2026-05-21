#ifndef SPRESULT_HPP
#define SPRESULT_HPP

#include "GraphAlgorithmResult.hpp"
#include "DynamicArray.hpp"
#include <climits>
#include <iostream>

class SPResult : public GraphAlgorithmResult {
    public:
        intmax_t pathLength = INTMAX_MAX;
        DynamicArray<size_t> path; // Stores the path in reverse order (from destination to source)

    public:
        SPResult() = default;
        
        virtual void print(std::ostream& os) const override {
            os << "Path length: " << pathLength << "\nPath: ";
            os << "(start) ";
            for (size_t i = path.size(); i > 0; i--) {
                size_t vertex = path.get(i - 1);
                os << vertex;

                if (i > 1) {
                    os << " -> ";
                }
            }
            os << " (end)";
        }

        virtual intmax_t cost() override {
            return pathLength;
        }
};

#endif // SPRESULT_HPP