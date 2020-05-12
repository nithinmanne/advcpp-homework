// homework2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <regex>

char constexpr file_name[] = "../hurdat_atlantic_1851-2011.txt";

char constexpr header_regex_seq[] = "\\d+\\s\\d{2}\\/\\d{2}\\/(\\d{4})\\sM\\=([\\d\\s]{1}\\d).+";
char constexpr dailydata_regex_seq[] = "[\\*SEWLD\\s][\\d\\s]{2}\\d[\\d\\s]{3}\\d([\\d\\s]{3}\\d)";

int main()
{
    std::ifstream file(file_name);
    
    std::regex header_regex(header_regex_seq);
    std::regex dailydata_regex(dailydata_regex_seq);

    std::smatch match;
    std::string line;
    int prev_year = -1;
    double sstotal = 0;
    while (std::getline(file, line)) {
        std::regex_match(line, match, header_regex);
        int year = std::stoi(match[1]);
        if (prev_year != year) {
            if (prev_year != -1)
                std::cout << prev_year << ": " << sstotal << std::endl;
            sstotal = 0;
            prev_year = year;
        }
        int count = std::stoi(match[2]);
        for (int i = 0; i < count; i++) {
            std::getline(file, line);
            auto dailydata_begin = std::sregex_iterator(line.begin(), line.end(), dailydata_regex);
            for (auto it = dailydata_begin; it != std::sregex_iterator(); it++) {
                sstotal += std::stoi((*it)[1])/4.;
            }
        }
        std::getline(file, line);
    }
    if (prev_year != -1)
        std::cout << prev_year << ": " << sstotal << std::endl;
}
