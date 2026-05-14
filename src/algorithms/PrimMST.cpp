#include "PrimMST.hpp"
#include "PriorityQueue.hpp"
#include "Pair.hpp"

int PrimMST::run() {
    PriorityQueue<Pair<intmax_t, size_t>> pq;

    pq.push({0, 0});
    pq.push({4, 0});
    pq.push({-20, 0});
    pq.push({-3, 0});

    size_t size = pq.size();
    for (size_t i = 0; i < size; i++) {
        auto e = pq.getFront();
        std::cout << e << "\n";
        pq.pop();
    }

    return 1;
}

GraphAlgorithmResult& PrimMST::result() {
    return m_result;
}