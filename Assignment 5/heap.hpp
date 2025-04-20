#pragma once
#include <math.h>

#include <iostream>
using namespace std;

#ifndef HEAPHPP
#define HEAPHPP

#define DEFAULTHEAPSIZE 1023

// Max Heap
template <class T>
class Heap {
 protected:
  T* _heap;
  int _size; // Tracks no. of elem, not capacity

 public:
     Heap() { _heap = new T[DEFAULTHEAPSIZE]; _size = 0; } // Array Implementation

  int size() const {
    // TODO: implement this
    return _size;
  }

  bool empty() const {
    // TODO: implement this
    return _size == 0;
  }
  void insert(const T&);
  T extractMax();
  T peekMax() const;
  void printHeapArray() const;
  void printTree() const;
  void changeKey(const T& from, const T& to);
  void deleteItem(const T&);

  ~Heap() { delete[] _heap; };

private:
    /*
    *   all the helper functions needed
    */

    // Search and return the index of an item
    int index(const T& item) {
        for (int i = 0; i < size(); i++) {
            if (_heap[i] == item) {
                return i;
            }
        }
        throw std::out_of_range("Item is no in heap");
    }

    /* 
     * Checks if an item is the leaf.
     * All items in index greater than size()/2 
     * will be in the lowest level i.e. is a leaf
    */ 
    bool isLeaf(int index) {
        return index >= _size/2;
    }

    void swap(T& a, T& b) {
        T temp = a;
        a = b;
        b = temp;
        return;
    }

    void bubbleUp(int currIndex) {
        int parentIndex = (currIndex - 1) / 2;
        while (currIndex > 0 && (_heap[parentIndex] < _heap[currIndex])) {
            swap(_heap[currIndex], _heap[parentIndex]);
            currIndex = parentIndex;
            parentIndex = (currIndex - 1) / 2;
        }
        return;
    }

    void bubbleDown(int currIndex) {
        while (!isLeaf(currIndex)) {
            // Still need to check if there is a left or right child
            int leftIndex = currIndex * 2 + 1;
            int rightIndex = currIndex * 2 + 2;
            int largestPrioIndex = currIndex;

            // check if left child exists and if it is larger than curr
            if (leftIndex < _size && _heap[leftIndex] > _heap[currIndex]) {
                largestPrioIndex = leftIndex;
            }

            // then check if right exists (only if left exists) and check if larger than left/curr, whichever bigger
            if (rightIndex < _size && _heap[rightIndex] > _heap[largestPrioIndex]) {
                largestPrioIndex = rightIndex;
            }

            // If curr is larger than/equal to both children == heap is done
            if (largestPrioIndex == currIndex) {
                return;
            }
            
            swap(_heap[currIndex], _heap[largestPrioIndex]);
            currIndex = largestPrioIndex;
        }
    }
};

template <class T>
void Heap<T>::insert(const T& item) {
  // TODO: implement this
  // Append at the end and bubbleUp to correct index
    if (_size == DEFAULTHEAPSIZE) {
        throw std::out_of_range("Heap is full");
    }
    _heap[_size] = item;
    bubbleUp(_size);
    _size++;
    return;
}

template <class T>
T Heap<T>::extractMax() {
  // TODO: implement this
    if (empty()) {
        throw std::out_of_range("Heap is empty");
    }
    T max = _heap[0];
    deleteItem(max);
    return max;
}

template <class T>
T Heap<T>::peekMax() const {
  // TODO: What happens if the heap is empty?
    if (empty()) {
        throw std::out_of_range("Heap is empty");
    }
  return _heap[0];
}

template <class T>
void Heap<T>::printHeapArray() const {
  for (int i = 0; i < size(); i++) {
    cout << _heap[i] << " ";
  }
  cout << endl;
}

template <class T>
void Heap<T>::changeKey(const T& from, const T& to) { 
  // TODO: implement this
    if (from == to) {
        return;
    }
    int currIndex = index(from);
    _heap[currIndex] = to;
    // increaseKey()
    if (from < to) {
        bubbleUp(currIndex);
    }
    // decreaseKey()
    else {
        bubbleDown(currIndex);
    }
    return;
}

template <class T>
void Heap<T>::deleteItem(const T& x) {
  // TODO: implement this
    if (empty()) {
        throw std::out_of_range("Heap is empty");
    }
    int currIndex = index(x);
    T lastPrio = _heap[_size - 1];
    swap(_heap[currIndex], _heap[_size - 1]);

    _size--; // deletes the node

    if (lastPrio > x) {
        bubbleUp(currIndex);
    }
    else {
        bubbleDown(currIndex);
    }
    
    return;
}

template <class T>
void Heap<T>::printTree() const {
  int parity = 0;
  if (size() == 0) return;
  int space = pow(2, 1 + (int)log2f(size())), i;
  int nLevel = (int)log2f(size()) + 1;
  int index = 0, endIndex;
  int tempIndex;

  for (int l = 0; l < nLevel; l++) {
    index = 1;
    parity = 0;
    for (i = 0; i < l; i++) index *= 2;
    endIndex = index * 2 - 1;
    index--;
    tempIndex = index;
    while (index < size() && index < endIndex) {
      for (i = 0; i < space - 1; i++) cout << " ";
      if (index == 0)
        cout << "|";
      else if (parity)
        cout << "\\";
      else
        cout << "/";
      parity = !parity;
      for (i = 0; i < space; i++) cout << " ";
      index++;
    }
    cout << endl;
    index = tempIndex;
    while (index < size() && index < endIndex) {
      for (i = 0; i < (space - 1 - ((int)log10(_heap[index]))); i++)
        cout << " ";
      cout << _heap[index];
      for (i = 0; i < space; i++) cout << " ";
      index++;
    }

    cout << endl;
    space /= 2;
  }
}

#endif