// homework1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstring>
#include <regex>

char constexpr input[] = "Here are some numbers: 1.23, 4, 5.6, 7.89";

int main()
{
    std::regex decimal_regex("(\\d+)\\.(\\d+)");

    auto decimal_begin = std::cregex_iterator(input, input + std::strlen(input), decimal_regex);

    for (auto it = decimal_begin; it != std::cregex_iterator(); it++) {
        std::cmatch decimal = *it;
        std::cout << decimal[1] << " is before the decimal and " << decimal[2] << " is after the decimal" << std::endl;
    }
}
