#pragma once
#include "lexer.hpp"
#include <map>
#include <string>
#include <vector>

class Interpreter {
public:
    explicit Interpreter(std::vector<Token> tokens);
    void run();
    const std::map<std::string, long long>& symbols() const { return variables_; }
private:
    std::vector<Token> tokens_;
    size_t position_ = 0;
    std::map<std::string, long long> variables_;
    const Token& peek() const;
    bool match(TokenType type);
    Token consume(TokenType type, const std::string& message);
    long long expression();
    long long term();
    long long factor();
    void statement();
    [[noreturn]] void error(const Token& token, const std::string& message) const;
};
