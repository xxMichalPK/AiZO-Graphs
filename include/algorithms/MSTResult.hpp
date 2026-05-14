#ifndef MSTRESULT_HPP
#define MSTRESULT_HPP

#include "GraphAlgorithmResult.hpp"
#include <climits>
#include <iostream>

class MSTResult : public GraphAlgorithmResult {
    public:
        intmax_t pathLength = INTMAX_MAX;

    public:
        MSTResult() = default;
        
        virtual void print(std::ostream& os) const override {
            os << "Path length: " << pathLength;
        }

        virtual intmax_t cost() override {
            return pathLength;
        }
};

#endif // MSTRESULT_HPP