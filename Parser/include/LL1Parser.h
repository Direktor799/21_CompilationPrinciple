#pragma once

#include "LL1TableCreator.h"
#include <iostream>

class LL1Parser {
  private:
    LL1Table m_table;
    NonTerminator m_startNonTerminator;
    std::unordered_set<Terminator> m_terminatorSet;
    std::unordered_set<NonTerminator> m_nonTerminatorSet;
    void _setSymbolSet();
    Symbol _getSymbol(std::string &sentence);
    bool _isNonTerminator(const Symbol &symbol);
    bool _isTerminator(const Symbol &symbol);
    void _error(const std::string &sentence);

  public:
    LL1Parser(const LL1Table &table, const NonTerminator &startNonTerminator);
    bool parse(std::string sentence);
};