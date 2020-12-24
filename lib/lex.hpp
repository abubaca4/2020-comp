#ifndef LEX_H
#define LEX_H

#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "lex_type.h"

class lex
{
private:
    std::set<char> separator{' ', '\n'};
    std::set<char> non_id_chars;
    std::set<std::string> kwords;
    std::map<std::string, lex_types> symb;
    std::set<int> symb_len;

public:
    static const char *lex_types_text(lex_types number);
    void load_special(const std::string &path, lex_types type_of_s);
    void load_kwords(const std::string &path);
    std::vector<lex_record> parse_file(const std::string &path);
};

#endif