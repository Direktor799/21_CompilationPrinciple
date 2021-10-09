#include "Lexer.h"
//to do: operator priority, error&warning, char&string, EOF
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
            else //可能是除号
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
        else if (isdigit(currentChar) || currentChar == '.') //数字常量
        {
            bool is_float = false;
            if (currentChar == '0') //bin, hex, oct
            {
                nextChar();
                if (currentChar == 'b' || currentChar == 'B')
                {
                    nextChar();
                    while (currentChar == '0' || currentChar == '1')
                        nextChar();
                }
                else if (currentChar == 'x' || currentChar == 'X')
                {
                    nextChar();
                    while (isdigit(currentChar) ||
                           (currentChar >= 'a' && currentChar <= 'f') ||
                           (currentChar >= 'A' && currentChar <= 'F'))
                        nextChar();
                }
                else
                {
                    while (currentChar >= '0' && currentChar <= '7')
                        nextChar();
                }
            }
            else //dec, float
            {
                if (currentChar != '.') //可能的整数部分
                {
                    while (isdigit(currentChar))
                        nextChar();
                }
                if (currentChar == '.') //可能的小数部分
                {
                    is_float = true;
                    nextChar();
                    while (isdigit(currentChar))
                        nextChar();
                }
                if (currentChar == 'e' || currentChar == 'E')
                {
                    nextChar();
                    while (isdigit(currentChar))
                        nextChar();
                }
            }
            if (is_float)
            {
                if (currentChar == 'f')
                {
                    tokens.emplace_back(Token("float", currentWord));
                }
                else
                {
                    rollBack();
                    tokens.emplace_back(Token("double", currentWord));
                }
            }
            else
            {
                std::string typeName;
                if (currentChar == 'u' || currentChar == 'U')
                {
                    typeName += "unsigned ";
                    nextChar();
                }
                if (currentChar == 'L')
                {
                    typeName += "long";
                    nextChar();
                    if (currentChar == 'L')
                    {
                        typeName += " long";
                        nextChar();
                    }
                }
                else if (currentChar == 'l')
                {
                    typeName += "long";
                    nextChar();
                    if (currentChar == 'L')
                    {
                        typeName += " long";
                        nextChar();
                    }
                }
                rollBack();
                if (typeName.length() == 0 || typeName == "unsigned ")
                    typeName += "int";
                tokens.emplace_back(Token(typeName, currentWord));
            }
        }
        else if (currentChar == '\'' || currentChar == '\"') //字符/字符串常量
        {
            char quotType = currentChar;
            nextChar();
            while (currentChar != quotType)
            {
                if (currentChar == '\\')
                {
                    //NOT supported list:
                    //'\e'(console font color)
                    //'\u'(unicode)
                    //'\p'(unicode re)
                    nextChar();
                    if (currentChar >= '0' && currentChar <= '7') //isdigit unknown
                    {
                        nextChar();
                        if (currentChar >= '0' && currentChar <= '7')
                        {
                            nextChar();
                            if (currentChar >= '0' && currentChar <= '7')
                                nextChar();
                        }
                    }
                    else if (currentChar == 'n' || currentChar == 'r' ||
                             currentChar == 't' || currentChar == 'v' || currentChar == 'a' ||
                             currentChar == 'b' || currentChar == 'f' || currentChar == '\'' ||
                             currentChar == '\"' || currentChar == '\\' || currentChar == '\?')
                        nextChar();
                    else if (currentChar == 'x')
                    {
                        nextChar();
                        int hexLength = 0;
                        while (isdigit(currentChar) ||
                               (currentChar >= 'a' && currentChar <= 'f') ||
                               (currentChar >= 'A' && currentChar <= 'F'))
                        {
                            hexLength++;
                            nextChar();
                        }
                    }
                    else if (currentChar == '\r' || currentChar == '\n')
                    {
                        while (currentChar == '\r' || currentChar == '\n')
                            nextChar();
                    }
                    else
                    {
                        //error
                    }
                }
                else
                    nextChar();
            }
            if (quotType == '\'')
                tokens.emplace_back("char", currentWord);
            else
                tokens.emplace_back("string", currentWord);
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
        else if (currentChar == '%')
        {
            nextChar();
            if (currentChar == '=')
                tokens.emplace_back(Token("mod_assign_op", currentWord));
            else
            {
                rollBack();
                tokens.emplace_back(Token("mod_op", currentWord));
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
        else if (currentChar == '<')
        {
            nextChar();
            if (currentChar == '<')
            {
                nextChar();
                if (currentChar == '=')
                    tokens.emplace_back(Token("lshift_assign_op", currentWord));
                else
                {
                    rollBack();
                    tokens.emplace_back(Token("lshift_op", currentWord));
                }
            }
            else
                rollBack();
        }
        else if (currentChar == '>')
        {
            nextChar();
            if (currentChar == '>')
            {
                nextChar();
                if (currentChar == '=')
                    tokens.emplace_back(Token("rshift_assign_op", currentWord));
                else
                {
                    rollBack();
                    tokens.emplace_back(Token("rshift_op", currentWord));
                }
            }
            else
                rollBack();
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