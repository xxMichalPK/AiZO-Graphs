#ifndef PRIORITYQUEUE_HPP
#define PRIORITYQUEUE_HPP

#include "BaseContainer.hpp"
#include <stdexcept>

/**
 * Priority queue implemented with a heap.
 * It uses the Floyd algorithm as it was described on our AiZO course
 * so no online reference here
 */
template<typename T>
class PriorityQueue : public BaseContainer<T> {
    private:
        // A priority queue is implemented using a heap so we need
        // a dynamic structure for holding the data
        T* m_data = nullptr;
        size_t m_memorySize = 0;
        
    public:
        PriorityQueue() : PriorityQueue(10) {};
        PriorityQueue(size_t initialSize);
        ~PriorityQueue();

        virtual void push(T element) override;
        virtual void pop() override;
        virtual T getFront() override;
        virtual T getBack() override;

    private:
        size_t leftChild(size_t i);
        size_t rightChild(size_t i);
        size_t parent(size_t i);

        void resize(size_t newSize);

        void swap(size_t idx1, size_t idx2);
        void heapifyUp(size_t nodeIdx);
        void heapifyDown(size_t nodeIdx);

};


/**
 * Initializes the priority queue with the predefined size
 * 
 * @param initialSize the initial size (capacity) of the priority queue
 */
template<typename T>
PriorityQueue<T>::PriorityQueue(size_t initialSize) : m_memorySize(initialSize) {
    m_data = new T[initialSize]();
}

/**
 * Clears the memory of the priority queue
 */
template<typename T>
PriorityQueue<T>::~PriorityQueue() {
    delete[] m_data;
}

/**
 * Returns the left child index of node at inex i
 * 
 * @returns left child index of node at index i
 */
template<typename T>
size_t PriorityQueue<T>::leftChild(size_t i) {
    return 2 * i + 1;
}

/**
 * Returns the right child index of node at inex i
 * 
 * @returns right child index of node at index i
 */
template<typename T>
size_t PriorityQueue<T>::rightChild(size_t i) {
    return 2 * i + 2;
}

/**
 * Returns the parent of node at inex i
 * 
 * @returns parent of node at index i
 */
template<typename T>
size_t PriorityQueue<T>::parent(size_t i) {
    // Handle the edge case of trying to find the parent of the root node
    if (i == 0) return 0;
    return static_cast<size_t>((i - 1) / 2);
}

/**
 * Swaps 2 elements in the priority queue table
 */
template<typename T>
void PriorityQueue<T>::swap(size_t idx1, size_t idx2) {
    T tmp = m_data[idx1];
    m_data[idx1] = m_data[idx2];
    m_data[idx2] = tmp;
}

/**
 * Resizes the priority queue array to the new specified size
 * 
 * @param newSize new size of the priority queue
 */
template<typename T>
void PriorityQueue<T>::resize(size_t newSize) {
    // If the new size is less than the previous one just return true and don't
    // bother with resizing it to a smaller size
    if (newSize <= m_memorySize) return;

    T* newData = new T[newSize]();
    for (size_t i = 0; i < m_memorySize; i++) {
        newData[i] = m_data[i];
    }

    delete[] m_data;
    m_memorySize = newSize;
    m_data = newData;
}

/**
 * Restores the min-heap property after inserting a new element into the priority queue
 * 
 * @param nodeIdx the index of the node to start the min-heapify from
 */
template<typename T>
void PriorityQueue<T>::heapifyUp(size_t nodeIdx) {
    // If we ar on the root node we can return
    if (nodeIdx == 0) return;
    
    size_t parentIdx = parent(nodeIdx);

    // Check if the parent is smaller than the current node if yes we can return
    if (m_data[parentIdx] <= m_data[nodeIdx]) return;

    // If not we need to swap and call minHeapify again for the parent
    swap(parentIdx, nodeIdx);
    heapifyUp(parentIdx);
}

/**
 * Restores the min-heap property after removing an element from the priority queue
 * 
 * @param nodeIdx the index of the node to start the min-heapify from
 */
template<typename T>
void PriorityQueue<T>::heapifyDown(size_t nodeIdx) {
    size_t leftChildIdx = leftChild(nodeIdx);
    size_t rightChildIdx = rightChild(nodeIdx);

    // If we are on a leaf node we can return
    if (leftChildIdx >= this->m_elementCount) return;

    // We need to find the smallest of the current node and its children
    size_t smallestIdx = nodeIdx;
    if (m_data[leftChildIdx] < m_data[smallestIdx]) {
        smallestIdx = leftChildIdx;
    }
    if (rightChildIdx < this->m_elementCount && m_data[rightChildIdx] < m_data[smallestIdx]) {
        smallestIdx = rightChildIdx;
    }

    // If the smallest is the current node we can return
    if (smallestIdx == nodeIdx) return;

    // If not we need to swap and call minHeapify again for the smallest
    swap(smallestIdx, nodeIdx);
    heapifyDown(smallestIdx);
}

/**
 * Adds the element to the priority queue
 * 
 * @param element The element to add
 */
template<typename T>
void PriorityQueue<T>::push(T element) {
    // Check if we have space to insert the new element
    if (this->m_elementCount >= m_memorySize) {
        // If not resize it to 2 times the size
        size_t newSize = m_memorySize == 0 ? 1 : m_memorySize * 2;
        resize(newSize);
    }

    // Put the element into the array
    m_data[this->m_elementCount] = element;
    this->m_elementCount++;

    // Now the fun part, we need to restore the heap property
    heapifyUp(this->m_elementCount - 1);
    // So that should be it, we'll see...
}

/**
 * Removes one element from the front of the priority queue
 */
template<typename T>
void PriorityQueue<T>::pop() {
    if (this->m_elementCount == 0) {
        throw std::out_of_range("The priority queue is empty");
    }

    this->m_elementCount--;
    // Swap the first and last element
    swap(0, this->m_elementCount);

    // Clear the element completely
    m_data[this->m_elementCount] = T();

    // Restore the heap property by calling heapifyDown from the root node
    heapifyDown(0);
}

/**
 * Gets the data from the front of the priority queue
 * 
 * @returns The first element in the priority queue
 */
template<typename T>
T PriorityQueue<T>::getFront() {
    if (this->m_elementCount == 0) {
        throw std::out_of_range("The priority queue is empty");
    }

    return m_data[0];
}

/**
 * Gets the data from the end of the queue
 * 
 * @returns The last element in the queue
 */
template<typename T>
T PriorityQueue<T>::getBack() {
    // So here we have a problem because the last element doesn't have to be the max one
    // but we can cheat by just looking for the max one

    // ATTENTION: This is only for the min-heap idk if I'll implement a max one but have that in mind
    T maxVal = m_data[0];
    for (size_t i = 1; i < this->m_elementCount; i++) {
        if (m_data[i] <= maxVal) continue;
        maxVal = m_data[i];
    }
    return maxVal;
}

#endif // PRIORITYQUEUE_HPP