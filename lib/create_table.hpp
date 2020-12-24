#ifndef create_table_H
#define create_table_H
#include <map>
#include <string>
#include <vector>
#include <set>

enum actions
{
    shift,
    reduce,
    refuse
};

std::map<std::string, std::map<std::string, actions>> make_table(std::vector<std::pair<std::string, std::vector<std::string>>> &grammar, std::pair<std::map<std::string, std::set<std::string>>, std::map<std::string, std::set<std::string>>> &first_follow_sets)
{
    std::map<std::string, std::map<std::string, actions>> result;

    std::set<std::string> non_terminals, all;
    for (auto &rule : grammar)
    {
        non_terminals.insert(rule.first);
        all.insert(rule.first);
        for (auto &symbol : rule.second)
        {
            all.insert(symbol);
        }
    }
    for (auto &rule : grammar)
    {
        std::string B = rule.second[0];
        }
}

#endif