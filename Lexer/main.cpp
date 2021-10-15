#include <iostream>
#include "Lexer.h"
int main()
{
    std::string filePath = "test.c";
    Lexer lexer(filePath);
    lexer.analyze();
    lexer.outputTokens();
    std::cout << std::endl;
    lexer.outputCounts();
    std::cout << std::endl;
    lexer.outputExceptions();
}