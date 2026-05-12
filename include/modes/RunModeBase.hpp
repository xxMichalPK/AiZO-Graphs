#ifndef RUNMODEBASE_HPP
#define RUNMODEBASE_HPP

#include "Parameters.h"

class RunModeBase {
    public:
        RunModeBase() = delete;
        virtual ~RunModeBase() = default;

        /**
         * Tells you if the graph should be directed or not
         * 
         * @returns true if it should be directed, false otherwise
         */
        static bool isDirected() {
            return (Parameters::problem != Parameters::Problems::mst);
        }
};

#endif // RUNMODEBASE_HPP