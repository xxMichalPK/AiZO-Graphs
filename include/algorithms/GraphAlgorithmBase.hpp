#ifndef GRAPHALGORITHMBASE_HPP
#define GRAPHALGORITHMBASE_HPP

#include "GraphAlgorithmResult.hpp"

class GraphAlgorithmBase {
    protected:
        bool m_resultReady = false;

    protected:
        GraphAlgorithmBase() = default;

    public:
        virtual ~GraphAlgorithmBase() = default;

        virtual int run() = 0;
        virtual GraphAlgorithmResult& result() = 0;
        virtual bool resultReady() const {
            return m_resultReady;
        }
};

#endif // GRAPHALGORITHMBASE_HPP