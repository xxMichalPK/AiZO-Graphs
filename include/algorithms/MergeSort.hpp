#ifndef MERGESORT_HPP
#define MERGESORT_HPP

#include "IndexedContainer.hpp"

template<typename T>
class MergeSort {
    public:
        MergeSort() = delete;
        static void sort(IndexedContainer<T>& container);
        static void sort(IndexedContainer<T>& container, size_t left, size_t right);
    
    private:
        static void merge(IndexedContainer<T>& container, size_t left, size_t mid, size_t right);
};

template<typename T>
void MergeSort<T>::sort(IndexedContainer<T>& container) {
    size_t size = container.size();
    if (size <= 1) return;

    size_t mid = size / 2;
    MergeSort<T>::sort(container, 0, mid);
    MergeSort<T>::sort(container, mid, size);
    MergeSort<T>::merge(container, 0, mid, size);
}

template<typename T>
void MergeSort<T>::sort(IndexedContainer<T>& container, size_t left, size_t right) {
    if (left >= right - 1) return;

    size_t mid = left + (right - left) / 2;
    MergeSort<T>::sort(container, left, mid);
    MergeSort<T>::sort(container, mid, right);
    MergeSort<T>::merge(container, left, mid, right);
}

template<typename T>
void MergeSort<T>::merge(IndexedContainer<T>& container, size_t left, size_t mid, size_t right) {
    size_t l = left;
    size_t m = mid;

    DynamicArray<T> temp(right - left);
    // Compare the two halves and merge them into temp
    while (l < mid && m < right) {
        if (container.get(l) <= container.get(m)) {
            temp.insert(container.get(l));
            l++;
        } else {
            temp.insert(container.get(m));
            m++;
        }
    }

    // Copy any remaining elements from the left half
    while (l < mid) {
        temp.insert(container.get(l));
        l++;
    }

    // and from the right half
    while (m < right) {
        temp.insert(container.get(m));
        m++;
    }

    // Move the sorted elements back into the original container
    for (size_t l = 0; l < temp.size(); l++) {
        container.set(left + l, temp.get(l));
    }
}

#endif // MERGESORT_HPP