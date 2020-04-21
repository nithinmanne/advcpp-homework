// homework2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

constexpr unsigned int fib(int i) {
    if (i <= 0) return 0;
    else if (i == 1) return 1;
    else return fib(i - 1) + fib(i - 2);
}


int main()
{
    std::cout << "fib(6) = " << fib(6) << std::endl;
    static_assert(fib(6) == 8);
    /* This static assert will fail if the value of fib(6) wasn't available at compile time. */
    int i;
    std::cout << "Enter i: ";
    std::cin >> i;
    std::cout << "fib(i) = " << fib(i) << std::endl;
    /* This second call to fib doesn't run at compile time, since its value is set during runtime
       and it would be impossible for the compiler to know what 'i' is, unless it can predict the
       future. I know compilers are very smart, but not enough to literally predict the future. */
}
