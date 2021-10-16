#include "Lexer.h"
//to do:error&warning
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
        currentWord += program[pos++];
        currentChar = currentWord.back();
        if (currentChar == '\n')
        {
            line++;
            col = 1;
        }
        else
            col++;
    }
}

void Lexer::rollBack()
{
    pos = std::max(pos - 1, 0UL);
    if (currentChar == '\n')
    {
        line--;
        std::string lastLine = program.substr(program.find_last_of('\n', pos - 1) + 1);
        col = lastLine.find('\n') + 1;
    }
    else
    {
        col--;
    }
    currentWord = currentWord.substr(0, currentWord.length() - 1);
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
        if (!isprint(currentChar) || currentChar == ' ') //跳过换行和空格
        {
            continue;
        }
        if (currentChar == '#') //跳过宏
        {
            while (currentChar != '\n' && currentChar != EOF)
                nextChar();
            continue;
        }
        if (currentChar == '/') //跳过注释
        {
            nextChar();
            if (currentChar == '/')
            {
                while (currentChar != '\n' && currentChar != EOF)
                    nextChar();
                continue;
            }
            else if (currentChar == '*')
            {
                while (currentChar != EOF)
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
                if (currentChar == EOF)
                    tokens.emplace_back(Token(line, col - currentWord.length(), "Error", currentWord, "Unterminted Comment"));
                continue;
            }
            else //可能是除号
            {
                rollBack();
            }
        }
        if (isalpha(currentChar) || currentChar == '_') //标识符
        {
            while ((isalnum(currentChar) || currentChar == '_') && currentChar != EOF)
                nextChar();
            rollBack();
            if (keyWord.find(currentWord) != keyWord.end())
                tokens.emplace_back(Token(line, col - currentWord.length(), currentWord, currentWord));
            else
                tokens.emplace_back(Token(line, col - currentWord.length(), "id", currentWord));
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
                    while ((currentChar == '0' || currentChar == '1') && currentChar != EOF)
                        nextChar();
                }
                else if (currentChar == 'x' || currentChar == 'X')
                {
                    nextChar();
                    while ((isdigit(currentChar) || (currentChar >= 'a' && currentChar <= 'f') ||
                            (currentChar >= 'A' && currentChar <= 'F')) &&
                           currentChar != EOF)
                        nextChar();
                }
                else
                {
                    while ((currentChar >= '0' && currentChar <= '7') && currentChar != EOF)
                        nextChar();
                }
            }
            else //dec, float
            {
                if (currentChar != '.') //可能的整数部分
                {
                    while ((isdigit(currentChar)) && currentChar != EOF)
                        nextChar();
                }
                if (currentChar == '.') //可能的小数部分
                {
                    is_float = true;
                    nextChar();
                    while ((isdigit(currentChar)) && currentChar != EOF)
                        nextChar();
                }
                if (currentChar == 'e' || currentChar == 'E')
                {
                    nextChar();
                    while ((isdigit(currentChar)) && currentChar != EOF)
                        nextChar();
                }
            }
            if (is_float)
            {
                if (currentChar == 'f')
                {
                    tokens.emplace_back(Token(line, col - currentWord.length(), "value", currentWord));
                }
                else
                {
                    rollBack();
                    tokens.emplace_back(Token(line, col - currentWord.length(), "value", currentWord));
                }
            }
            else
            {
                if (currentChar == 'u' || currentChar == 'U')
                    nextChar();
                if (currentChar == 'L')
                {
                    nextChar();
                    if (currentChar == 'L')
                        nextChar();
                }
                else if (currentChar == 'l')
                {
                    nextChar();
                    if (currentChar == 'L')
                        nextChar();
                }
                rollBack();
                tokens.emplace_back(Token(line, col - currentWord.length(), "value", currentWord));
            }
        }
        else if (currentChar == '\'' || currentChar == '\"') //字符/字符串常量
        {
            char quotType = currentChar;
            nextChar();
            size_t stringLength = 0;
            while (currentChar != quotType && currentChar != EOF)
            {
                if (currentChar == '\\')
                {
                    size_t beforeEscapeLength = currentWord.length() - 1;
                    //NOT supported list:
                    //'\e'(console font color)
                    //'\u'(unicode)
                    //'\p'(unicode re)
                    nextChar();
                    if (currentChar >= '0' && currentChar <= '7')
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
                        while (isdigit(currentChar) || (currentChar >= 'a' && currentChar <= 'f') ||
                               (currentChar >= 'A' && currentChar <= 'F') && currentChar != EOF)
                        {
                            hexLength++;
                            nextChar();
                        }
                    }
                    else if (currentChar == '\r' || currentChar == '\n')
                    {
                        while ((currentChar == '\r' || currentChar == '\n') && currentChar != EOF)
                            nextChar();
                    }
                    else
                    {
                        tokens.emplace_back(Token(line, col - currentWord.substr(beforeEscapeLength).length(), "Warning", currentWord.substr(beforeEscapeLength), "Unknown Escape Sequence"));
                    }
                }
                else if (currentChar == '\n')
                {
                    break;
                }
                else
                    nextChar();
                stringLength++;
            }
            if (currentChar == EOF)
            {
                tokens.emplace_back(Token(line, col - currentWord.length(), "Error", currentWord, "Unterminated String"));
            }
            else if (currentChar == '\n')
            {
                rollBack();
                tokens.emplace_back(Token(line, col - currentWord.length(), "Error", currentWord, "Unterminated String"));
            }
            else
            {
                if (quotType == '\'' && stringLength > 1)
                    tokens.emplace_back(Token(line, col - currentWord.length(), "Warning", currentWord, "Multi-character Character"));
                tokens.emplace_back(Token(line, col - currentWord.length(), "value", currentWord));
            }
        }
        else if (currentChar == '<')
        {
            nextChar();
            if (currentChar != '=')
                rollBack();
            tokens.emplace_back(Token(line, col - currentWord.length(), "rel_op", currentWord));
        }
        else if (currentChar == '>')
        {
            nextChar();
            if (currentChar != '=')
                rollBack();
            tokens.emplace_back(Token(line, col - currentWord.length(), "rel_op", currentWord));
        }
        else if (currentChar == '=')
        {
            nextChar();
            if (currentChar == '=')
                tokens.emplace_back(Token(line, col - currentWord.length(), "rel_op", currentWord));
            else
            {
                rollBack();
                tokens.emplace_back(Token(line, col - currentWord.length(), "assign_op", currentWord));
            }
        }
        else if (currentChar == '!')
        {
            nextChar();
            if (currentChar == '=')
                tokens.emplace_back(Token(line, col - currentWord.length(), "rel_op", currentWord));
            else
            {
                rollBack();
                tokens.emplace_back(Token(line, col - currentWord.length(), "log_not_op", currentWord));
            }
        }
        else if (currentChar == '+')
        {
            nextChar();
            if (currentChar == '=')
                tokens.emplace_back(Token(line, col - currentWord.length(), "plus_assign_op", currentWord));
            else if (currentChar == '+')
                tokens.emplace_back(Token(line, col - currentWord.length(), "inc_op", currentWord));
            else
            {
                rollBack();
                tokens.emplace_back(Token(line, col - currentWord.length(), "plus_op", currentWord));
            }
        }
        else if (currentChar == '-')
        {
            nextChar();
            if (currentChar == '=')
                tokens.emplace_back(Token(line, col - currentWord.length(), "sub_assign_op", currentWord));
            else if (currentChar == '-')
                tokens.emplace_back(Token(line, col - currentWord.length(), "dec_op", currentWord));
            else if (currentChar == '>')
                tokens.emplace_back(Token(line, col - currentWord.length(), "member_pointer_op", currentWord));
            else
            {
                rollBack();
                tokens.emplace_back(Token(line, col - currentWord.length(), "sub_op", currentWord));
            }
        }
        else if (currentChar == '*')
        {
            nextChar();
            if (currentChar == '=')
                tokens.emplace_back(Token(line, col - currentWord.length(), "mul_assign_op", currentWord));
            else
            {
                rollBack();
                tokens.emplace_back(Token(line, col - currentWord.length(), "mul_op", currentWord));
            }
        }
        else if (currentChar == '/')
        {
            nextChar();
            if (currentChar == '=')
                tokens.emplace_back(Token(line, col - currentWord.length(), "div_assign_op", currentWord));
            else
            {
                rollBack();
                tokens.emplace_back(Token(line, col - currentWord.length(), "div_op", currentWord));
            }
        }
        else if (currentChar == '%')
        {
            nextChar();
            if (currentChar == '=')
                tokens.emplace_back(Token(line, col - currentWord.length(), "mod_assign_op", currentWord));
            else
            {
                rollBack();
                tokens.emplace_back(Token(line, col - currentWord.length(), "mod_op", currentWord));
            }
        }
        else if (currentChar == '|')
        {
            nextChar();
            if (currentChar == '|')
                tokens.emplace_back(Token(line, col - currentWord.length(), "rel_op", currentWord));
            else if (currentChar == '=')
                tokens.emplace_back(Token(line, col - currentWord.length(), "or_assign_op", currentWord));
            else
            {
                rollBack();
                tokens.emplace_back(Token(line, col - currentWord.length(), "or_op", currentWord));
            }
        }
        else if (currentChar == '&')
        {
            nextChar();
            if (currentChar == '&')
                tokens.emplace_back(Token(line, col - currentWord.length(), "rel_op", currentWord));
            else if (currentChar == '=')
                tokens.emplace_back(Token(line, col - currentWord.length(), "and_assign_op", currentWord));
            else
            {
                rollBack();
                tokens.emplace_back(Token(line, col - currentWord.length(), "and_op", currentWord));
            }
        }
        else if (currentChar == '^')
        {
            nextChar();
            if (currentChar == '=')
                tokens.emplace_back(Token(line, col - currentWord.length(), "xor_assign_op", currentWord));
            else
            {
                rollBack();
                tokens.emplace_back(Token(line, col - currentWord.length(), "xor_op", currentWord));
            }
        }
        else if (currentChar == '~')
        {
            tokens.emplace_back(Token(line, col - currentWord.length(), "bit_not_op", currentWord));
        }
        else if (currentChar == '<')
        {
            nextChar();
            if (currentChar == '<')
            {
                nextChar();
                if (currentChar == '=')
                    tokens.emplace_back(Token(line, col - currentWord.length(), "lshift_assign_op", currentWord));
                else
                {
                    rollBack();
                    tokens.emplace_back(Token(line, col - currentWord.length(), "lshift_op", currentWord));
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
                    tokens.emplace_back(Token(line, col - currentWord.length(), "rshift_assign_op", currentWord));
                else
                {
                    rollBack();
                    tokens.emplace_back(Token(line, col - currentWord.length(), "rshift_op", currentWord));
                }
            }
            else
                rollBack();
        }
        else if (currentChar == '.')
        {
            tokens.emplace_back(Token(line, col - currentWord.length(), "member_op", currentWord));
        }
        else if (currentChar == '(' || currentChar == ')' ||
                 currentChar == '{' || currentChar == '}' ||
                 currentChar == '[' || currentChar == ']' ||
                 currentChar == ',' || currentChar == ';' ||
                 currentChar == '?' || currentChar == ':')
        {
            tokens.emplace_back(Token(line, col - currentWord.length(), "sep", currentWord));
        }
        else
            tokens.emplace_back(Token(line, col - currentWord.length(), "Error", currentWord, "Unknown Character"));
    }
}

void Lexer::outputTokens()
{
    for (auto &token : tokens)
        if (token.m_type != "Error" && token.m_type != "Warning")
            std::cout << token << std::endl;
}

void Lexer::outputExceptions()
{
    for (auto &token : tokens)
    {
        if (token.m_type == "Error" || token.m_type == "Warning")
        {
            std::cout << filePath << ':' << token.m_line << ':' << token.m_col << ':';
            if (token.m_type == "Error")
                std::cout << "\033[31m" << token.m_type << "\033[0m";
            else
                std::cout << "\033[33m" << token.m_type << "\033[0m";
            std::cout << ':' << token.m_info << std::endl;
            std::string tmp = program;
            for (int i = 1; i < token.m_line; i++)
                tmp = tmp.substr(tmp.find('\n') + 1);
            tmp = tmp.substr(0, tmp.find('\n'));
            for (int i = 0; i < tmp.length(); i++)
            {
                if (i == token.m_col - 1)
                {
                    if (token.m_type == "Error")
                        std::cout << "\033[31m";
                    else
                        std::cout << "\033[33m";
                }
                std::cout << tmp[i];
                if (i == token.m_col + token.m_value.length() - 2)
                    std::cout << "\033[0m";
            }
            std::cout << std::endl;
        }
    }
}

void Lexer::outputCounts()
{
    int totalCharCount = program.length();
    int totalLineCount = 1;
    for (auto &char_ : program)
    {
        if (char_ == '\n')
        {
            totalLineCount++;
        }
    }
    if (program.back() == '\n')
        totalLineCount--;
    std::unordered_map<std::string, size_t> typesCount;
    for (auto &token : tokens)
    {
        typesCount[token.m_type]++;
    }
    for (auto &typeCount : typesCount)
    {
        std::cout << typeCount.first << ": " << typeCount.second << std::endl;
    }
    std::cout << "Total Line: " << totalLineCount << std::endl;
    std::cout << "Total Charactor: " << totalCharCount << std::endl;
}