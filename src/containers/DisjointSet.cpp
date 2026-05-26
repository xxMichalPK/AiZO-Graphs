#include "DisjointSet.hpp"


/**
 * Initializes a disjoint set with the specified size.
 * 
 * @param size The number of elements in the disjoint set.
 * @param initialize If true, initializes each element as its own set.
 */
DisjointSet::DisjointSet(size_t size, bool initialize) : m_size(size) {
    m_parent = new size_t[m_size];
    m_rank = new size_t[m_size]();
    if (!initialize) return;

    for (size_t i = 0; i < m_size; i++) {
        m_parent[i] = i;
    }
}


/**
 * Frees the memory allocated for the disjoint set.
 */
DisjointSet::~DisjointSet() {
    delete[] m_parent;
    delete[] m_rank;
}


/**
 * Manually initializes a single element as its own set.
 * 
 * @param element The index of the element to initialize.
 */
void DisjointSet::makeSet(size_t element) {
    if (element >= m_size) return;
    m_parent[element] = element;
    m_rank[element] = 0;
}

/**
 * Finds the root of the set that the specified element belongs to.
 * 
 * @param element The index of the element for which to find the root.
 * 
 * @returns The index of the root of the set.
 */
size_t DisjointSet::find(size_t element) {
    if (element >= m_size) return element;
    if (m_parent[element] != element) {
        m_parent[element] = find(m_parent[element]);
    }
    return m_parent[element];
}


/**
 * Unites the sets that contain the specified elements.
 * 
 * @param element1 The index of the first element.
 * @param element2 The index of the second element.
 */
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