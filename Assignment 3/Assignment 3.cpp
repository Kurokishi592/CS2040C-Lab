// Assignment 3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "BST.hpp"


int main()
{
    Tree<int> tree;
    tree.insert(2);
    tree.insert(4);
    tree.insert(1);
    tree.insert(0);
    tree.insert(3);
    std::cout << tree.to_string() << std::endl;
}
