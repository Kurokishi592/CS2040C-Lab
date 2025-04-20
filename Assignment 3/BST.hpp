#pragma once
#ifndef TREE_H
#define TREE_H

#include <algorithm>
#include <cassert>
#include <functional>
#include <string>

using std::string;

template <typename T>
std::string my_to_string(const T& t) {
  return std::to_string(t);
}

template <>
std::string my_to_string(const std::string& t) {
  return "\"" + t + "\"";
}

template <typename T> struct Node {
  T element;
  int height;
  Node<T> *left;
  Node<T> *right;

  Node(T element)
      : element{element}, height{0}, left{nullptr}, right{nullptr} {}
  Node(T element, int height)
      : element{element}, height{height}, left{nullptr}, right{nullptr} {}
};

template <typename T> class Tree {
private:
  size_t m_size;
  Node<T> *m_root;

public:
  // Constructor
  Tree();

  // Rule of three:
  // If a class requires a user-defined destructor, a user-defined copy
  // constructor, or a user-defined copy assignment operator, it almost
  // certainly requires all three.

  // Destructor
  ~Tree();

  // Returns a pointer to the root
  Node<T> *root();

  // Checks whether the tree is empty
  bool empty() const;

  // Returns the number of elements
  size_t size() const;

  // Returns the height of the tree
  int height() const;

  // Inserts the specified element
  void insert(T element);

  // Checks whether the container contains the specified element
  bool contains(T element) const;

  // Returns the maximum element
  T max() const;

  // Returns the minimum element
  T min() const;

  // Returns the successor of the specified element
  T successor(T element);

  // Convert each element in the tree to string in pre-order.
  string pre_order();

  // Convert each element in the tree to string in order.
  string in_order();

  // Convert each element in the tree to string in post-order.
  string post_order();

  // Returns a string equivalent of the tree
  string to_string(bool with_height = true) const {
    return m_to_string(with_height, m_root, 0);
  }

private:
  string m_to_string(bool with_height, Node<T> *node, int ident) const {
    string res;
    if (node == nullptr) {
      return res;
    }
    if (node->right != nullptr) {
      res += m_to_string(with_height, node->right, ident + 2);
    }
    for (int i = 0; i < ident; i++) {
      res += " ";
    }
    res += my_to_string(node->element);
    if (with_height) {
      res += "(h=" + my_to_string(node->height) + ")";
    }
    res += "\n";
    if (node->left != nullptr) {
      res += m_to_string(with_height, node->left, ident + 2);
    }
    return res;
  }

  // Feel free to declare helper functions here, if necessary

  // Use post-order-traversal to delete children first then node itself.
  void postClear(Node<T>* node) {
      if (!node) return;
      postClear(node->left);
      postClear(node->right);
      delete node;
  }

  // Calculate height at a given node
  int height(Node<T>* node) const {
      if (!node) return -1; // height of empty subtree, handles empty left/right child
      int leftHeight = height(node->left);
      int rightHeight = height(node->right);
      return std::max(leftHeight, rightHeight) + 1;
  }

  // Helper to perform right-rotate on current node with left child, and update height
  Node<T>* rightRotate(Node<T>* current) {
      Node<T>* child = current->left; // temp pointer
      current->left = child->right; // as current becomes child, it must inherit the child's right subtree
      child->right = current; // current is now the right child of new current

      current->height = std::max(height(current->left), height(current->right)) + 1;
      child->height = std::max(height(child->left), height(child->right)) + 1;

      return child; // Return the new root to update the parent's pointer
  }

  // Helper to perform left-rotate on current node with right child, and update height
  Node<T>* leftRotate(Node<T>* current) {
      Node<T>* child = current->right; // temp pointer
      current->right = child->left; // as current becomes child, it must inherit the child's right subtree
      child->left = current; // current is now the right child of new current

      current->height = std::max(height(current->left), height(current->right)) + 1;
      child->height = std::max(height(child->left), height(child->right)) + 1;

      return child; // Return the new root to update the parent's pointer
  }

  // Insert an element and balance to maintain AVL Tree. Updates height too.
  Node<T>* insertAVL(Node<T>* current, T element) {
      //standard insert function (with updating parent node for rotation)
      if (!current) return new Node<T>(element);
      if (element < current->element) {
          current->left = insertAVL(current->left, element);
      }
      else if (element > current->element) {
          current->right = insertAVL(current->right, element);
      }
 
      int heightLeftSubtree = height(current->left);
      int heightRightSubtree = height(current->right);

      //update height of each node
      current->height = std::max(heightLeftSubtree, heightRightSubtree) + 1;
      
      //check for unbalance, rotate accordingly and return the updated root
      int heightDifference = heightLeftSubtree - heightRightSubtree;
      
      // Case 1: Left-Left -> A(curr) is left-heavy, B(left-child) is left heavy 
      // i.e. element is inserted into left subtree of left child
      if (heightDifference > 1 && element < current->left->element) {
          return rightRotate(current); 
      }

      // Case 2: Right-Right -> A(curr) is right heavy, B(right-child) is right heavy
      // i.e. element is inserted into right subtree of right child
      if (heightDifference < -1 && element > current->right->element) {
          return leftRotate(current);
      }
      
      // for A left/right heavy and B is balanced, they are handled in Case 1 and 2 alr

      // Case 3: Left-Right -> A(curr) is left heavy, B(left-child) is right heavy
      // i.e. element is inserted into right subtree of left child
      if (heightDifference > 1 && element > current->left->element) {
          // left-rotate(B) first
          current->left = leftRotate(current->left);
          // then right-rotate(A)
          return rightRotate(current);
      }

      // Case 4: Right-Left -> A(curr) is right heavy, B(right-child) is left heavy
      // i.e. element is inserted into left subtree of right child
      if (heightDifference < -1 && element < current->right->element) {
          // right-rotate(B) first
          current->right = rightRotate(current->right);
          // then left-rotate(A)
          return leftRotate(current);
      }

      // if the tree is not unbalanced
      return current;
  }

  // Finds a successor of element from the current node
  Node<T>* findSuccessor(Node<T>* current, T element) {
      if (!current) return nullptr;

      //if current's value > x, this node is a potential successor
      if (current->element > element) {
          //search leftsubtree
          Node<T>* leftSuccessor = findSuccessor(current->left, element);
          //return leftsuccessor if found, otherwise this node is the successor
          return leftSuccessor ? leftSuccessor : current;
      }

      //if current <= x, continue to search the rightsubtree
      return findSuccessor(current->right, element);
  }

  // Helper to print in-order-traversal
  string _in_order(Node<T>* node) {
      return (node->left == nullptr ? "" : _in_order(node->left) + " ")
          + my_to_string(node->element)
          + (node->right == nullptr ? "" : " " + _in_order(node->right));
  }

  // Helper to print post-order-traversal
  string _post_order(Node<T>* node) {
      return (node->left == nullptr ? "" : _post_order(node->left) + " ")
          + (node->right == nullptr ? "" : _post_order(node->right) + " ")
          + my_to_string(node->element);
  }

  };

// Constructor
template <typename T> Tree<T>::Tree() {
    m_root = nullptr;
  // TODO: Implement this method
    m_size = 0;
}

// Destructor
template <typename T> Tree<T>::~Tree() {
  // TODO: Implement this method
    postClear(m_root);
}

// Returns a pointer to the root
template <typename T> Node<T> *Tree<T>::root() {
  // TODO: Implement this method
  return m_root;
}

// Checks whether the tree is empty
template <typename T> bool Tree<T>::empty() const {
  // TODO: Implement this method
  return m_root == nullptr;
}

// Returns the number of elements
template <typename T> size_t Tree<T>::size() const {
  // TODO: Implement this method
  return m_size;
}

// Returns the height of the tree
template <typename T> int Tree<T>::height() const {
  // TODO: Implement this method
    return height(m_root);
}

// Inserts an element
template <typename T> void Tree<T>::insert(T element) {
  // TODO: Implement this method
    m_root = insertAVL(m_root, element); // start inserting from the root
    m_size += 1;
}

// Checks whether the container contains the specified element
template <typename T> bool Tree<T>::contains(T element) const {
  // TODO: Implement this method
    Node<T>* current = m_root;
    while (current) {
        if (current->element == element) return true;
        else if (element > current->element) {
            current = current->right;
        }
        else {
            current = current->left;
        }
    }
    return false;
}

// Returns the maximum element
template <typename T> T Tree<T>::max() const {
  // TODO: Implement this method
    if (empty()) {
        throw std::out_of_range("Tree is empty");
    }
    Node<T>* current = m_root;
    while (current->right) {
        current = current->right;
    }
    return current->element;
}

// Returns the minimum element
template <typename T> T Tree<T>::min() const {
  // TODO: Implement this method
    if (empty()) {
        throw std::out_of_range("Tree is empty");
    }
    Node<T>* current = m_root;
    while (current->left) {
        current = current->left;
    }
    return current->element;
}

// Returns the successor of the specified element
template <typename T> T Tree<T>::successor(T element) {
  // TODO: Implement this method
    Node<T>* successorNode = findSuccessor(m_root, element);
    if (!successorNode) {
        throw std::out_of_range("There is no successor");
    }
    return successorNode->element;
}

template <typename T>
string _pre_order(Node<T> *node) {
  return my_to_string(node->element)
    + (node->left == nullptr ? "" : " " + _pre_order(node->left))
    + (node->right == nullptr ? "" : " " + _pre_order(node->right));
}

template <typename T>
string Tree<T>::pre_order() {
  if (m_root == nullptr) {
    return "";
  }
  return _pre_order(m_root);
}

template <typename T>
string Tree<T>::in_order() {
  // TODO: Implement this method
    if (m_root == nullptr) {
        return "";
    }
    return _in_order(m_root);
}

template <typename T>
string Tree<T>::post_order() {
  // TODO: Implement this method
    if (m_root == nullptr) {
        return "";
    }
    return _post_order(m_root);
}

#endif
