#ifndef PAIR_HPP
#define PAIR_HPP

#include <iostream>

template<typename T1, typename T2>
class Pair {
    protected:
        T1 m_first {};
        T2 m_second {};

    public:
        Pair() = default;
        Pair(T1 first, T2 second) : m_first(first), m_second(second) {}

        T1 first() const { return m_first; }
        T2 second() const { return m_second; }

        bool operator<(Pair otherPair) { return m_first < otherPair.m_first; }
        bool operator<=(Pair otherPair) { return m_first <= otherPair.m_first; }
        bool operator>(Pair otherPair) { return m_first > otherPair.m_first; }
        bool operator>=(Pair otherPair) { return m_first >= otherPair.m_first; }
        bool operator==(Pair otherPair) { return m_first == otherPair.m_first; }
};

template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const Pair<T1, T2>& p) { 
    os << "{" << p.first() << ", " << p.second() << "}";
    return os;
}

#endif // PAIR_HPP