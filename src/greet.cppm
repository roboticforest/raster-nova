//
// Created by robot on 7/8/2025.
//

module;

#include <iostream>

export module greet;

export void say_hello();

void say_hello() {
    std::cout << "What's up!?" << std::endl;
}