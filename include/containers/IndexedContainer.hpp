#ifndef INDEXEDCONTAINER_HPP
#define INDEXEDCONTAINER_HPP

#include "BaseContainer.hpp"

template<typename T>
class IndexedContainer : public BaseContainer<T> {
    protected:
        IndexedContainer() = default;

    public:
        virtual ~IndexedContainer() = default;

        void insert(T element) {
            this->push(element);
        }
        void remove() {
            this->pop();
        }

        virtual T get(size_t index) = 0;
        virtual void set(size_t index, T element) = 0;
        virtual void removeAt(size_t index) = 0;
        virtual bool contains(T element) = 0;

};

#endif // INDEXEDCONTAINER_HPP