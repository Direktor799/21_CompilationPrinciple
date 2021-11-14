#include "iostream"
#include "LL1Parser.h"

int main()
{
    std::vector<std::string> productions = {"E→E+T|E-T|T", "T→T*F|T/F|F", "F→(E)|num"};
    LL1TableCreator ll1tableCreator(productions);
    LL1Table ll1table = ll1tableCreator.getLL1Table();
    LL1Parser ll1parser(ll1table);
    std::string sentence;
    std::cin >> sentence;
    ll1parser.parse(sentence);
}