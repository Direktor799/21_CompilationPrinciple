#include "LL1Parser.h"
#include "LRParser.h"
#include <iostream>

int main() {
    // std::vector<Production> nonRecursiveProductions = {
    //     Production("E", {"T", "E\'"}),        Production("E\'", {"+", "T", "E\'"}),
    //     Production("E\'", {"-", "T", "E\'"}), Production("E\'", {"ε"}),
    //     Production("T", {"F", "T\'"}),        Production("T\'", {"*", "F", "T\'"}),
    //     Production("T\'", {"/", "F", "T\'"}), Production("T\'", {"ε"}),
    //     Production("F", {"(", "E", ")"}),     Production("F", {"num"})};

    // LL1TableCreator ll1tableCreator(nonRecursiveProductions, "E");
    // std::cout << ll1tableCreator.getLL1Table();
    // LL1Parser ll1parser(ll1tableCreator.getLL1Table(), ll1tableCreator.getStartNonTerminator());
    // std::string sentence;
    // std::cin >> sentence;
    // ll1parser.parse(sentence);

    LRTable lrtable(16);
    lrtable[0] = {std::make_pair("(", "s4"), std::make_pair("num", "s5"), std::make_pair("E", "1"),
                  std::make_pair("T", "2"), std::make_pair("F", "3")};
    lrtable[1] = {std::make_pair("+", "s6"), std::make_pair("-", "s7"), std::make_pair("$", "acc")};
    lrtable[2] = {std::make_pair("+", "r3"),   std::make_pair("-", "r3"), std::make_pair("*", "s8"),
                  std::make_pair("/", "s9"),   std::make_pair("(", "r3"), std::make_pair(")", "r3"),
                  std::make_pair("num", "r3"), std::make_pair("$", "r3")};
    lrtable[3] = {std::make_pair("+", "r6"),   std::make_pair("-", "r6"), std::make_pair("*", "r6"),
                  std::make_pair("/", "r6"),   std::make_pair("(", "r6"), std::make_pair(")", "r6"),
                  std::make_pair("num", "r6"), std::make_pair("$", "r6")};
    lrtable[4] = {std::make_pair("(", "s4"), std::make_pair("num", "s5"), std::make_pair("E", "10"),
                  std::make_pair("T", "2"), std::make_pair("F", "3")};
    lrtable[5] = {std::make_pair("+", "r8"),   std::make_pair("-", "r8"), std::make_pair("*", "r8"),
                  std::make_pair("/", "r8"),   std::make_pair("(", "r8"), std::make_pair(")", "r8"),
                  std::make_pair("num", "r8"), std::make_pair("$", "r8")};
    lrtable[6] = {std::make_pair("(", "s4"), std::make_pair("num", "s5"), std::make_pair("T", "11"),
                  std::make_pair("F", "3")};
    lrtable[7] = {std::make_pair("(", "s4"), std::make_pair("num", "s5"), std::make_pair("T", "12"),
                  std::make_pair("F", "3")};
    lrtable[8] = {std::make_pair("(", "s4"), std::make_pair("num", "s5"),
                  std::make_pair("F", "13")};
    lrtable[9] = {std::make_pair("(", "s4"), std::make_pair("num", "s5"),
                  std::make_pair("F", "14")};
    lrtable[10] = {std::make_pair("+", "s6"), std::make_pair("-", "s7"),
                   std::make_pair(")", "s15")};
    lrtable[11] = {std::make_pair("+", "r1"),   std::make_pair("-", "r1"),
                   std::make_pair("*", "s8"),   std::make_pair("/", "s9"),
                   std::make_pair("(", "r1"),   std::make_pair(")", "r1"),
                   std::make_pair("num", "r1"), std::make_pair("$", "r1")};
    lrtable[12] = {std::make_pair("+", "r2"),   std::make_pair("-", "r2"),
                   std::make_pair("*", "s8"),   std::make_pair("/", "s9"),
                   std::make_pair("(", "r2"),   std::make_pair(")", "r2"),
                   std::make_pair("num", "r2"), std::make_pair("$", "r2")};
    lrtable[13] = {std::make_pair("+", "r4"),   std::make_pair("-", "r4"),
                   std::make_pair("*", "r4"),   std::make_pair("/", "r4"),
                   std::make_pair("(", "r4"),   std::make_pair(")", "r4"),
                   std::make_pair("num", "r4"), std::make_pair("$", "r4")};
    lrtable[14] = {std::make_pair("+", "r5"),   std::make_pair("-", "r5"),
                   std::make_pair("*", "r5"),   std::make_pair("/", "r5"),
                   std::make_pair("(", "r5"),   std::make_pair(")", "r5"),
                   std::make_pair("num", "r5"), std::make_pair("$", "r5")};
    lrtable[15] = {std::make_pair("+", "r7"),   std::make_pair("-", "r7"),
                   std::make_pair("*", "r7"),   std::make_pair("/", "r7"),
                   std::make_pair("(", "r7"),   std::make_pair(")", "r7"),
                   std::make_pair("num", "r7"), std::make_pair("$", "r7")};

    std::vector<Production> productions(9);
    productions[0] = Production("E\'", {"E"});
    productions[1] = Production("E", {"E", "+", "T"});
    productions[2] = Production("E", {"E", "-", "T"});
    productions[3] = Production("E", {"T"});
    productions[4] = Production("T", {"T", "*", "F"});
    productions[5] = Production("T", {"T", "/", "F"});
    productions[6] = Production("T", {"F"});
    productions[7] = Production("F", {"(", "E", ")"});
    productions[8] = Production("F", {"num"});

    LRParser lrparser(lrtable, productions);
    std::string sentence;
    std::cin >> sentence;
    lrparser.parse(sentence);
}