// homework2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

template<typename CharT = char, typename Traits = std::char_traits<CharT>>
class IndentStreamBuf : public std::basic_stringbuf<CharT, Traits> {
private:
    std::basic_ostream<CharT, Traits>& ostream;
    int indent_size;
    typename std::basic_string<CharT>::size_type indent_count = 0;
public:
    IndentStreamBuf(std::basic_ostream<CharT, Traits>& ostream, int indent_size = 4) : ostream(ostream), indent_size(indent_size) {}
    int overflow(int ch) override {
        CharT char_ch = static_cast<CharT>(ch);
        ostream << char_ch;
        if (char_ch == '\n')
            ostream << std::basic_string<CharT>(indent_count*indent_size, ' ');
        return ch;
    }
    void indent() { indent_count++; }
    void unindent() { indent_count--; }
};

template<typename CharT = char, typename Traits = std::char_traits<CharT>>
class IndentStream : public std::basic_ostream<CharT, Traits> {
    IndentStreamBuf<CharT, Traits> streambuf;
public:
    IndentStream(std::basic_ostream<CharT, Traits>& ostream, int indent_size = 4) : streambuf(ostream, indent_size), std::basic_ostream<CharT, Traits>(&streambuf) {}
};

template<typename CharT = char, typename Traits = std::char_traits<CharT>>
std::basic_ostream<CharT, Traits>& indent(std::basic_ostream<CharT, Traits>& os) {
    static_cast<IndentStreamBuf<CharT, Traits>*>(os.rdbuf())->indent();
    return os;
}
template<typename CharT = char, typename Traits = std::char_traits<CharT>>
std::basic_ostream<CharT, Traits>& unindent(std::basic_ostream<CharT, Traits>& os) {
    static_cast<IndentStreamBuf<CharT, Traits>*>(os.rdbuf())->unindent();
    return os;
}

int main()
{
    using std::cout;
    using std::endl;
    IndentStream ins(cout);
    ins << "int" << endl;
    ins << "fib(int n) {" << indent << endl;
    ins << "if (n == 0) " << indent << endl;
    ins << "return 0;" << unindent << endl;
    ins << "if (n == 1) " << indent << endl;
    ins << "return 1;" << unindent << endl;
    ins << "return fib(n-2) + fib(n-1);" << unindent << endl;
    ins << "}" << endl;
}
