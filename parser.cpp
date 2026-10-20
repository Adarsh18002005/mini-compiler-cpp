#include "parser.hpp"
#include <iostream>
#include <limits>
#include <stdexcept>
#include <utility>

Interpreter::Interpreter(std::vector<Token> tokens) : tokens_(std::move(tokens)) {}
const Token& Interpreter::peek() const { return tokens_.at(position_); }
bool Interpreter::match(TokenType type) { if (peek().type == type) { ++position_; return true; } return false; }
[[noreturn]] void Interpreter::error(const Token& t, const std::string& msg) const {
    throw std::runtime_error("Syntax/runtime error at " + std::to_string(t.line) + ":" + std::to_string(t.column) + ": " + msg);
}
Token Interpreter::consume(TokenType type, const std::string& message) {
    if (peek().type != type) error(peek(), message);
    return tokens_.at(position_++);
}
long long Interpreter::factor() {
    if (match(TokenType::Minus)) return -factor();
    if (match(TokenType::Number)) {
        const Token& t = tokens_[position_-1];
        try { return std::stoll(t.lexeme); }
        catch (const std::exception&) { error(t, "integer literal out of range"); }
    }
    if (match(TokenType::Identifier)) {
        const Token& t = tokens_[position_-1];
        auto it = variables_.find(t.lexeme);
        if (it == variables_.end()) error(t, "undefined variable '" + t.lexeme + "'");
        return it->second;
    }
    if (match(TokenType::LParen)) {
        auto value = expression(); consume(TokenType::RParen, "expected ')' after expression"); return value;
    }
    error(peek(), "expected a number, identifier, or parenthesized expression");
}
long long Interpreter::term() {
    auto value = factor();
    while (peek().type == TokenType::Star || peek().type == TokenType::Slash) {
        Token op = tokens_[position_++]; auto right = factor();
        if (op.type == TokenType::Star) value *= right;
        else { if (right == 0) error(op, "division by zero"); value /= right; }
    }
    return value;
}
long long Interpreter::expression() {
    auto value = term();
    while (peek().type == TokenType::Plus || peek().type == TokenType::Minus) {
        Token op = tokens_[position_++]; auto right = term();
        if (op.type == TokenType::Plus) value += right; else value -= right;
    }
    return value;
}
void Interpreter::statement() {
    if (match(TokenType::Let)) {
        auto id = consume(TokenType::Identifier, "expected variable name after 'let'");
        consume(TokenType::Assign, "expected '=' after variable name");
        auto value = expression(); consume(TokenType::Semicolon, "expected ';' after declaration");
        variables_[id.lexeme] = value; return;
    }
    if (match(TokenType::Print)) {
        auto value = expression(); consume(TokenType::Semicolon, "expected ';' after print statement");
        std::cout << value << '\n'; return;
    }
    error(peek(), "expected 'let' or 'print'");
}
void Interpreter::run() { while (peek().type != TokenType::End) statement(); }
