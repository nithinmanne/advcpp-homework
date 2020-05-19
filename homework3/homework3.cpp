/*
I'm using an implementation of B-Tree for C++ from https://github.com/Kronuz/cpp-btree
with a few minor changes to support MSVC.

There doesn't seem to be a significant difference in the performance and memory usage,
even when I'm trying to stress test the container. This is probably due to the fact that
there is no memory leak in this container implementation.

*/

// homework3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#define NO_GC

#include <iostream>
#ifndef NO_GC
#include <gc.h>
#endif

#ifndef NO_GC
void* operator new(std::size_t count) {
    return GC_MALLOC(count);
}
void operator delete(void*) {}
#endif

#include "set.h"

int main()
{
#ifndef NO_GC
    GC_INIT();
#endif
    btree::set<int> set;
    for (int i = 0; i < 16 * 1024 * 1024; i++) {
        set.insert(i % 16);
    }
}
