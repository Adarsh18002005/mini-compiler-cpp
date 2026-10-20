#pragma once
#include <string>
#include <vector>

enum class TokenType { Let, Print, Identifier, Number, Plus, Minus, Star, Slash, Assign, Semicolon, LParen, RParen, End };
struct Token { TokenType type; std::string lexeme; int line; int column; };
std::string tokenName(TokenType type);
std::vector<Token> tokenize(const std::string& source);
