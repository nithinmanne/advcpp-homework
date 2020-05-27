// homework1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
/*
Without Using less<>, it wasn't possible to find a string_view in the set, since
string_view can't be implicitly converted into string which is needed for the call
to less<string> which is the default compare operator. The results are:
set<string>.find(string): 5800
set<string>.find(char*): 6800
set<string, less<>>.find(string): 6000
set<string, less<>>.find(char*): 4400
set<string, less<>>.find(string_view): 5100

For string, The performance is mostly similar in both cases.
But for char*, less<> seems to perform better, even though
I expected it to be the opposite.

*/

#include <iostream>
#include <set>
#include <string>
#include <functional>
#include <fstream>
#include <chrono>
#include <string_view>

using set1 = std::set<std::string>;
using set2 = std::set<std::string, std::less<>>;

char constexpr FILE_NAME[] = "Oxford English Dictionary.txt";

template<typename SetType, typename StrType>
auto measure(SetType& set, StrType str) {
    auto start = std::chrono::high_resolution_clock::now();
    set.find(str);
    auto end = std::chrono::high_resolution_clock::now();
    return (end.time_since_epoch() - start.time_since_epoch()).count();
}

template<typename SetType, typename StrType>
auto best_measure(SetType& set, StrType str, int n = 1000) {
    auto best = measure(set, str);
    for (int i = 0; i < n; i++) {
        auto time = measure(set, str);
        if (time < best) best = time;
    }
    return best;
}

int main()
{
    std::ifstream dic_file{ FILE_NAME };
    set1 set1_words;
    set2 set2_words;
    std::string word;
    while (dic_file >> word) {
        set1_words.insert(word);
        set2_words.insert(word);
    }

    /* Randomly Chosen */
    std::string word1 = "Randomly";
    char const* word2 = "Randomly";
    std::string_view word3 = "Randomly";

    std::cout << "set<string>.find(string): " << best_measure(set1_words, word1) << std::endl;
    std::cout << "set<string>.find(char*): " << best_measure(set1_words, word2) << std::endl;
    // std::cout << "set<string>.find(string_view): " << best_measure(set1_words, word3) << std::endl;

    std::cout << "set<string, less<>>.find(string): " << best_measure(set2_words, word1) << std::endl;
    std::cout << "set<string, less<>>.find(char*): " << best_measure(set2_words, word2) << std::endl;
    std::cout << "set<string, less<>>.find(string_view): " << best_measure(set2_words, word3) << std::endl;

}
