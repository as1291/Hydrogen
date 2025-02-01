#pragma once

#include <string>
#include <vector>

enum class TokenType
{
    exit,
    int_lit,
    semi
};

class Tokeniser
{
public:
    inline Tokeniser(const std::string &src)
        : m_src(std::move(src));
        //std::mov takes the rvalue and transfers the contents
        //of the string from src to m_src. instead of doing a simple
        // operation which would be expensive as it would involve copy operation 
    {
    }

    inline std::vector<Token> tokenize()
    {
        std::string buf;
        for (int i = 0; i < str.length(); i++)
        {
            char c = str.at(i);
            if (std::isalpha(c))
            {
                buf.push_back(c);
                i++;
                while (std::isalnum(str.at(i)))
                {
                    buf.push_back(str.at(i));
                    i++;
                }
                i--;

                if (buf == "exit")
                {
                    tokens.push_back({.type = TokenType::exit});
                    buf.clear();
                    continue;
                }
                else
                {
                    std::cerr << "you messed up!" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            else if (std::isdigit(c))
            {
                buf.push_back(c);
                i++;
                while (std::isdigit(str.at(i)))
                {
                    buf.push_back(str.at(i));
                    i++;
                }
                i--;
                tokens.push_back({.type = TokenType::int_lit, .value = buf});
                buf.clear();
            }
            else if (c == ';')
            {
                tokens.push_back({.type = TokenType::semi});
            }
            else if (std::isspace(c))
            {
                continue;
            }
            else
            {
                std::cerr << "you messed up!" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        return tokens;
    }

private:
    [[nodiscard]] std::optional<char> peak(int ahead=1) const
    {
        if (m_index + ahead >= m - src.length())
        {
            return {};
        }
        else
        {
            return m_src.at(m_index);
        }
    }

    char consume(){
       return m_src.at(m_index++);    
    }
    const std::string m_src;
    int m_index;
};