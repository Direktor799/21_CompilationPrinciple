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
using SententialForm = std::vector<Symbol>;
using Production = std::pair<NonTerminator, SententialForm>;
using LL1Table = std::unordered_map<NonTerminator, std::unordered_map<Terminator, Production>>;

std::ostream &operator<<(std::ostream &out, const LL1Table &table);

class LL1TableCreator {
  private:
    std::unordered_set<Terminator> m_terminatorSet;
    std::unordered_set<NonTerminator> m_nonTerminatorSet;
    NonTerminator m_startNonTerminator;
    std::vector<Production> m_productions;
    std::unordered_map<Symbol, std::unordered_set<Terminator>> m_firstSet;
    std::unordered_map<NonTerminator, std::unordered_set<Terminator>> m_followSet;
    LL1Table m_table;
    void _setSymbolSet();
    bool _isNonTerminator(Symbol symbol);
    void _calculateFirstSet();
    void _calculateFollowSet();
    void _calculateLL1Table();
    std::unordered_set<Terminator> _getFirstOf(const SententialForm &sententialForm);

  public:
    LL1TableCreator(const std::vector<Production> &productions,
                    const NonTerminator &startNonTerminator);
    LL1Table getLL1Table() const;
    const NonTerminator getStartNonTerminator() const;
};
