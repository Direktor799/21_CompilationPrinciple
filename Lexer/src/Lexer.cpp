#include "Lexer.h"

Lexer::Lexer(std::string filePath) : pos(0)
{
    std::fstream file(filePath);
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

void Lexer::nextChar()
{
    if (pos >= program.length())
        currentChar = EOF;
    else
    {
        currentWord += program[pos++];
        currentChar = currentWord.back();
    }
}

void Lexer::rollBack(size_t length)
{
    pos = std::max(pos - length, 0UL);
    currentWord = currentWord.substr(0, currentWord.length() - length);
    currentChar = currentWord.back();
}

void Lexer::analyze()
{
    while (true)
    {
        nextChar();
        //std::cout << int(currentChar) << std::endl;
        if (currentChar == EOF)
            break;
        else if (!isprint(currentChar)) //跳过空格
        {
            continue;
        }
        else if (currentChar == '#') //跳过宏
        {
            while (currentChar != '\n')
                nextChar();
        }
        else if (currentChar == '/')  //跳过注释
        {
            nextChar();
            if (currentChar == '/')
            {
                while (currentChar != '\n')
                    nextChar();
            }
            else if (currentChar == '*')
            {
                while (true)
                {
                    nextChar();
                    if (currentChar == '*')
                    {
                        nextChar();
                        if (currentChar == '/')
                            break;
                        else
                            rollBack();
                    }
                }
            }
        }
        else if (isalpha(currentChar) || currentChar == '_') //标识符
        {
            while (isalnum(currentChar) || currentChar == '_')
                nextChar();
            rollBack();
            if (keyWord.find(currentWord) != keyWord.end())
                tokens.emplace_back(Token("KeyWord", currentWord));
            else
                tokens.emplace_back(Token("ID", currentWord));
        }
        else if (isalnum(currentChar)) //数字常量
        {

        }
        else if (currentChar == '\'') //字符常量
        {
            nextChar();
            if (currentChar == '\\')
            {

            }
            else
            {

            }
        }
        else if (currentChar == '\"') //字符串常量
        {
            /* code */
        }
        else if (currentChar == '<')
        {
            /* code */
        }
        else if (currentChar == '>')
        {
            /* code */
        }
        currentWord.clear();
    }
}

void Lexer::outputTokens()
{
    for (auto &token : tokens)
        std::cout << token << std::endl;
}