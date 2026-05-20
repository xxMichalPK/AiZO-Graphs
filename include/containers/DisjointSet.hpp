#ifndef DISJOINTSET_HPP
#define DISJOINTSET_HPP

#include <cstddef>

class DisjointSet {
    private:
        size_t* m_parent = nullptr;
        size_t* m_rank = nullptr;
        size_t m_size = 0;

    public:
        DisjointSet(size_t size, bool initialize = true);
        ~DisjointSet();

        void makeSet(size_t element);
        size_t find(size_t element);
        void unionSets(size_t element1, size_t element2);
};

#endif // DISJOINTSET_HPP