#ifndef PAIR_HPP
#define PAIR_HPP

#include <iostream>

template<typename T1, typename T2>
class Pair {
    protected:
        T1 element1 {};
        T2 element2 {};

    public:
        Pair() = default;
        Pair(T1 e1, T2 e2) : element1(e1), element2(e2) {}

        T1 first() { return element1; }
        T2 second() { return element2; }

        bool operator<(Pair e2) { return element1 < e2.element1; }
        bool operator<=(Pair e2) { return element1 <= e2.element1; }
        bool operator>(Pair e2) { return element1 > e2.element1; }
        bool operator>=(Pair e2) { return element1 >= e2.element1; }
        bool operator==(Pair e2) { return element1 == e2.element1; }
};

template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const Pair<T1, T2>& p) { 
    os << "{" << p.first() << ", " << p.second() << "}";
    return os;
}

#endif // PAIR_HPP