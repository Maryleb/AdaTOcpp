#include <iostream>
#include <fstream>
#include <filesystem>
#include <memory>

#include <axx/lexer/Lexer.hpp>
#include <axx/parser/Parser.hpp>
#include <axx/semantic/SemanticAnalyzer.hpp>
#include <axx/codegen/CodeGenerator.hpp>

int main(int argc, char* argv[])
{
    if (argc > 1)
    {

        if (!std::filesystem::exists(argv[1]))
        {
            std::cerr << "File " << argv[1] << " does not exist!\n";
            return -1;
        }
        if (std::filesystem::is_directory(argv[1]))
        {
            std::cerr << argv[1] << " is a directory!\n";
            return -1;
        }

        std::ofstream output("output.cpp");

        auto lexer = std::make_unique<Lexer>();
        auto parser = std::make_unique<Parser>();
        auto seman = std::make_unique<SemanticAnalyzer>();
        auto codegen = std::make_unique<CodeGenerator>(output);

        // Выводим все лексемы, полученные лексером
        std::cout << "Lexer:\n";
        std::ifstream file_lexer(argv[1]);
        lexer->open(file_lexer);
        lexer->print_all_tokens();

        try
        {

            // Выводим дерево, полученное парсером
            std::cout << "\n\nParser:\n";
            std::ifstream file_parser(argv[1]);
            lexer->open(file_parser);
            parser->setLexer(lexer.get());
            auto ast = parser->getAST();
            ast->print();

            // Проводим семантический анализ дерева
            seman->check(ast);

            // Генерация кода
            codegen->generate(ast);
        }
        catch (std::exception e)
        {
            std::cerr << e.what() << std::endl;
            exit(-1);
        }
    }
    return 0;
}