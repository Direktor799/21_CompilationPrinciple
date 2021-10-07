#include "Lexer.h"

Lexer::Lexer(std::string path) : pos(0), line(1), col(1)
{
    filePath = path;
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
        if (currentChar == '\n')
        {
            line++;
            col = 1;
        }
        else
            col++;
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
        currentWord.clear();
        nextChar();
        if (currentChar == EOF)
            break;
        if (!isprint(currentChar)) //跳过空格
        {
            continue;
        }
        if (currentChar == '#') //跳过宏
        {
            while (currentChar != '\n')
                nextChar();
            continue;
        }
        if (currentChar == '/') //跳过注释
        {
            nextChar();
            if (currentChar == '/')
            {
                while (currentChar != '\n')
                    nextChar();
                continue;
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
                continue;
            }
            else    //可能是除号
            {
                rollBack();
            }
        }
        if (isalpha(currentChar) || currentChar == '_') //标识符
        {
            while (isalnum(currentChar) || currentChar == '_')
                nextChar();
            rollBack();
            if (keyWord.find(currentWord) != keyWord.end())
                tokens.emplace_back(Token(currentWord, currentWord));
            else
                tokens.emplace_back(Token("id", currentWord));
        }
        else if (isdigit(currentChar) || isdigit(currentChar)) //数字常量
        {
            if (currentChar == '0') //bin, oct, hex
            {
                nextChar();
                if (currentChar == 'b' || currentChar == 'B')
                {
                    while (currentChar == '0' || currentChar == '1')
                        nextChar();
                        int a = 0x11ull;
                }
                else if (currentChar == 'x' || currentChar == 'X')
                {

                }
                else
                {
                    
                }
            }
            else    //dec, float
            {

            }
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
            nextChar();
            if (currentChar != '=')
                rollBack();
            tokens.emplace_back(Token("rel_op", currentWord));
        }
        else if (currentChar == '>')
        {
            nextChar();
            if (currentChar != '=')
                rollBack();
            tokens.emplace_back(Token("rel_op", currentWord));
        }
        else if (currentChar == '=')
        {
            nextChar();
            if (currentChar == '=')
                tokens.emplace_back(Token("rel_op", currentWord));
            else
            {
                rollBack();
                tokens.emplace_back(Token("assign_op", currentWord));
            }
        }
        else if (currentChar == '!')
        {
            nextChar();
            if (currentChar == '=')
                tokens.emplace_back(Token("rel_op", currentWord));
            else
            {
                rollBack();
                tokens.emplace_back(Token("not_op", currentWord));
            }
        }
        else if (currentChar == '+')
        {
            nextChar();
            if (currentChar == '=')
                tokens.emplace_back(Token("plus_assign_op", currentWord));
            else if (currentChar == '+')
                tokens.emplace_back(Token("inc_op", currentWord));
            else
            {
                rollBack();
                tokens.emplace_back(Token("plus_op", currentWord));
            }
        }
        else if (currentChar == '-')
        {
            nextChar();
            if (currentChar == '=')
                tokens.emplace_back(Token("sub_assign_op", currentWord));
            else if (currentChar == '-')
                tokens.emplace_back(Token("dec_op", currentWord));
            else
            {
                rollBack();
                tokens.emplace_back(Token("sub_op", currentWord));
            }
        }
        else if (currentChar == '*')
        {
            nextChar();
            if (currentChar == '=')
                tokens.emplace_back(Token("mul_assign_op", currentWord));
            else
            {
                rollBack();
                tokens.emplace_back(Token("mul_op", currentWord));
            }
        }
        else if (currentChar == '/')
        {
            nextChar();
            if (currentChar == '=')
                tokens.emplace_back(Token("div_assign_op", currentWord));
            else
            {
                rollBack();
                tokens.emplace_back(Token("div_op", currentWord));
            }
        }
        else if (currentChar == '|')
        {
            nextChar();
            if (currentChar == '|')
                tokens.emplace_back(Token("rel_op", currentWord));
            else if (currentChar == '=')
                tokens.emplace_back(Token("or_assign_op", currentWord));
            else
            {
                rollBack();
                tokens.emplace_back(Token("or_op", currentWord));
            }
        }
        else if (currentChar == '&')
        {
            nextChar();
            if (currentChar == '&')
                tokens.emplace_back(Token("rel_op", currentWord));
            else if (currentChar == '=')
                tokens.emplace_back(Token("and_assign_op", currentWord));
            else
            {
                rollBack();
                tokens.emplace_back(Token("and_op", currentWord));
            }
        }
        else if (currentChar == '^')
        {
            nextChar();
            if (currentChar == '=')
                tokens.emplace_back(Token("xor_assign_op", currentWord));
            else
            {
                rollBack();
                tokens.emplace_back(Token("xor_op", currentWord));
            }
        }
        else if (currentChar == '(' || currentChar == ')' ||
        currentChar == '{' || currentChar == '}' ||
        currentChar == '[' || currentChar == ']' ||
        currentChar == ';')
        {
            tokens.emplace_back(Token("sep", currentWord));
        }
    }
}

void Lexer::outputTokens()
{
    for (auto &token : tokens)
        std::cout << token << std::endl;
}

void Lexer::outputErrorInfos()
{
    for (auto &errorinfo : errorinfos)
    {
        std::cout << filePath << ':' << errorinfo.m_line << ':' << errorinfo.m_col << ": error:";
        std::cout << errorinfo.m_error << std::endl;
    }
}