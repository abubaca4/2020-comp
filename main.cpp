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
    test_lex.load_special("EQ.txt", lex_types::equal);
    test_lex.load_special("REL_OPS.txt", lex_types::relative_operators);
    test_lex.load_special("DELIMETER.txt", lex_types::delimeter);
    auto result = test_lex.parse_file("example.alg");
    save_lex_resul_to_file("result.csv", result);
}
