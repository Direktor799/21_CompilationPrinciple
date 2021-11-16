#include "LL1Parser.h"
#include "iostream"

int main() {
    std::vector<Production> productions = {
        Production("E", {"E", "+", "T"}), Production("E", {"E", "-", "T"}), Production("E", {"T"}),
        Production("T", {"T", "*", "F"}), Production("T", {"T", "/", "F"}), Production("T", {"F"}),
        Production("F", {"(", "E", ")"}), Production("F", {"num"})};

    std::vector<Production> nonRecursiveProductions = {
        Production("E", {"T", "E\'"}),        Production("E\'", {"+", "T", "E\'"}),
        Production("E\'", {"-", "T", "E\'"}), Production("E\'", {"ε"}),
        Production("T", {"F", "T\'"}),        Production("T\'", {"*", "F", "T\'"}),
        Production("T\'", {"/", "F", "T\'"}), Production("T\'", {"ε"}),
        Production("F", {"(", "E", ")"}),     Production("F", {"num"})};

    std::vector<Production> productions_4_4 = {Production("E", {"T", "E\'"}),
                                               Production("E\'", {"+", "T", "E\'"}),
                                               Production("E\'", {"ε"}),
                                               Production("T", {"F", "T\'"}),
                                               Production("T\'", {"*", "F", "T\'"}),
                                               Production("T\'", {"ε"}),
                                               Production("F", {"(", "E", ")"}),
                                               Production("F", {"id"})};

    LL1TableCreator ll1tableCreator(productions_4_4, "E");
    ll1tableCreator.calculateLL1Table();
    std::cout << ll1tableCreator.getLL1Table();
    LL1Parser ll1parser(ll1tableCreator.getLL1Table(), ll1tableCreator.getStartNonTerminator());
    std::string sentence;
    std::cin >> sentence;
    ll1parser.parse(sentence);
}