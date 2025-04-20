//
//  main.cpp
//  Assignment 2
//
//  Created by hcheng on 2/9/24.
//

#include <iostream>
#include "food.h"
#include "LinkedList.hpp"
#include <vector>

int main(int argc, const char * argv[]) {

    List<int> list;
    const std::initializer_list<int> SAMPLE_INTS{ 3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5, 8, 9, 7, 9, 3 };
    int i = 0;
    for (auto it = SAMPLE_INTS.begin(); it != SAMPLE_INTS.end(); it++) {
        list.push_head(*it);
        std::cout << "list.size(): " << list.size() << std::endl;
    }

    std::cout << "Your to_string:\n" << list.to_string() << std::endl;
    // should be "{3, 9, 7, 9, 8, 5, 3, 5, 6, 2, 9, 5, 1, 4, 1, 3}"

    std::cout << "Contins test:" << list.contains(9) << std::endl;
    std::cout << "Contins test:" << list.contains(-1) << std::endl;

    std::cout << "Pop test:\n";
    i = SAMPLE_INTS.size();
    for (auto it = SAMPLE_INTS.begin(); it != SAMPLE_INTS.end(); it++) {
        list.pop_head();
        std::cout << "New size: " << list.size() << std::endl;
        std::cout << "New list: " << list.to_string() << std::endl;
    }

    // initialize a list by an array
    List<int> list2(SAMPLE_INTS);
    std::cout << list2.to_string() << std::endl;

    return 0;
}
