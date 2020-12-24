#include "lib/lex.hpp"

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
}
