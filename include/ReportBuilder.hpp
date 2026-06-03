#ifndef REPORTBUILDER_HPP
#define REPORTBUILDER_HPP

#include <string>

#include "GraphRepr.hpp"
#include "GraphAlgorithmResult.hpp"
#include "GraphAlgorithmBase.hpp"

class ReportBuilder {
    public:
        ReportBuilder() = delete;

        static std::string buildReport(size_t reportIndex, GraphRepr* representation,
                                       GraphAlgorithmBase* algorithm, GraphAlgorithmResult* result, size_t durationMicroseconds = 0);
    
    private:
        static std::string buildReportHeader(size_t reportIndex, GraphRepr* representation, size_t durationMicroseconds = 0);
        static std::string buildMSTReport(GraphRepr* representation, GraphAlgorithmBase* algorithm, GraphAlgorithmResult* result);
        static std::string buildSPReport(GraphRepr* representation, GraphAlgorithmBase* algorithm, GraphAlgorithmResult* result);
        static std::string buildMFReport(GraphRepr* representation, GraphAlgorithmBase* algorithm, GraphAlgorithmResult* result);
};

#endif // REPORTBUILDER_HPP