// homework2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

std::string compute(int value) {
    return std::string(value, 'a');
}

class Computed {
private:
    int value;
    std::string computed;
public:
    Computed(int value) : value{ value }, computed{ compute(value) } {}

    /* Without these Lines, 'computed1' in the main function below would have an invalid state. */
    Computed(Computed const&) = default;
    Computed& operator=(Computed const&) = default;

    const std::string& getComputed() const {
        return computed;
    }
};


int main()
{
    Computed computed1{ 20 };
    Computed computed2 = computed1;
    Computed computed3 = std::move(computed1);

    std::cout << computed1.getComputed() << std::endl;
    std::cout << computed2.getComputed() << std::endl;
    std::cout << computed3.getComputed() << std::endl;
}
