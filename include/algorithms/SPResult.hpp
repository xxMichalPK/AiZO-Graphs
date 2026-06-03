#ifndef SPRESULT_HPP
#define SPRESULT_HPP

#include "GraphAlgorithmResult.hpp"
#include "DynamicArray.hpp"

#include <iostream>

class SPResult : public GraphAlgorithmResult {
    public:
        intmax_t pathLength = INTMAX_MAX;
        DynamicArray<size_t> path; // Stores the path in reverse order (from destination to source)

    public:
        SPResult() = default;
        
        virtual void print(std::ostream& os) const override {
            os << "Path length: " << pathLength;
        }

        virtual intmax_t cost() override {
            return pathLength;
        }

        virtual std::string fullResultString() override {
            std::string result = "";
            for (size_t i = path.size(); i > 0; i--) {
                size_t vertex = path.get(i - 1);
                result += std::to_string(vertex);

                if (i > 1) {
                    result += " -> ";
                }
            }
            return result;
        }
};

#endif // SPRESULT_HPP