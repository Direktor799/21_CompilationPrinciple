#include "Lexer.h"

Lexer::Lexer(std::string filePath) : pos(0), status(Start)
{
    std::fstream file("../" + filePath);
    if (!file.is_open())
    {
        std::cout << "Can not open " + filePath << std::endl;
        return;
    }
    std::stringstream ss;
    ss << file.rdbuf();
    program = ss.str();
    file.close();
}

const char Lexer::getNextChar()
{
    if (pos >= program.length())
        return EOF;
    nowWord += program[pos++];
    return nowWord.back();
}

void Lexer::rollBack(size_t length)
{
    pos = std::max(pos - length, 0UL);
    nowWord = nowWord.substr(0, nowWord.length() - length);
}

void Lexer::analyze()
{
    char c;
    while (c = getNextChar())
    {
        nowWord.clear();
        if (status == Start)
        {
            if (!isprint(c)) //跳过空格
            {
                continue;
            }
            else if (c == '#') //跳过宏
            {
                while (c != '\n')
                    getNextChar();
            }
            else if (isalpha(c)) //标识符
            {
                while (isalnum(c) || c == '_')
                    c = getNextChar();
                rollBack();
                if (keyWord.find(nowWord) != keyWord.end())
                    tokens.emplace_back(Token("KeyWord", nowWord));
                else
                    tokens.emplace_back(Token("ID", nowWord));
            }
            else if (isalnum(c))
            {
            }
            else if (c == '<')
            {
                /* code */
            }
            else if (c == '>')
            {
                /* code */
            }
        }
    }
}

void Lexer::outputTokens()
{
    for (auto &token : tokens)
        std::cout << token;
}