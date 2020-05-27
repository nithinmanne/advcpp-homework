// homework2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/*
Without std::memory_order_relaxed: Time: 0.128604
With std::memory_order_relaxed: Time: 0.116952

The performance in both cases seems to be exactly similar, this
is maybe due to the fact that the majority of time is taken on
the compare and exchange part rather than the first load.

*/

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

#include "LockFreeStack.h"

void thread(mpcs51044::Stack& stack) {
    for (int i = 0; i < 10'000; i++) stack.push(1);
    for (int i = 0; i < 10'000; i++)
        if (stack.pop() != 1)
            std::cerr << "Stack Data Integrity Error" << std::endl;
}

int main() {
    mpcs51044::Stack stack;
    std::vector<std::thread> tv;
    tv.reserve(10);
    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < 10; i++) tv.emplace_back(thread, std::ref(stack));
    for (int i = 0; i < 10; i++) tv[i].join();
    std::cout << "Time: " << std::chrono::duration<double>(std::chrono::system_clock::now() - start).count() << std::endl;
}
