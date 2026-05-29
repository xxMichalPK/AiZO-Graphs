#ifndef GRAPHALGORITHMBASE_HPP
#define GRAPHALGORITHMBASE_HPP

#include <string>
#include "GraphAlgorithmResult.hpp"

class GraphAlgorithmBase {
    private:
        std::string m_name;
        std::string m_id;

    protected:
        bool m_resultReady = false;

    protected:
        GraphAlgorithmBase(const std::string& name, const std::string& id) : m_name(name), m_id(id) {}

    public:
        virtual ~GraphAlgorithmBase() = default;

        virtual int run() = 0;
        virtual GraphAlgorithmResult& result() = 0;
        virtual bool resultReady() const {
            return m_resultReady;
        }
        const std::string& name() const {
            return m_name;
        }
        const std::string& id() const {
            return m_id;
        }
};

#endif // GRAPHALGORITHMBASE_HPP