// homework1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <ostream>
#include <iterator>
#include <vector>

#include <boost/iterator/function_output_iterator.hpp>

template<typename T, typename CharT = char, typename Traits = std::char_traits<CharT>>
class _ostream_joiner_helper {
private:
    /* Storing Pointer to stream, since boost::function_output_iterator
       requires what we pass to it to be copy constructible/assignable,
       which is not possible since storing a reference would not keep it
       assignable, and storing a copy is not possible, since basic_ostream
       is not copyable. Note that, using references seems to actually work
       in G++ 9 in Linux, but MSVC complains about the implicitly deleted
       copy assignment. */
    std::basic_ostream<CharT, Traits>* stream_ptr;
    CharT const* delim;
    bool first = true;
public:
    _ostream_joiner_helper(std::basic_ostream<CharT, Traits>& stream, CharT const* delim) :
        stream_ptr{ &stream }, delim{ delim } {}

    void operator()(T const& t) {
        if (first) first = false;
        else  (*stream_ptr) << delim;
        (*stream_ptr) << t;
    }
};

template<typename T, typename CharT = char, typename Traits = std::char_traits<CharT>>
auto ostream_joiner(std::basic_ostream<CharT, Traits>& stream, CharT const* delim = "\0") {
    return boost::function_output_iterator(_ostream_joiner_helper<T>(stream, delim));
}

template<typename T, typename CharT = char, typename Traits = std::char_traits<CharT>>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& stream, std::vector<T> vector) {
    stream << "[ ";
    std::copy(vector.begin(), vector.end(), ostream_joiner<T>(stream, ", "));
    stream << " ]";
    return stream;
}

int main()
{
    std::vector<double> double_vec{ 1., 2.71, 3.1415, 4., 5., 6.66 };
    std::cout << double_vec << std::endl;
}
