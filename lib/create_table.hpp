#ifndef create_table_H
#define create_table_H
#include "first_follow.hpp"
#include <map>
#include <string>
#include <vector>
#include <set>

const auto nabla = std::string("∇");

enum actions
{
    shift,
    reduce,
    to_bad
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
    all.insert(nabla);
    for (auto &good : all)
    {
        result[good] = {};
    }

    for (auto &rule : grammar)
    {
        for (size_t i = 0; i < rule.second.size() - 1; i++)
        {
            auto B = rule.second[i];

            std::set<std::string> a;
            if (first_follow_sets.first.find(rule.second[i + 1]) != first_follow_sets.first.end())
            {
                a = first_follow_sets.first[rule.second[i + 1]];
            }
            else
            {
                a = {rule.second[i + 1]};
            }
            for (auto &j : a)
            {
                result[B][j] = shift;
            }
        }
    }
    for (auto &i : first_follow_sets.first[grammar[0].first])
    {
        result[nabla][i] = shift;
    }
    for (auto &rule : grammar)
    {
        auto B = *(--rule.second.end());
        std::set<std::string> a = first_follow_sets.second[rule.first];
        for (auto &i : a)
        {
            if (result[B].find(i) != result[B].end() && (result[B][i] == shift || result[B][i] == to_bad))
            {
                result[B][i] = to_bad;
            }
            else
            {
                result[B][i] = reduce;
            }
        }
    }
    result[grammar[0].first][END_MARKER] = shift;
    return result;
}

#endif