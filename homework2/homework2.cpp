/*
Running this code without using the Boehm Collector
had a devastating effect on my computer. The audio
playing in my browser started stutterring immediately,
slowly becoming worse, finally ending with the
program raising an std::bad_alloc, but not before
a few other processes including my browser crashed.
My display even shut off for a second, when I think
some process related to the GPU driver crashed.

On using the Boehm Collector, it ran till the end without
any issues, and never took more than 3072 KB as per
Task Manager.
*/

// homework2.cpp : This file contains the 'main' function. Program execution begins and ends there.
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
void operator delete(void*) {} // Not Even Called Here
#endif

struct Block { // 1 Kibibyte
    char data[1024];
};
struct Disk { // 1 Mebibyte
    Block blocks[1024];
};

int main()
{
#ifndef NO_GC
    GC_INIT();
#endif
    // Allocates 16 Gibibytes which is equal to the RAM my computer has
    for (int i = 0; i < 16 * 1024; i++) {
        new Disk;
    }
}
