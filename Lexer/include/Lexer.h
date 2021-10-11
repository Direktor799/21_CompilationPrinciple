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

class Info
{
public:
    size_t m_line;
    size_t m_col;
    size_t m_wordLength;
    std::string m_type;
    std::string m_info;
    Info(size_t line, size_t col, size_t wordLength, std::string type, std::string info) : m_line(line), m_col(col), m_wordLength(wordLength), m_type(type), m_info(info){};
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
    std::vector<Info> infos;
    void nextChar();
    void rollBack(size_t length = 1);

public:
    Lexer(std::string path);
    void analyze();
    void outputTokens();
    void outputInfos();
};
