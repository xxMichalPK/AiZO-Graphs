#ifndef BASECONTAINER_HPP
#define BASECONTAINER_HPP

#include <cstdint>
#include <cstddef>

template<typename T>
class BaseContainer {
    protected:
        size_t m_elementCount = 0;

    protected:
        BaseContainer() = default;

    public:
        virtual ~BaseContainer() = default;

        virtual void push(T element) = 0;
        virtual void pop() = 0;
        virtual T getFront() = 0;
        virtual T getBack() = 0;

        virtual size_t size() const {
            return m_elementCount;
        }
        virtual bool isEmpty() const {
            return size() == 0;
        }

};

#endif // BASECONTAINER_HPP