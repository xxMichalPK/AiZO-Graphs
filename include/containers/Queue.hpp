#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "BaseContainer.hpp"
#include <stdexcept>

template<typename T>
class Queue : public BaseContainer<T> {
    private:
        // Implement the queue as a singly linked list
        // and hold both the head and tail
        struct QueueNode {
            T data;
            struct QueueNode* next;
        };
        QueueNode* m_head = nullptr;
        QueueNode* m_tail = nullptr;
        
    public:
        Queue() = default;
        ~Queue();

        virtual void push(T element) override;
        virtual void pop() override;
        virtual T getFront() override;
        virtual T getBack() override;

};

// Do the implementation here because last time there was too much jumping between the files

/**
 * The standard deconstructor that frees all the queue nodes
 */
template<typename T>
Queue<T>::~Queue() {
    QueueNode* current = m_head;
    // Free the memory by going through every list element
    while (current != nullptr) {
        QueueNode* next = current->next;
        delete current;
        current = next;
    }
}

/**
 * Adds the element to the end of the queue
 * 
 * @param element The element to add
 */
template<typename T>
void Queue<T>::push(T element) {
    // Create the new node
    QueueNode* newNode = new QueueNode{element, nullptr};

    // Increment the element count cause nothing should break below
    // (if it does there probably wouldn't be a way to prevent this)
    this->m_elementCount++;

    // Check if the queue is empty if yes, set the new element as both head and tail
    if (m_head == nullptr || m_tail == nullptr) {
        m_head = newNode;
        m_tail = newNode;
        return;
    }

    // Append the new node to the end of the queue
    m_tail->next = newNode;
    m_tail = newNode;
}

/**
 * Removes one element from the front of the queue
 */
template<typename T>
void Queue<T>::pop() {
    QueueNode* current = m_head;
    if (current == nullptr) return; // The queue is empty so just return

    // Set the new head and delete the previous one
    m_head = m_head->next;
    
    // If there is only one element (head == tail) set the tail to the new
    // head as well (it will be a nullptr)
    if (current == m_tail) m_tail = m_head;

    // Delete the first element
    delete current;

    // Decrement the element count
    this->m_elementCount--;
}

/**
 * Gets the data from the front of the queue (the first element)
 * 
 * @returns The first element in the queue
 */
template<typename T>
T Queue<T>::getFront() {
    if (m_head == nullptr) {
        throw std::out_of_range("The queue is empty!");
    }
    return m_head->data;
}

/**
 * Gets the data from the end of the queue (the last element)
 * 
 * @returns The last element in the queue
 */
template<typename T>
T Queue<T>::getBack() {
    if (m_tail == nullptr) {
        throw std::out_of_range("The queue is empty!");
    }
    return m_tail->data;
}

#endif // QUEUE_HPP