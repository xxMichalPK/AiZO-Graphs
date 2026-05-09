#ifndef DYNAMICARRAY_HPP
#define DYNAMICARRAY_HPP

#include "IndexedContainer.hpp"
#include <stdexcept>

template<typename T>
class DynamicArray : public IndexedContainer<T> {
    private:
        T* m_data = nullptr;
        size_t m_currentSize = 0;

    public:
        DynamicArray(size_t initialSize);
        DynamicArray() : DynamicArray(10) {}
        ~DynamicArray();

        virtual void push(T element) override;
        virtual void pop() override;
        virtual T getFront() override;
        virtual T getBack() override;
        virtual T get(size_t index) override;
        virtual void set(size_t index, T element) override;
        virtual void removeAt(size_t index) override;
    
    protected:
        void resize(size_t newSize);

};

template<typename T>
DynamicArray<T>::DynamicArray(size_t initialSize) : m_currentSize(initialSize) {
    m_data = new T[m_currentSize]();
}

template<typename T>
DynamicArray<T>::~DynamicArray() {
    delete[] m_data;
}

template<typename T>
void DynamicArray<T>::resize(size_t newSize) {
    if (newSize <= m_currentSize) return;

    T* newData = new T[newSize]();
    for (size_t i = 0; i < m_currentSize; i++) {
        newData[i] = m_data[i];
    }

    delete[] m_data;

    m_currentSize = newSize;
    m_data = newData;
}

template<typename T>
void DynamicArray<T>::push(T element) {
    if (this->m_elementCount >= m_currentSize) {
        resize(m_currentSize * 2);
    }

    m_data[this->m_elementCount] = element;
    this->m_elementCount++;
}

template<typename T>
void DynamicArray<T>::pop() {
    if (this->m_elementCount == 0) {
        throw std::out_of_range("The array is empty!");
    }

    this->m_elementCount--;
}

template<typename T>
T DynamicArray<T>::getFront() {
    if (this->m_elementCount == 0) {
        throw std::out_of_range("The array is empty!");
    }
    
    return m_data[0];
}

template<typename T>
T DynamicArray<T>::getBack() {
    if (this->m_elementCount == 0) {
        throw std::out_of_range("The array is empty!");
    }
    
    return m_data[this->m_elementCount - 1];
}

template<typename T>
T DynamicArray<T>::get(size_t index) {
    if (this->m_elementCount == 0) {
        throw std::out_of_range("The array is empty!");
    }

    if (index >= this->m_elementCount) {
        throw std::out_of_range("Index out of bounds!");
    }

    return m_data[index];
}

template<typename T>
void DynamicArray<T>::set(size_t index, T element) {
    if (this->m_elementCount == 0) {
        throw std::out_of_range("The array is empty!");
    }

    if (index >= this->m_elementCount) {
        throw std::out_of_range("Index out of bounds!");
    }

    m_data[index] = element;
}

template<typename T>
void DynamicArray<T>::removeAt(size_t index) {
    if (this->m_elementCount == 0) {
        throw std::out_of_range("The array is empty!");
    }

    if (index >= this->m_elementCount) {
        throw std::out_of_range("Index out of bounds!");
    }

    for (size_t i = index; i < this->m_elementCount; i++) {
        m_data[i] = m_data[i + 1];
    }
    this->m_elementCount--;
}

#endif // DYNAMICARRAY_HPP