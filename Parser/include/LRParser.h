#pragma once

#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Terminator = std::string;
using NonTerminator = std::string;
using Symbol = std::string;
using State = size_t;
using SententialForm = std::vector<Symbol>;
using Production = std::pair<NonTerminator, SententialForm>;
using LRTable = std::vector<std::unordered_map<Symbol, std::string>>;

class LRParser {
  private:
    LRTable m_table;
    std::vector<Production> m_productions;
    std::unordered_set<Terminator> m_terminatorSet;
    std::unordered_set<NonTerminator> m_nonTerminatorSet;
    Symbol _getSymbol(std::string &sentence);
    void _error(const std::string &sentence);

  public:
    LRParser(const LRTable &table, const std::vector<Production> &productions);
    bool parse(std::string sentence);
};