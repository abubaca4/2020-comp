#include "lib/lex.hpp"
#include "lib/grammar_parser.hpp"
#include "lib/first_follow.hpp"
#include "lib/create_table.hpp"
#include <stack>
#include <iostream>

void save_lex_resul_to_file(const std::string &path, const std::vector<lex_record> &result)
{
    std::ofstream file(path);
    file << "\"№\",\"type\",\"text\",\"line\",\"symbol\"" << std::endl;
    for (size_t i = 0; i < result.size(); i++)
    {
        file << "\"" << i << "\",\"" << lex::lex_types_text(result[i].type) << "\",\"" << result[i].text << "\",\"" << result[i].line + 1 << "\",\"" << result[i].symbol + 1 << "\"" << std::endl;
    }
    file.close();
}

std::vector<std::string> get_n_elements_stack(size_t n, std::stack<std::string> &in)
{
    std::vector<std::string> result;
    if (in.size() <= n)
    {
        n = in.size() - 1;
    }

    for (size_t i = 0; i < n; i++)
    {
        result.push_back(in.top());
        in.pop();
    }

    for (int i = result.size() - 1; i >= 0; i--)
    {
        in.push(result[i]);
    }
    return std::vector<std::string>(result.rbegin(), result.rend());
}

void delete_n_elements_stack(size_t n, std::stack<std::string> &in)
{
    if (in.size() <= n)
    {
        n = in.size() - 1;
    }

    for (size_t i = 0; i < n; i++)
    {
        in.pop();
    }
}

void cout_buff(std::stack<std::string> in)
{
    while (in.size() != 0)
    {
        std::cout << in.top() << " ";
        in.pop();
    }
    std::cout << std::endl;
}

bool comp_pair(const std::pair<std::string, std::vector<std::string>> &left, const std::pair<std::string, std::vector<std::string>> &right)
{
    return (left.second.size() > right.second.size());
}

int main()
{
    lex test_lex;
    test_lex.load_kwords("KEYWORDS.txt");
    test_lex.load_special("DELIMETER.txt", lex_types::delimeter);
    test_lex.load_special("EQ.txt", lex_types::equal);
    test_lex.load_special("REL_OPS.txt", lex_types::relative_operators);
    test_lex.load_special("open_bracket.txt", lex_types::open_bracket);
    test_lex.load_special("close_bracket.txt", lex_types::close_bracket);
    test_lex.load_special("or_op.txt", lex_types::or_op);
    test_lex.load_special("and_op.txt", lex_types::and_op);
    test_lex.load_special("pm_operator.txt", lex_types::pm_operator);
    test_lex.load_special("md_operator.txt", lex_types::md_operator);
    test_lex.load_special("open_par.txt", lex_types::open_par);
    test_lex.load_special("close_par.txt", lex_types::close_par);
    test_lex.load_special("prefix_operator.txt", lex_types::prefix_operator);
    test_lex.load_special("unary_operator.txt", lex_types::unary_operator);
    auto result = test_lex.parse_file("test_example.txt");
    save_lex_resul_to_file("result.csv", result);
    auto grammar = parse_grammar_file("grammar_rules.txt");
    auto first_follow_sets = first_follow(grammar);
    auto table = make_table(grammar, first_follow_sets);

    auto sorted_grammar = grammar;
    std::sort(sorted_grammar.begin(), sorted_grammar.end(), comp_pair);

    std::stack<std::string> buff;
    buff.push(nabla);
    result.push_back({.type = eof, .text = END_MARKER, .line = (--result.end())->line, .symbol = (--result.end())->symbol});
    size_t cur_lex = 0;
    auto &lexem = result[cur_lex++];
    while (cur_lex < result.size() || buff.size() != 2 || buff.top() != grammar[0].first)
    {
        auto last = buff.top();
        if (table[last].find(lex::lex_types_text(lexem.type)) == table[last].end())
        {
            std::cout << "Error on line " << lexem.line << " pos " << lexem.symbol << " lexem " << lex::lex_types_text(lexem.type) << std::endl;
            //cout_buff(buff);
            return 1;
        }
        else
        {
            bool is_error = false;
            switch (table[last][lex::lex_types_text(lexem.type)])
            {
            case shift:
                buff.push(lex::lex_types_text(lexem.type));
                lexem = result[cur_lex++];
                break;

            case reduce:
                is_error = true;
                for (auto &i : sorted_grammar)
                {
                    if (i.second == get_n_elements_stack(i.second.size(), buff))
                    {
                        delete_n_elements_stack(i.second.size(), buff);
                        buff.push(i.first);
                        is_error = false;
                        break;
                    }
                }
                if (is_error)
                {
                    std::cout << "Error on line " << lexem.line << " pos " << lexem.symbol << " lexem " << lex::lex_types_text(lexem.type) << std::endl;
                    //cout_buff(buff);
                    return 2;
                }
                break;

            default:
                std::cout << "Error on line " << lexem.line << " pos " << lexem.symbol << std::endl;
                return 3;
                break;
            }
        }
    }
    std::cout << "Success" << std::endl;
}
