// homework1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <type_traits>

template<int, typename = void> class Fib;

template<int i>
class Fib<i, std::enable_if_t<(i <= 0)>> {
public:
    static unsigned int constexpr value = 0;
};

template<>
class Fib<1> {
public:
    static unsigned int constexpr value = 1;
};

template<int i>
class Fib<i, std::enable_if_t<(i > 1)>> {
public:
    static unsigned int constexpr value = Fib<i - 1>::value + Fib<i - 2>::value;
};


int main()
{
    std::cout << "Fib<6>::value = " << Fib<6>::value << std::endl;
    static_assert(Fib<6>::value == 8);
    /* This static assert will fail if the value of Fib<6>::value wasn't available at compile time. */
}
