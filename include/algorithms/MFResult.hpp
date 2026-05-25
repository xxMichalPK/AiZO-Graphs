#ifndef MFRESULT_HPP
#define MFRESULT_HPP

#include "GraphAlgorithmResult.hpp"
#include <climits>
#include <iostream>

class MFResult : public GraphAlgorithmResult {
    public:
        intmax_t maxFlow = INTMAX_MIN;

    public:
        MFResult() = default;
        
        virtual void print(std::ostream& os) const override {
            os << "Max flow: " << maxFlow;
        }

        virtual intmax_t cost() override {
            return maxFlow;
        }
};

#endif // MFRESULT_HPP