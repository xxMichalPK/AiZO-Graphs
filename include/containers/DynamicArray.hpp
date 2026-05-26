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
        DynamicArray(size_t initialSize, T defaultValue);
        DynamicArray() : DynamicArray(10) {}
        ~DynamicArray();

        virtual void push(T element) override;
        virtual void pop() override;
        virtual T getFront() override;
        virtual T getBack() override;
        virtual T get(size_t index) override;
        virtual const T get(size_t index) const override;
        virtual void set(size_t index, T element) override;
        virtual void removeAt(size_t index) override;
        virtual bool contains(T element) override;
    
    protected:
        void resize(size_t newSize);

};


/**
 * Constructs a DynamicArray with the specified initial size
 * 
 * @param initialSize the initial size of the dynamic array
 */
template<typename T>
DynamicArray<T>::DynamicArray(size_t initialSize) : m_currentSize(initialSize) {
    m_data = new T[m_currentSize]();
}


/**
 * Constructs a DynamicArray with the specified initial size and default value
 * 
 * @param initialSize the initial size of the dynamic array
 * @param defaultValue the default value to initialize the elements of the array with
 */
template<typename T>
DynamicArray<T>::DynamicArray(size_t initialSize, T defaultValue) : m_currentSize(initialSize) {
    m_data = new T[m_currentSize]();
    for (size_t i = 0; i < m_currentSize; i++) {
        m_data[i] = defaultValue;
    }
    this->m_elementCount = m_currentSize;
}


/**
 * Frees the memory allocated for the dynamic array
 */
template<typename T>
DynamicArray<T>::~DynamicArray() {
    delete[] m_data;
}


/**
 * Resizes the dynamic array to the new size if the new size is greater than the current size
 * 
 * @param newSize the new size to resize the array to
 */
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


/**
 * Adds an element to the end of the dynamic array
 * 
 * @param element the element to add
 */
template<typename T>
void DynamicArray<T>::push(T element) {
    if (this->m_elementCount >= m_currentSize) {
        resize(m_currentSize * 2);
    }

    m_data[this->m_elementCount] = element;
    this->m_elementCount++;
}


/**
 * Removes the last element from the dynamic array
 */
template<typename T>
void DynamicArray<T>::pop() {
    if (this->m_elementCount == 0) {
        throw std::out_of_range("The array is empty!");
    }

    this->m_elementCount--;
}


/**
 * Gets the first element of the dynamic array
 * 
 * @return the first element of the dynamic array
 */
template<typename T>
T DynamicArray<T>::getFront() {
    if (this->m_elementCount == 0) {
        throw std::out_of_range("The array is empty!");
    }
    
    return m_data[0];
}


/**
 * Gets the last element of the dynamic array
 * 
 * @return the last element of the dynamic array
 */
template<typename T>
T DynamicArray<T>::getBack() {
    if (this->m_elementCount == 0) {
        throw std::out_of_range("The array is empty!");
    }
    
    return m_data[this->m_elementCount - 1];
}


/**
 * Gets the element at the specified index
 * 
 * @param index the index of the element to get
 * 
 * @return the element at the specified index
 */
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


/**
 * Gets the element at the specified index
 * 
 * @param index the index of the element to get
 * 
 * @return the element at the specified index
 */
template<typename T>
const T DynamicArray<T>::get(size_t index) const {
    if (this->m_elementCount == 0) {
        throw std::out_of_range("The array is empty!");
    }

    if (index >= this->m_elementCount) {
        throw std::out_of_range("Index out of bounds!");
    }

    return m_data[index];
}


/**
 * Sets the element at the specified index
 * 
 * @param index the index of the element to set
 * 
 * @param element the value to set the element to
 */
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


/**
 * Removes the element at the specified index and shifts the remaining elements to the left
 * 
 * @param index the index of the element to remove
 */
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


/**
 * Checks if the dynamic array contains the specified element
 * 
 * @param element the element to check for
 * 
 * @return true if the element is found in the array, false otherwise
 */
template<typename T>
bool DynamicArray<T>::contains(T element) {
    for (size_t i = 0; i < this->m_elementCount; i++) {
        if (m_data[i] == element) return true;
    }
    return false;
}

#endif // DYNAMICARRAY_HPP