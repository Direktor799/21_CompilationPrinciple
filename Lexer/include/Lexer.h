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
                         "break", "default", "sizeof", "return"};

class Token
{
private:
    std::string m_type;
    std::string m_value;
public:
    Token(std::string type, std::string value) : m_type(type), m_value(value)
    {
        
    };
    friend std::ostream &operator<<(std::ostream &out, Token &token)
    {
        out << '<' << token.m_type << ", " << token.m_value << '>';
        return out;
    }
};

class Lexer
{
private:
    std::string program;
    std::string currentWord;
    char currentChar;
    size_t pos;
    std::vector<Token> tokens;
    void nextChar();
    void rollBack(size_t length = 1);

public:
    Lexer(std::string filePath);
    void analyze();
    void outputTokens();
};
