#ifndef STACK_HPP
#define STACK_HPP

#include "BaseContainer.hpp"

template <typename T>
class Stack : public BaseContainer<T> {
    private:
        T* m_data = nullptr;
        size_t m_capacity = 0;

    public:
        Stack(size_t initialCapacity);
        Stack() : Stack(10) {};
        ~Stack();

        virtual void push(T element) override;
        virtual void pop() override;
        virtual T getFront() override;
        virtual T getBack() override;
    
    private:
        void resize(size_t newSize);
};


/**
 * Creates a stack with the specified initial capacity
 */
template<typename T>
Stack<T>::Stack(size_t initialCapacity) : m_capacity(initialCapacity) {
    m_data = new T[m_capacity];
}


/**
 * Frees memory allocated for the stack
 */
template<typename T>
Stack<T>::~Stack() {
    delete[] m_data;
}


/**
 * Resizes the stack to the new capacity
 * 
 * @param newSize The new capacity of the stack
 */
template<typename T>
void Stack<T>::resize(size_t newSize) {
    if (newSize <= m_capacity) return;

    T* newData = new T[newSize];
    for (size_t i = 0; i < this->m_elementCount; i++) {
        newData[i] = m_data[i];
    }
    delete[] m_data;
    m_data = newData;
    m_capacity = newSize;
}


/**
 * Adds an element to the top of the stack
 * 
 * @param element The element to add
 */
template<typename T>
void Stack<T>::push(T element) {
    if (this->m_elementCount >= m_capacity) {
        size_t newCapacity = m_capacity == 0 ? 1 : m_capacity * 2;
        resize(newCapacity);
    }

    m_data[this->m_elementCount] = element;
    this->m_elementCount++;
}


/**
 * Removes the top element from the stack
 */
template<typename T>
void Stack<T>::pop() {
    if (this->isEmpty()) {
        throw std::out_of_range("Stack is empty");
    }

    this->m_elementCount--;
}


/**
 * Returns the top element of the stack without removing it
 * 
 * @returns The top element of the stack
 */
template<typename T>
T Stack<T>::getFront() {
    if (this->isEmpty()) {
        throw std::out_of_range("Stack is empty");
    }

    return m_data[this->m_elementCount - 1];
}


/**
 * Returns the bottom element of the stack without removing it
 * 
 * @returns The bottom element of the stack
 */
template<typename T>
T Stack<T>::getBack() {
    if (this->isEmpty()) {
        throw std::out_of_range("Stack is empty");
    }

    return m_data[0];
}

#endif // STACK_HPP