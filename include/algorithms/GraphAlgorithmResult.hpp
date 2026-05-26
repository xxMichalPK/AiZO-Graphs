#ifndef GRAPHALGORITHMRESULT_HPP
#define GRAPHALGORITHMRESULT_HPP

#include <iostream>
#include <cstdint>

class GraphAlgorithmResult {
    protected:
        GraphAlgorithmResult() = default;

    public:
        virtual ~GraphAlgorithmResult() = default;

        virtual intmax_t cost() = 0;
        virtual void print(std::ostream& os) const = 0;
        virtual std::string resultStringRepresentation() = 0;
};

/**
 * Implement the << operator for piping to std::cout
 * 
 * @param os the output stream
 * @param result the result to print
 */
inline std::ostream& operator<<(std::ostream& os, const GraphAlgorithmResult& result) {
    result.print(os);
    return os;
}

#endif // GRAPHALGORITHMRESULT_HPP