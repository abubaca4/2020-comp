#ifndef grammar_parser_H
#define grammar_parser_H

#include <fstream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <algorithm>

// trim from start (in place)
static inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
}

// trim from end (in place)
static inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
                return !std::isspace(ch);
            }).base(),
            s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s)
{
    ltrim(s);
    rtrim(s);
}

std::vector<std::string> split(const std::string &source, const std::string &delimiter)
{
    std::vector<std::string> temp;
    auto start = 0U;
    auto end = source.find(delimiter);
    while (end != std::string::npos)
    {
        temp.push_back(source.substr(start, end - start));
        start = end + delimiter.length();
        end = source.find(delimiter, start);
    }
    temp.push_back(source.substr(start, end));
    return temp;
}

std::vector<std::pair<std::string, std::vector<std::string>>> parse_grammar_file(const std::string &path)
{
    std::ifstream file(path);
    std::vector<std::pair<std::string, std::vector<std::string>>> rules;
    std::string firstNonterminal = "";

    while (!file.eof())
    {
        std::string line;
        std::getline(file, line);
        auto parts = split(line, " -> ");
        trim(parts[0]);
        if (firstNonterminal == "")
        {
            firstNonterminal = parts[0];
        }
        rules.push_back(std::make_pair(parts[0], split(parts[1], " ")));
    }
    return rules;
}

#endif