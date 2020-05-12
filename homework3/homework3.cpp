// homework3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <chrono>

#include "ctre.hpp"

char constexpr file_name[] = "../hurdat_atlantic_1851-2011.txt";

#define header_regex_seq "\\d+\\s\\d{2}/\\d{2}/(\\d{4})\\sM=([\\d\\s]{1}\\d).+"
#define dailydata_regex_seq "[\\*SEWLD\\s][\\d\\s]{2}\\d[\\d\\s]{3}\\d([\\d\\s]{3}\\d)"


void using_regex(bool);
void using_ctre(bool);

int main()
{
    using_regex(true);
    using_ctre(true);
}

void using_regex(bool no_print = false) {
    std::ifstream file(file_name);

    std::regex header_regex(header_regex_seq);
    std::regex dailydata_regex(dailydata_regex_seq);

    std::smatch match;
    std::string line;
    int prev_year = -1;
    double sstotal = 0;
    auto start = std::chrono::system_clock::now();
    while (std::getline(file, line)) {
        std::regex_match(line, match, header_regex);
        int year = std::stoi(match[1]);
        if (prev_year != year) {
            if (prev_year != -1)
                if (!no_print) std::cout << prev_year << ": " << sstotal << std::endl;
            sstotal = 0;
            prev_year = year;
        }
        int count = std::stoi(match[2]);
        for (int i = 0; i < count; i++) {
            std::getline(file, line);
            auto dailydata_begin = std::sregex_iterator(line.begin(), line.end(), dailydata_regex);
            for (auto it = dailydata_begin; it != std::sregex_iterator(); it++) {
                sstotal += std::stoi((*it)[1]) / 4.;
            }
        }
        std::getline(file, line);
    }
    std::cout << "using_regex: "
              << std::chrono::duration<double>(std::chrono::system_clock::now() - start).count()
              << " seconds" << std::endl;
    if (prev_year != -1)
        if (!no_print) std::cout << prev_year << ": " << sstotal << std::endl;
}

static constexpr auto header_regex_pattern = ctll::fixed_string{ header_regex_seq };
static constexpr auto dailydata_regex_pattern = ctll::fixed_string{ dailydata_regex_seq };

void using_ctre(bool no_print = false) {
    std::ifstream file(file_name);

    std::string line;
    int prev_year = -1;
    double sstotal = 0;
    auto start = std::chrono::system_clock::now();
    while (std::getline(file, line)) {
        auto match = ctre::match<header_regex_pattern>(line);
        int year = std::stoi(match.get<1>().str());
        if (prev_year != year) {
            if (prev_year != -1)
                if (!no_print) std::cout << prev_year << ": " << sstotal << std::endl;
            sstotal = 0;
            prev_year = year;
        }
        int count = std::stoi(match.get<2>().str());
        for (int i = 0; i < count; i++) {
            std::getline(file, line);
            auto dailydata_search = ctre::search<dailydata_regex_pattern>(line);
            while (dailydata_search) {
                sstotal += std::stoi(dailydata_search.get<1>().str()) / 4.;
                line = { dailydata_search.get_end_position(), line.end() };
                dailydata_search = ctre::search<dailydata_regex_pattern>(line);
            }
        }
        std::getline(file, line);
    }
    std::cout << "using_ctre: "
              << std::chrono::duration<double>(std::chrono::system_clock::now() - start).count()
              << " seconds" << std::endl;
    if (prev_year != -1)
        if (!no_print) std::cout << prev_year << ": " << sstotal << std::endl;
}