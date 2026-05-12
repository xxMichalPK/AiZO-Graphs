#ifndef SINGLEFILEMODE_HPP
#define SINGLEFILEMODE_HPP

#include "DynamicArray.hpp"
#include "GraphRepr.hpp"
#include "RunModeBase.hpp"

class SingleFileMode : public RunModeBase {
    public:
        SingleFileMode() = delete;

        static int run();
    
    private:
        static DynamicArray<GraphRepr*>* createRepresentations(size_t vertexCount, size_t edgeCount);
};

#endif // SINGLEFILEMODE_HPP