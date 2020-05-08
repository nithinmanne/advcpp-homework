// homework1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <ostream>
#include <iterator>
#include <vector>

#include <boost/iterator/function_output_iterator.hpp>

template<typename T, typename CharT, typename Traits, typename DelimCharT>
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
    DelimCharT const* delim;
    bool first = true;
public:
    _ostream_joiner_helper(std::basic_ostream<CharT, Traits>& stream, DelimCharT const* delim) :
        stream_ptr{ &stream }, delim{ delim } {}

    void operator()(T const& t) {
        if (first) first = false;
        else if(delim)  (*stream_ptr) << delim;
        (*stream_ptr) << t;
    }
};

template<typename T, typename CharT = char, typename Traits = std::char_traits<CharT>, typename DelimCharT = CharT>
auto ostream_joiner(std::basic_ostream<CharT, Traits>& stream, DelimCharT const* delim = nullptr) {
    return boost::function_output_iterator(_ostream_joiner_helper<T, CharT, Traits, DelimCharT>(stream, delim));
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
    std::copy(double_vec.begin(), double_vec.end(), ostream_joiner<double>(std::cout));
    std::wcout << std::endl << double_vec << std::endl;
}
