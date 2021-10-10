#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <set>
#include <vector>
#include <algorithm>

const std::set<std::string> keyWord = {"auto", "short", "int", "long",
                                       "float", "double", "char", "struct",
                                       "union", "enum", "typedef", "const",
                                       "unsigned", "signed", "extern", "register",
                                       "static", "volatile", "void", "if",
                                       "else", "switch", "case", "for",
                                       "do", "while", "goto", "continue",
                                       "break", "default", "sizeof", "return", "sizeof"};

class Token
{
private:
    std::string m_type;
    std::string m_value;

public:
    Token(std::string type, std::string value) : m_type(type), m_value(value){};
    friend std::ostream &operator<<(std::ostream &out, Token &token)
    {
        out << '<' << token.m_type << ", " << token.m_value << '>';
        return out;
    }
};

class ErrorInfo
{
public:
    size_t m_line;
    size_t m_col;
    std::string m_error;
    ErrorInfo(size_t line, size_t col, std::string error) : m_line(line), m_col(col), m_error(error){};
};

class Lexer
{
private:
    std::string filePath;
    std::string program;
    std::string currentWord;
    char currentChar;
    size_t pos;
    size_t line;
    size_t col;
    std::vector<Token> tokens;
    std::vector<ErrorInfo> errorinfos;
    void nextChar();
    void rollBack(size_t length = 1);

public:
    Lexer(std::string path);
    void analyze();
    void outputTokens();
    void outputErrorInfos();
};
