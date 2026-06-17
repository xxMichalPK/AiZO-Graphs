#include "ReportBuilder.hpp"

#include "Parameters.h"
#include "RunModeBase.hpp"

#include "Logger.hpp"

// Lets say that 500k vertices is the limit for printing the graph
// lets be real... who's gonna read it anyway?
#define PRINT_GRAPH_SIZE_LIMIT 500 * 1000

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
                                       GraphAlgorithmBase* algorithm, GraphAlgorithmResult* result, size_t durationMicroseconds) {

    std::string reportHeader = buildReportHeader(reportIndex, representation, durationMicroseconds);

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

    return reportHeader + detailedReport + "\n\n";
}


/**
 * Builds the header of the report with general information about the graph
 * 
 * @param reportIndex the index of the report
 * @param representation the graph representation on which the algorithm was run
 */
std::string ReportBuilder::buildReportHeader(size_t reportIndex, GraphRepr* representation, size_t durationMicroseconds) {
    std::string header;
    header += std::string(40, '=') + "\n";
    header += "Report #" + std::to_string(reportIndex) + "\n";
    header += std::string(40, '=') + "\n";
    header += "Execution time: " + std::to_string(durationMicroseconds) + "us\n";
    header += "Type: " + std::string(RunModeBase::isDirected() ? "Directed" : "Undirected") + "\n";
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
    report += "Total cost: " + std::to_string(result->cost()) + "\n";

    report += "\n--- GRAPH ---\n\n";
    if (representation->getVertexCount() <= PRINT_GRAPH_SIZE_LIMIT) {
        report += representation->toString();
    } else {
        report += "Graph is too large to display\n";
    }
    report += "\n";

    report += "\n--- MST TREE ---\n\n";
    // Don't print the tree if it's too large because it takes a lot of time and disk space
    if (representation->getVertexCount() <= PRINT_GRAPH_SIZE_LIMIT) {
        report += result->fullResultString() + "\n";
    } else {
        report += "MST tree is too large to display\n";
    }
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
    report += "Start vertex: " + std::to_string(Parameters::vertexStart) + "\n";
    report += "End vertex: " + std::to_string(Parameters::vertexEnd) + "\n";
    report += "Total cost: " + std::to_string(result->cost()) + "\n";

    report += "\n--- GRAPH ---\n\n";
    if (representation->getVertexCount() <= PRINT_GRAPH_SIZE_LIMIT) {
        report += representation->toString();
    } else {
        report += "Graph is too large to display\n";
    }
    report += "\n";

    report += "\n--- SHORTEST PATH ---\n\n";
    // Don't print the path if it's too large
    if (representation->getVertexCount() <= PRINT_GRAPH_SIZE_LIMIT) {
        report += result->fullResultString() + "\n";
    } else {
        report += "Shortest path is too large to display\n";
    }
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
    report += "Source vertex: " + std::to_string(Parameters::vertexStart) + "\n";
    report += "Sink vertex: " + std::to_string(Parameters::vertexEnd) + "\n";
    report += "Total flow: " + std::to_string(result->cost()) + "\n";

    report += "\n--- GRAPH ---\n\n";
    if (representation->getVertexCount() <= PRINT_GRAPH_SIZE_LIMIT) {
        report += representation->toString();
    } else {
        report += "Graph is too large to display\n";
    }
    report += "\n";

    return report;
}