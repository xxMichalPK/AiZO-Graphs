#include "ReportBuilder.hpp"
#include "Parameters.h"
#include "Logger.hpp"
#include "SPResult.hpp"

/**
 * Builds a report file with the results of the graph algorithm
 * 
 * @param reportIndex the index of the report
 * @param representation the graph representation on which the algorithm was run
 * @param algorithm the graph algorithm that was run
 * @param result the result of the graph algorithm
 * 
 * @returns the generated report string
 */
std::string ReportBuilder::buildReport(size_t reportIndex, GraphRepr* representation, 
                                       GraphAlgorithmBase* algorithm, GraphAlgorithmResult* result) {

    std::string reportHeader = buildReportHeader(reportIndex, representation);

    std::string detailedReport;
    switch (Parameters::problem) {
        case Parameters::Problems::mst:
            detailedReport = buildMSTReport(representation, algorithm, result);
            break;
        case Parameters::Problems::sp:
            detailedReport = buildSPReport(representation, algorithm, result);
            break;
        case Parameters::Problems::mf:
            detailedReport = buildMFReport(representation, algorithm, result);
            break;
        default:
            Logger::logln(Logger::WARNING, "Undefined problem type. No detailed report will be generated.");
            break;
    }

    return reportHeader + detailedReport;
}


/**
 * Builds the header of the report with general information about the graph
 * 
 * @param reportIndex the index of the report
 * @param representation the graph representation on which the algorithm was run
 */
std::string ReportBuilder::buildReportHeader(size_t reportIndex, GraphRepr* representation) {
    std::string header;
    header += std::string(20, '=') + "\n";
    header += "Report #" + std::to_string(reportIndex) + "\n";
    header += std::string(20, '=') + "\n";
    header += "Type: Directed/Undirected (TO DO)\n";
    header += "Vertex count: " + std::to_string(representation->getVertexCount()) + "\n";
    header += "Edge count: " + std::to_string(representation->getEdgeCount()) + "\n";
    return header;
}


/**
 * Builds the detailed part of the report for the MST problem
 * 
 * @param representation the graph representation on which the algorithm was run
 * @param algorithm the graph algorithm that was run
 * @param result the result of the graph algorithm
 * 
 * @returns the generated report string for the MST problem
 */
std::string ReportBuilder::buildMSTReport(GraphRepr* representation, GraphAlgorithmBase* algorithm, GraphAlgorithmResult* result) {
    std::string report;
    report += "Representation: " + representation->name() + "\n";
    report += "Algorithm: " + algorithm->name() + "\n";

    report += "\n--- GRAPH ---\n\n";
    report += representation->toString() + "\n";
    report += "\n";

    report += "\n--- RESULT ---\n";
    // Print the MST tree here (TO DO)
    report += "\n";

    report += "Total cost: " + std::to_string(result->cost()) + "\n";
    return report;
}


/**
 * Builds the detailed part of the report for the SP problem
 * 
 * @param representation the graph representation on which the algorithm was run
 * @param algorithm the graph algorithm that was run
 * @param result the result of the graph algorithm
 * 
 * @returns the generated report string for the SP problem
 */
std::string ReportBuilder::buildSPReport(GraphRepr* representation, GraphAlgorithmBase* algorithm, GraphAlgorithmResult* result) {
    std::string report;
    report += "Representation: " + representation->name() + "\n";
    report += "Algorithm: " + algorithm->name() + "\n";

    report += "\n--- GRAPH ---\n\n";
    report += representation->toString() + "\n";

    report += "\n--- RESULT ---\n";
    // Print the path here (TO DO)
    report += "\n";

    report += "Total cost: " + std::to_string(result->cost()) + "\n";
    return report;
}


/**
 * Builds the detailed part of the report for the MF problem
 * 
 * @param representation the graph representation on which the algorithm was run
 * @param algorithm the graph algorithm that was run
 * @param result the result of the graph algorithm
 * 
 * @returns the generated report string for the MF problem
 */
std::string ReportBuilder::buildMFReport(GraphRepr* representation, GraphAlgorithmBase* algorithm, GraphAlgorithmResult* result) {
    std::string report;
    report += "Representation: " + representation->name() + "\n";
    report += "Algorithm: " + algorithm->name() + "\n";

    report += "\n--- GRAPH ---\n\n";
    report += representation->toString() + "\n";
    report += "\n";

    report += "\n--- RESULT ---\n";
    report += "Total flow: " + std::to_string(result->cost()) + "\n";
    return report;
}