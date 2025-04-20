
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <initializer_list>
#include <string>

template <typename T> struct Node {
  T element;
  Node<T> *next;

  Node(T element) : element{element}, next{nullptr} {}              //constructor for first
  Node(T element, Node<T> *next) : element{element}, next{next} {}  //constructor for linked (overload)
};

template <typename T> class List {
private:
  size_t m_size;
  Node<T> *m_head;

  // Feel free to add helper functions here, if necessary
 
  // Swap function (helper)
  void swap(List<T>& other) {
      std::swap(m_head, other.m_head);
      std::swap(m_size, other.m_size);
  }

  // Helper for to_string()
  template <typename U>
  std::string element_to_string(const U& element) const{
      return std::to_string(element);
  }
  // Overload for std::string to surround with quotes.
  std::string element_to_string(const std::string& element) const{
      return "\"" + element + "\"";
  }

public:
  // Constructs an empty container
  List() : m_size{0}, m_head{nullptr} {}

  // Constructs the container with the contents of the initializer list
  List(std::initializer_list<T> init_list) : m_size{0}, m_head{nullptr} {
    auto it = init_list.end();
    while (--it != init_list.begin()) {
      push_head(*it);
    }
    push_head(*it);
  }

  // Rule of three:
  // If a class requires a user-defined destructor, a user-defined copy
  // constructor, or a user-defined copy assignment operator, it almost
  // certainly requires all three.

  // Destructor
  ~List() {
    // TODO: (Optional) Implement this method
    //removeHead() but dont need m_size, terminates when current==nullptr
      Node<T>* current = m_head;
      while (current != nullptr) { 
          Node<T>* temp = current;
          current = current->next;
          delete temp;
      }
      m_head = nullptr;
      m_size = 0;
  }

  // Copy constructor (creates new object as a copy of `other` object) 
  List(const List<T> &other) {
    // TODO: (Optional) Implement this method
      m_head = nullptr;
      m_size = 0;
      Node<T>* currentOthernode = other.m_head;
      Node<T>* tail = nullptr; //track last node of new list

      //copy every node from other by iterating through its list
      while (currentOthernode != nullptr) {
          Node<T>* newNode = new Node<T>(currentOthernode->element);
          //if first node, set it to head
          if (empty()) {
              m_head = newNode;
              tail = newNode;
          }
          else { //append at tail
              tail->next = newNode;
              tail = newNode;
          }
          m_size++;
          currentOthernode = currentOthernode->next;
      }
  }

  // Copy assignment
  List<T> &operator=(const List<T> &other) {
    // TODO: (Optional) Implement this method
      if (this == &other) return *this; //check for self-assigning

      //just use the copy constructor, and swap
      List<T> temp(other);
      temp.swap(*this);

      //when temp goes out of scope, temp is destructored; cleans up old data too
      return *this; 
  }

  // Returns the contents of the head node
  const T& head() {
    if (m_size <= 0) {
      throw std::out_of_range("List is empty");
    }
    return m_head->element;
  }

  // Checks whether the container is empty
  bool empty() const { return m_size == 0; }

  // Returns the number of elements
  size_t size() const { return m_size; }

  // Inserts an element to the head
  void push_head(const T& element) {
    Node<T> *node = new Node<T>{element, m_head};
    m_head = node;
    m_size++;
  }

  // Removes the head element
  void pop_head() {
    if (m_size <= 0) {
      throw std::out_of_range("List is empty");
    }
    Node<T> *node = m_head;
    m_head = m_head->next;
    delete node;
    m_size--;
  }

  // Checks whether the container contains the specified element
  bool contains(const T& element) const {
    // TODO: Implement this method
      Node<T>* currentNode = m_head;
      while (currentNode != nullptr) {
          if (currentNode->element == element) {
              return true;
          }
          currentNode = currentNode->next;
      }
    return false;
  }

  // Extracts the maximum element from the container
  T extract_max() {
    // TODO: Implement this method
      if (empty()) {
          throw std::out_of_range("List is empty");		//exception for empty list
      }

      Node<T>* currentNode = m_head;
      Node<T>* prevNode = nullptr;
      Node<T>* maximumNode = currentNode; 
      Node<T>* beforeMaxNode = nullptr; //need to keep track to update ll

      while (currentNode != nullptr) {
          if (currentNode->element > maximumNode->element) { //I assume we take nearest from head for duplicate max
              maximumNode = currentNode;
              beforeMaxNode = prevNode;
          }
          prevNode = currentNode;
          currentNode = currentNode->next;
      }
      T max = maximumNode->element;

      //delete and return
      if (maximumNode == m_head) { //just in case max is in head
          pop_head();
      }
      else {
          beforeMaxNode->next = maximumNode->next;
          delete maximumNode;
          m_size--;
      }
      return max;
  }
  
  // Reverse the container
  void reverse() {
    // TODO: Implement this method
      if (empty()) {
          return;
      }
      
      //temp pointers for reversing
      Node<T>* prev = nullptr;
      Node<T>* curr = m_head;
      Node<T>* next = nullptr;

      while (curr != nullptr) {
          next = curr->next;
          curr->next = prev;
          prev = curr;
          curr = next;
      }
      m_head = prev;
  }

  // Returns a std::string equivalent of the container
  std::string to_string() const {
    // TODO: Implement this method
      if (empty()) {
          return "{}";
      }
      Node<T>* currentNode = m_head;
      std::string answer = "{";

      while (currentNode != nullptr) {
          answer += element_to_string(currentNode->element);   //use overloaded function
          if (currentNode->next != nullptr) {				
              answer += ", ";								
          }
          currentNode = currentNode->next;
      }
      answer += "}";
      return answer;
  }

};

#endif
