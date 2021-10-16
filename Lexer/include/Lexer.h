#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <set>
#include <vector>
#include <algorithm>
#include <unordered_map>

const std::set<std::string> keyWord = {"auto", "short", "int", "long",
                                       "float", "double", "char", "struct",
                                       "union", "enum", "typedef", "const",
                                       "unsigned", "signed", "extern", "register",
                                       "static", "volatile", "void", "if",
                                       "else", "switch", "case", "for",
                                       "do", "while", "goto", "continue",
                                       "break", "default", "sizeof", "return"};

class Token
{
public:
    size_t m_line;
    size_t m_col;
    std::string m_type;
    std::string m_value;
    std::string m_info;

    Token(size_t line, size_t col, std::string type, std::string value, std::string info = "")
        : m_line(line), m_col(col), m_type(type), m_value(value), m_info(info){};
    friend std::ostream &operator<<(std::ostream &out, const Token &token)
    {
        out << '<' << token.m_type << ", " << token.m_value << '>';
        return out;
    }
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
    void nextChar();
    void rollBack();

public:
    Lexer(std::string path);
    void analyze();
    void outputTokens();
    void outputExceptions();
    void outputCounts();
};
