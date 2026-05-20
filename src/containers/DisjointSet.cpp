#include "DisjointSet.hpp"

DisjointSet::DisjointSet(size_t size, bool initialize) : m_size(size) {
    m_parent = new size_t[m_size];
    m_rank = new size_t[m_size]();
    if (!initialize) return;

    for (size_t i = 0; i < m_size; i++) {
        m_parent[i] = i;
    }
}

DisjointSet::~DisjointSet() {
    delete[] m_parent;
    delete[] m_rank;
}

void DisjointSet::makeSet(size_t element) {
    if (element >= m_size) return;
    m_parent[element] = element;
    m_rank[element] = 0;
}

size_t DisjointSet::find(size_t element) {
    if (element >= m_size) return element;
    if (m_parent[element] != element) {
        m_parent[element] = find(m_parent[element]);
    }
    return m_parent[element];
}

void DisjointSet::unionSets(size_t element1, size_t element2) {
    size_t root1 = find(element1);
    size_t root2 = find(element2);

    if (root1 == root2) return;

    if (m_rank[root1] < m_rank[root2]) {
        m_parent[root1] = root2;
    } else if (m_rank[root1] > m_rank[root2]) {
        m_parent[root2] = root1;
    } else {
        m_parent[root2] = root1;
        m_rank[root1]++;
    }
}