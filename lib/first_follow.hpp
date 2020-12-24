#ifndef first_follow_H
#define first_follow_H

#include <string>
#include <map>
#include <set>
#include <vector>

const auto EMPTY_CHAIN = std::string("null");
const auto END_MARKER = std::string("-|");

template <typename T>
std::set<T> getUnion(const std::set<T> &a, const std::set<T> &b)
{
    std::set<T> result = a;
    result.insert(b.begin(), b.end());
    return result;
}

bool isNonterminal(std::map<std::string, std::set<std::string>> &firstSets, const std::string &item)
{
    return (firstSets.find(item) != firstSets.end());
}

std::set<std::string> collectSet(std::map<std::string, std::set<std::string>> &firstSets, const std::set<std::string> &initialSet, const std::vector<std::string> &items, const std::set<std::string> &additionalSet)
{
    auto set = initialSet;
    bool should_brake;

    for (size_t index = 0; index < items.size(); index++)
    {
        should_brake = true;
        auto &item = items[index];
        if (isNonterminal(firstSets, item))
        {
            auto copy_firstSets = firstSets[item];
            while (copy_firstSets.find(EMPTY_CHAIN) != copy_firstSets.end())
            {
                copy_firstSets.erase(copy_firstSets.find(EMPTY_CHAIN));
            }
            set = getUnion(set, copy_firstSets);
            if (firstSets[item].find(EMPTY_CHAIN) != firstSets[item].end())
            {
                if (items.size() != index + 1)
                {
                    should_brake = false;
                }
                set = getUnion(set, additionalSet);
            }
        }
        else
        {
            set = getUnion(set, std::set<std::string>{item});
        }
        if (should_brake)
        {
            break;
        }
    }
    return set;
}

std::map<std::string, std::set<std::string>> makeFirstSets(std::map<std::string, std::set<std::string>> &FirstSets, std::vector<std::pair<std::string, std::vector<std::string>>> &rules)
{
    bool isSetChanged;
    do
    {
        isSetChanged = false;
        for (auto &i : rules)
        {
            auto set = FirstSets[i.first];
            set = getUnion(set, collectSet(FirstSets, set, i.second, std::set<std::string>{EMPTY_CHAIN}));

            if (FirstSets[i.first].size() != set.size())
            {
                FirstSets[i.first] = set;
                isSetChanged = true;
            }
        }
    } while (isSetChanged);
    return FirstSets;
}

std::map<std::string, std::set<std::string>> makeFollowSets(std::map<std::string, std::set<std::string>> &firstSets, std::map<std::string, std::set<std::string>> &followSets, std::vector<std::pair<std::string, std::vector<std::string>>> &rules)
{
    followSets[rules[0].first].insert(END_MARKER);

    bool isSetChanged;

    do
    {
        isSetChanged = false;
        for (auto &i : rules)
        {
            for (size_t index = 0; index < i.second.size(); index++)
            {
                auto &item = i.second[index];
                if (!isNonterminal(firstSets, item))
                {
                    continue;
                }
                auto set = followSets[item];
                set = getUnion(set, index + 1 < i.second.size() ? collectSet(firstSets, set, std::vector<std::string>(i.second.begin() + index + 1, i.second.end()), followSets[i.first]) : followSets[i.first]);
                if (followSets[item].size() != set.size())
                {
                    followSets[item] = set;
                    isSetChanged = true;
                }
            }
        }
    } while (isSetChanged);
    return followSets;
}

std::pair<std::map<std::string, std::set<std::string>>, std::map<std::string, std::set<std::string>>> first_follow(std::vector<std::pair<std::string, std::vector<std::string>>> &rules)
{
    std::map<std::string, std::set<std::string>> firstSets;
    std::map<std::string, std::set<std::string>> followSets;

    for (auto &i : rules)
    {
        firstSets[i.first] = {};
        followSets[i.first] = {};
    }

    firstSets = makeFirstSets(firstSets, rules);
    followSets = makeFollowSets(firstSets, followSets, rules);
    return std::make_pair(firstSets, followSets);
}

#endif