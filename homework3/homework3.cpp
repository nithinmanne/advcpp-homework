// homework3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <list>

using std::vector;
using std::list;

template<typename T>vector<T> f(T t) { return vector<T>{7}; }
// This function seems to be creating a vector of type T, and length 7.
// However, the {} can be misinterpreted as an initializer list.
template<typename T>vector<T> f_fixed(T t) { return vector<T>(7); }


auto v1 = f(7); 
// This usage fails, since it assumes that {7} is an initalizer list.
// This is a int vector of size 1 with just '7' in it.
// This function can be fixed by using () instead of {}
auto v1_fixed = f_fixed(7);

auto v2 = f("foo");
// The above function works properly, and it creates a vector of size 7 and type char const*.

vector v3(v1.begin(), v1.end());
// This works correctly and uses the correct contructor to essentially copy v1 to v3.
// Since v1 is a vector with just '7', v3 is also the same.

vector v4{ v1.begin(), v1.end() };
// This usage might not be correct, since we would assume that it returns the same output as v3.
// However, since the type isn't given, the most logical assumption the compiler makes that, its
// an initalizer list of two iterators, and it creates such a vector.
// This can be fixed vby using it like v3.

vector v5{ v3 };
// This calls the copy constructor and v5 creates a vector<int> of size 1 with '7' inside it.
// However, checking this in GCC 7, with C++ 17, it actually calls the list initializer constructor
// and constructs a vector of vectors. This is due to competing constructors. Its better to avoid
// such situations when implementing any class.

list l1{ v3 };
// I think this is not the intended outcome, and if we want to copy the elements of vector to the list, we can use a different constructor.
list<int> l1_fixed(v3.begin(), v3.end());
// Some Best Practices:
// 1. Most of the above situations can be made more clear by explicitly specifying the type the container needs to hold.
// 2. Also, I think that its preferable to use curly braces for intializer list constructor only and normal brackets for other constructors.


int main()
{
    std::cout << "Hello World!\n";
}

/* Old Submission
template<typename T>
vector<T> f(T t) { return vector<T>{t}; }
auto v1 = f(7); // Creates a vector<int> of size 1 with '7' inside it.
auto v2 = f("foo"); // Creates a vector<char const*> of size 1 with "foo" inside it.

vector v3{ 4 }; // Creates a vector<int> of size 1 with '4' inside it.
// vector v4(4);
// This actually causes a deduction fail, since this '4' will match the constructor for creating
// a vector with size '4', but then it won't be able to deduce type of object to contain.
// Can be fixed by specifying type.
vector v5{ v3 };
// This calls the copy constructor and v5 creates a vector<int> of size 1 with '4' inside it.
// However, checking this in GCC 7, with C++ 17, it actually calls the list initializer constructor
// and constructs a vector of vectors. This is due to competing constructors. Its better to avoid
// such situations when implementing any class.
list l1{ v3 }; // Creates a list of size 1 that holds the vector v3.
// I think this is not the intended outcome, and if we want to copy the elements of vector to the list, we can use a different constructor.

// Some Best Practices:
// 1. All the above situations can be fixed by explicitly specifying the type the container needs to hold.
// 2. Also, I think that its preferable to use curly braces for intializer list constructor only.
vector<int> v3_fixed{ 4 }; // Creates vector with '4' in it.
vector<int> v4_fixed(4); // Creates a vector of size 4 with '0's.
vector<int> v5_fixed(v3); // Copy constructs from v3
list<int> l1_fixed(v3.begin(), v3.end());
*/