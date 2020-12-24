#ifndef LEX_type_H
#define LEX_type_H
#include <string>

enum lex_types
    {
        equal,
        delimeter,
        relative_operators,
        keywords,
        ids,
        broken_sequence,
        integer,
        real
    };

struct lex_record
{
    lex_types type;
    std::string text;
    size_t line;
    size_t symbol;
};
#endif