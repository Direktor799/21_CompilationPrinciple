#include <iostream>
#include "Lexer.h"
int main()
{
    std::string filePath = "main.cpp";
    Lexer lexer(filePath);
    lexer.analyze();
    lexer.outputTokens();
}