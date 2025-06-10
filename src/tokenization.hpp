#pragma once

#include <string>
#include <vector>
#include <optional>
#include <iostream>
#include <algorithm>
// #include "./token.hpp"

enum class TokenType
{
    exit,
    int_lit,
    semi,
    open_paren,
    close_paren,
    ident,
    let,
    eq,
    plus,
    star,
    sub,
    div,
    open_curly,
    close_curly,
    if_,
    elif,
    else_
};

std::optional<int> bin_prec(const TokenType type)
{
    switch (type)
    {
    case TokenType::plus:
    case TokenType::sub:
        return 0;
    case TokenType::star:
    case TokenType::div:
        return 1;
    default:
        return {};
    }
}

struct Token
{
    TokenType type;
    std::optional<std::string> value{};
};

class Tokenizer
{
public:
    explicit Tokenizer(const std::string &src)
        : m_src(std::move(src))
    // std::mov takes the rvalue and transfers the contents
    // of the string from src to m_src. instead of doing a simple
    //  operation which would be expensive as it would involve copy operation
    {
    }

    std::vector<Token> tokenize()
    {
        std::string buf;
        std::vector<Token> tokens;
        while (peek().has_value())
        {
            if (std::isalpha(peek().value()))
            {
                buf.push_back(consume());
                while (peek().has_value() && std::isalnum(peek().value()))
                {
                    buf.push_back(consume());
                }
                if (buf == "exit")
                {
                    tokens.push_back({.type = TokenType::exit});
                    buf.clear();
                    continue;
                }
                else if (buf == "let")
                {
                    tokens.push_back({.type = TokenType::let});
                    buf.clear();
                    continue;
                }
                else if (buf == "if")
                {
                    tokens.push_back({.type = TokenType::if_});
                    buf.clear();
                    continue;
                }
                else if (buf == "elif")
                {
                    tokens.push_back({.type = TokenType::elif});
                    buf.clear();
                    continue;
                }
                else if (buf == "else")
                {
                    tokens.push_back({.type = TokenType::else_});
                    buf.clear();
                    continue;
                }

                else
                {
                    tokens.push_back({.type = TokenType::ident, .value = buf});
                    buf.clear();
                    continue;
                }
            }
            else if (std::isdigit(peek().value()))
            {
                buf.push_back(consume());
                while (peek().has_value() && std::isdigit(peek().value()))
                {
                    buf.push_back(consume());
                }
                tokens.push_back({.type = TokenType::int_lit, .value = buf});
                buf.clear();
                continue;
            }
            else if (peek().value() == '/' && peek(1).has_value() && peek(1).value() == '/')
            {
                consume();
                consume();
                while (peek().has_value() && peek().value() != '\n')
                {
                    consume();
                }
            }
            else if (peek().value() == '/' && peek(1).has_value() && peek(1).value() == '*')
            {
                consume();
                consume();
                while (peek().has_value())
                {
                    if (peek().has_value() && peek().value() == '*' && peek(1).has_value() && peek(1).value() == '/')
                        break;
                    consume();
                }
                if (peek().has_value())
                {
                    consume();
                }
                if (peek().has_value())
                {
                    consume();
                }
            }
            else if (peek().value() == '(')
            {
                consume();
                tokens.push_back({.type = TokenType::open_paren});
                continue;
            }
            else if (peek().value() == ')')
            {
                consume();
                tokens.push_back({.type = TokenType::close_paren});
                continue;
            }
            else if (peek().value() == ';')
            {
                consume();
                tokens.push_back({.type = TokenType::semi});
                continue;
            }
            else if (peek().value() == '=')
            {
                consume();
                tokens.push_back({.type = TokenType::eq});
                continue;
            }
            else if (std::isspace(peek().value()))
            {
                consume();
                continue;
            }
            else if (peek().value() == '+')
            {
                consume();
                tokens.push_back({.type = TokenType::plus});
                continue;
            }
            else if (peek().value() == '*')
            {
                consume();
                tokens.push_back({.type = TokenType::star});
                continue;
            }
            else if (peek().value() == '-')
            {
                consume();
                tokens.push_back({.type = TokenType::sub});
                continue;
            }
            else if (peek().value() == '/')
            {
                consume();
                tokens.push_back({.type = TokenType::div});
                continue;
            }
            else if (peek().value() == '{')
            {
                consume();
                tokens.push_back({.type = TokenType::open_curly});
                continue;
            }
            else if (peek().value() == '}')
            {
                consume();
                tokens.push_back({.type = TokenType::close_curly});
                continue;
            }
            else
            {
                // std::cout << "(unrecognized token)" << std::endl;
                std::cerr << "you messed up!" << std::endl;
                exit(EXIT_FAILURE);
                // std::terminate();
            }
        }
        m_index = 0;
        return tokens;
    }

private:
    [[nodiscard]] std::optional<char> peek(int offset = 0) const
    {
        if (m_index + offset >= m_src.length())
        {
            return {};
        }
        return m_src.at(m_index + offset);
    }

    char consume()
    {
        return m_src.at(m_index++);
    }
    const std::string m_src;
    int m_index = 0;
};
