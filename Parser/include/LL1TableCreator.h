#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

typedef std::string Terminator;
typedef std::string NonTerminator;
typedef std::vector<std::string> SententialForm;
typedef std::pair<NonTerminator, SententialForm> Production;
typedef std::unordered_map<NonTerminator, std::unordered_map<Terminator, Production>> LL1Table;

class LL1TableCreator
{
private:
    std::unordered_set<NonTerminator> m_nonTerminatorSet;
    std::unordered_map<NonTerminator, std::unordered_set<SententialForm>> m_productions;
    std::unordered_set<Terminator> _getFirstSet(NonTerminator term);
    std::unordered_set<Terminator> _getFollowSet(NonTerminator term);
    bool _isNonTerminator(NonTerminator term);

public:
    LL1TableCreator(std::vector<std::string> productions);
    void setProductions(std::vector<std::string> productions);
    LL1Table getLL1Table();
    ~LL1TableCreator() = default;
};
