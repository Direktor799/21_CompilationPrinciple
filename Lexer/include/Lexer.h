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

enum DFAStatus
{
    Start,

};

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
        out << '<' << token.m_type << ", " << token.m_value << '>' << std::endl;
        return out;
    }
};

class Lexer
{
private:
    std::string program;
    std::string nowWord;
    size_t pos;
    DFAStatus status;
    std::vector<Token> tokens;
    const char getNextChar();
    void rollBack(size_t length = 1);

public:
    Lexer(std::string filePath);
    void analyze();
    void outputTokens();
};
