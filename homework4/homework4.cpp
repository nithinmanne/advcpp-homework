// homework4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <list>

template<typename T>
class MyVectorBin {
private:
    using container_t = std::vector<T>;
    using list_t = std::list<container_t>;
    list_t container_list;
public:
    container_t get() {
        if (container_list.empty()) return container_t();
        else {
            container_t container = std::move(container_list.back());
            container_list.pop_back();
            return container;
        }
    }
    void put(container_t&& container) { 
        container_list.emplace_back(std::move(container)); 
    }
};

template<typename T>
class MyVector {
private:
    using container_t = std::vector<T>;
    container_t container;
    static MyVectorBin<T> vectorBin;
public:
    MyVector() {
        /* Works Due to Copy Elision (Named Return Value Optimization) */
        container = vectorBin.get();
    }
    ~MyVector() {
        container.clear();
        vectorBin.put(std::move(container));
    }
    /* Add Other Operators as Needed */
    using size_type = std::size_t;
    T const* data() const { return container.data(); }
    size_type size() const noexcept { return container.size(); }
    size_type capacity() const noexcept { return container.capacity(); }
    void push_back(T const& value) { container.push_back(value); }
    void push_back(T&& value) { container.push_back(std::move(value)); }
};
template<typename T>
MyVectorBin<T> MyVector<T>::vectorBin;


int main()
{
    {
        MyVector<int> vec1;
        for (int i = 0; i < 100; i++) vec1.push_back(i);
        std::cout << "vec1 address: " << vec1.data() << std::endl;
    }
    {
        MyVector<int> vec2;
        std::cout << "vec2 address: " << vec2.data() << " "
            << "with capacity: " << vec2.capacity() << " "
            << "and size: " << vec2.size() << std::endl;
    }
}
