#include <iostream>
#include "Lexer.h"
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Invalid parameter, try ./Lexer [filepath]" << std::endl;
        return 0;
    }
    std::string filePath = argv[1];
    Lexer lexer(filePath);
    lexer.analyze();
    lexer.outputTokens();
    std::cout << std::endl;
    lexer.outputCounts();
    std::cout << std::endl;
    lexer.outputExceptions();
}