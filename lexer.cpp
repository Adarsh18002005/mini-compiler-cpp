#include "lexer.hpp"
#include <cctype>
#include <stdexcept>

std::string tokenName(TokenType t) {
    switch (t) {
        case TokenType::Let: return "LET"; case TokenType::Print: return "PRINT";
        case TokenType::Identifier: return "IDENTIFIER"; case TokenType::Number: return "NUMBER";
        case TokenType::Plus: return "PLUS"; case TokenType::Minus: return "MINUS";
        case TokenType::Star: return "STAR"; case TokenType::Slash: return "SLASH";
        case TokenType::Assign: return "ASSIGN"; case TokenType::Semicolon: return "SEMICOLON";
        case TokenType::LParen: return "LPAREN"; case TokenType::RParen: return "RPAREN";
        case TokenType::End: return "EOF";
    }
    return "UNKNOWN";
}
std::vector<Token> tokenize(const std::string& s) {
    std::vector<Token> out;
    size_t i = 0; int line = 1, col = 1;
    while (i < s.size()) {
        char c = s[i];
        if (c == '\n') { ++i; ++line; col = 1; continue; }
        if (std::isspace(static_cast<unsigned char>(c))) { ++i; ++col; continue; }
        if (c == '#') { while (i < s.size() && s[i] != '\n') { ++i; ++col; } continue; }
        int startCol = col; size_t start = i;
        if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
            while (i < s.size() && (std::isalnum(static_cast<unsigned char>(s[i])) || s[i] == '_')) { ++i; ++col; }
            auto word = s.substr(start, i - start);
            out.push_back({word == "let" ? TokenType::Let : word == "print" ? TokenType::Print : TokenType::Identifier, word, line, startCol});
            continue;
        }
        if (std::isdigit(static_cast<unsigned char>(c))) {
            while (i < s.size() && std::isdigit(static_cast<unsigned char>(s[i]))) { ++i; ++col; }
            out.push_back({TokenType::Number, s.substr(start, i - start), line, startCol});
            continue;
        }
        TokenType type;
        switch (c) {
            case '+': type = TokenType::Plus; break; case '-': type = TokenType::Minus; break;
            case '*': type = TokenType::Star; break; case '/': type = TokenType::Slash; break;
            case '=': type = TokenType::Assign; break; case ';': type = TokenType::Semicolon; break;
            case '(': type = TokenType::LParen; break; case ')': type = TokenType::RParen; break;
            default: throw std::runtime_error("Lexical error at " + std::to_string(line) + ":" + std::to_string(col) + ": unexpected character '" + c + "'");
        }
        out.push_back({type, std::string(1,c), line, col}); ++i; ++col;
    }
    out.push_back({TokenType::End, "", line, col});
    return out;
}
