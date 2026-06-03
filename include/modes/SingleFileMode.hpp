#ifndef SINGLEFILEMODE_HPP
#define SINGLEFILEMODE_HPP

#include "RunModeBase.hpp"

class SingleFileMode : public RunModeBase {
    public:
        SingleFileMode() = delete;

        static int run();
};

#endif // SINGLEFILEMODE_HPP