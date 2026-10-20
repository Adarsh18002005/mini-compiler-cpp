#include "lexer.hpp"
#include "parser.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

int main(int argc, char** argv) {
    if (argc < 2 || argc > 3) {
        std::cerr << "Usage: mini_compiler <source.mini> [--tokens|--symbols]\n";
        return 1;
    }
    std::string option = argc == 3 ? argv[2] : "";
    if (option != "" && option != "--tokens" && option != "--symbols") {
        std::cerr << "Unknown option: " << option << '\n'; return 1;
    }
    std::ifstream file(argv[1]);
    if (!file) { std::cerr << "Cannot open file: " << argv[1] << '\n'; return 1; }
    std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    try {
        auto tokens = tokenize(source);
        if (option == "--tokens") {
            for (const auto& t : tokens) std::cout << t.line << ':' << t.column << "  " << tokenName(t.type) << "  " << t.lexeme << '\n';
        } else {
            Interpreter interpreter(std::move(tokens)); interpreter.run();
            if (option == "--symbols") {
                std::cout << "--- Symbol table ---\n";
                for (const auto& [name, value] : interpreter.symbols()) std::cout << name << " = " << value << '\n';
            }
        }
    } catch (const std::exception& e) { std::cerr << e.what() << '\n'; return 1; }
}
