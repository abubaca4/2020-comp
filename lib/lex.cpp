#include "lex.hpp"

const char *lex::lex_types_text(lex_types number)
{
    static const std::map<lex_types, std::string> text_types{{equal, "equal"},
                                                         {delimeter, "delimeter"},
                                                         {relative_operators, "relative_operators"},
                                                         {keywords, "keywords"},
                                                         {ids, "ids"},
                                                         {broken_sequence, "broken_sequence"},
                                                         {integer, "integer"},
                                                         {real, "real"}};
    if (text_types.find(number) != text_types.end())
    {
        return text_types.find(number)->second.c_str();
    }
    else
    {
        return "not defined";
    }
}

void lex::load_special(const std::string &path, lex_types type_of_s)
{
    std::ifstream file(path);
    while (!file.eof())
    {
        std::string temp;
        getline(file, temp);
        for (auto &i : temp)
        {
            non_id_chars.insert(i);
        }
        symb[temp] = type_of_s;
        symb_len.insert(temp.length());
    }
    file.close();
}

void lex::load_kwords(const std::string &path)
{
    std::ifstream file(path);
    while (!file.eof())
    {
        std::string temp;
        getline(file, temp);
        kwords.insert(temp);
    }
    file.close();
}

std::vector<lex_record> lex::parse_file(const std::string &path)
{
    std::vector<lex_record> result;
    std::ifstream file(path);

    enum states
    {
        start,
        symbols,
        text
    };

    char temp;
    std::string lex_m{""};
    states state = start;
    size_t line = 0, symbol = 0;
    file.get(temp);
    do
    {
        switch (state)
        {
        case start:
            lex_m = "";
            if (!separator.count(temp))
            {
                if (non_id_chars.count(temp))
                {
                    state = symbols;
                }
                else
                {
                    state = text;
                }
            }
            else
            {
                if (temp == '\n')
                {
                    symbol = 0;
                    line++;
                }
                else
                {
                    symbol++;
                }
                file.get(temp);
            }
            break;

        case symbols:
            /*if (symb_len.count(lex_m.length()))
            {
                if (symb.find(lex_m) != symb.end())
                {
                    result.push_back({.type = symb[lex_m], .text = lex_m, .line = line, .symbol = symbol});
                    lex_m = "";
                    continue;
                }
            }*/
            if (non_id_chars.count(temp))
            {
                lex_m += temp;
                file.get(temp);
                symbol++;
            }
            else
            {
                state = start;
                if (lex_m.length() == 0)
                    continue;
                if (symb.find(lex_m) != symb.end())
                {
                    result.push_back({.type = symb[lex_m], .text = lex_m, .line = line, .symbol = symbol});
                }
                else
                {
                    result.push_back({.type = broken_sequence, .text = lex_m, .line = line, .symbol = symbol});
                }
            }
            break;

        case text:
            if (!separator.count(temp) && !non_id_chars.count(temp))
            {
                lex_m += temp;
                file.get(temp);
                symbol++;
            }
            else
            {
                state = start;
                if (kwords.count(lex_m))
                {
                    result.push_back({.type = keywords, .text = lex_m, .line = line, .symbol = symbol});
                }
                else
                {
                    result.push_back({.type = ids, .text = lex_m, .line = line, .symbol = symbol});
                }
            }
            break;
        }
    } while (!file.eof());
    file.close();
    for (auto &i : result)
    {
        i.symbol -= i.text.length();
        if (i.type == ids)
        {
            try
            {
                std::stoi(i.text);
                i.type = integer;
            }
            catch (std::invalid_argument)
            {
                try
                {
                    std::stof(i.text);
                    i.type = real;
                }
                catch (std::invalid_argument)
                {
                }
            }
            if (i.type == integer && i.text.find('.') != std::string::npos)
            {
                try
                {
                    std::stof(i.text);
                    i.type = real;
                }
                catch (std::invalid_argument)
                {
                }
            }
        }
    }
    return result;
}