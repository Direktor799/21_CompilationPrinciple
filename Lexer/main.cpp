#include <iostream>
#include "Lexer.h"
int main()
{
    std::string filePath = "test.c";
    Lexer lexer(filePath);
    lexer.analyze();
    lexer.outputTokens();
    lexer.outputInfos();
}