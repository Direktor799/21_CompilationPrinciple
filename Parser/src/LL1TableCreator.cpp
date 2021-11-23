#include "LL1TableCreator.h"

std::ostream &operator<<(std::ostream &out, const LL1Table &table) {
    std::unordered_set<Terminator> terminatorSet;
    std::unordered_set<NonTerminator> nonTerminatorSet;
    size_t maxProductionLength = 0;
    for (auto &state : table) {
        nonTerminatorSet.insert(state.first);
    }
    for (auto &state : table) {
        for (auto &option : state.second) {
            terminatorSet.insert(option.first);
            size_t length = option.second.first.length();
            for (auto &symbol : option.second.second) {
                length += symbol.length();
            }
            maxProductionLength = std::max(length, maxProductionLength);
        }
    }
    out.setf(std::ios_base::left);
    out << std::setw(maxProductionLength + 2) << "";
    for (auto &terminator : terminatorSet) {
        out << std::setw(maxProductionLength + 2) << terminator;
    }
    out << std::endl;
    for (auto &nonTerminator : nonTerminatorSet) {
        out << std::setw(maxProductionLength + 2) << nonTerminator;
        for (auto &terminator : terminatorSet) {
            if (table.at(nonTerminator).find(terminator) != table.at(nonTerminator).end()) {
                size_t addition = 2;
                std::string content = table.at(nonTerminator).at(terminator).first + "→";
                for (auto &symbol : table.at(nonTerminator).at(terminator).second) {
                    content += symbol;
                    if (symbol == "ε") {
                        addition++;
                    }
                }
                out << std::setw(maxProductionLength + 2 + addition) << content;
            } else {
                out << std::setw(maxProductionLength + 2) << "";
            }
        }
        out << std::endl;
    }
    return out << std::endl;
}

LL1TableCreator::LL1TableCreator(const std::vector<Production> &productions,
                                 const NonTerminator &startNonTerminator)
    : m_productions(productions), m_startNonTerminator(startNonTerminator) {
    _setSymbolSet();
    _calculateLL1Table();
}

void LL1TableCreator::_setSymbolSet() {
    m_nonTerminatorSet.clear();
    m_terminatorSet.clear();

    for (auto &production : m_productions) {
        m_nonTerminatorSet.insert(production.first);
    }

    for (auto &production : m_productions) {
        for (auto &symbol : production.second) {
            if (!_isNonTerminator(symbol)) {
                m_terminatorSet.insert(symbol);
            }
        }
    }

    _calculateFirstSet();
    _calculateFollowSet();
}

void LL1TableCreator::_calculateFirstSet() {
    m_firstSet.clear();
    for (auto &X : m_terminatorSet) {
        m_firstSet[X] = {X};
    }

    while (true) {
        size_t preFirstSetSize = m_firstSet.size();
        for (auto &production : m_productions) {
            const NonTerminator &X = production.first;
            const SententialForm &Y = production.second;
            bool isXNullable = true;
            for (size_t i = 0; i < Y.size(); i++) {
                bool isYiNullable = false;
                for (auto &firstOfYi : m_firstSet[Y[i]]) {
                    if (firstOfYi == "ε") {
                        isYiNullable = true;
                    } else {
                        m_firstSet[X].insert(firstOfYi);
                    }
                }
                if (!isYiNullable) {
                    isXNullable = false;
                    break;
                }
            }
            if (isXNullable) {
                m_firstSet[X].insert("ε");
            }
        }
        if (m_firstSet.size() == preFirstSetSize) {
            break;
        }
    }
}

void LL1TableCreator::_calculateFollowSet() {
    m_followSet.clear();
    m_followSet[m_startNonTerminator] = {"$"};
    while (true) {
        size_t preFollowSetSize = m_followSet.size();
        for (auto &production : m_productions) {
            const NonTerminator &A = production.first;
            for (auto symbolIter = production.second.begin(); symbolIter < production.second.end();
                 symbolIter++) {
                const NonTerminator &B = *symbolIter;
                if (!_isNonTerminator(B)) {
                    continue;
                }
                SententialForm beta(symbolIter + 1, production.second.end());
                bool isBetaNullable = false;
                for (auto &firstSymbol : _getFirstOf(beta)) {
                    if (firstSymbol == "ε") {
                        isBetaNullable = true;
                    } else {
                        m_followSet[B].insert(firstSymbol);
                    }
                }
                if (isBetaNullable) {
                    for (auto &firstSymbol : m_followSet[A]) {
                        m_followSet[B].insert(firstSymbol);
                    }
                }
            }
        }
        if (m_followSet.size() == preFollowSetSize) {
            break;
        }
    }
}

bool LL1TableCreator::_isNonTerminator(Symbol symbol) {
    return m_nonTerminatorSet.find(symbol) != m_nonTerminatorSet.end();
}

std::unordered_set<Terminator> LL1TableCreator::_getFirstOf(const SententialForm &sententialForm) {
    std::unordered_set<Terminator> firstSet;
    bool isSententialFormNullable = true;
    for (size_t i = 0; i < sententialForm.size(); i++) {
        bool isSymbolNullable = false;
        for (auto &firstOfSymbol : m_firstSet[sententialForm[i]]) {
            if (firstOfSymbol == "ε") {
                isSymbolNullable = true;
            } else {
                firstSet.insert(firstOfSymbol);
            }
        }
        if (!isSymbolNullable) {
            isSententialFormNullable = false;
            break;
        }
    }
    if (isSententialFormNullable) {
        firstSet.insert("ε");
    }
    return firstSet;
}

void LL1TableCreator::_calculateLL1Table() {
    m_table.clear();
    for (auto &production : m_productions) {
        const NonTerminator &A = production.first;
        const SententialForm &alpha = production.second;
        auto firstSetOfAlpha = _getFirstOf(alpha);
        for (auto &a : firstSetOfAlpha) {
            if (a != "ε") {
                m_table[A][a] = production;
            }
        }
        if (firstSetOfAlpha.find("ε") != firstSetOfAlpha.end()) {
            for (auto &b : m_followSet.at(A)) {
                m_table[A][b] = production;
            }
        }
    }
}

LL1Table LL1TableCreator::getLL1Table() const { return m_table; }

const NonTerminator LL1TableCreator::getStartNonTerminator() const { return m_startNonTerminator; }
