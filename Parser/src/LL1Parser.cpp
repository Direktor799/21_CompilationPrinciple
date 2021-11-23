#include "LL1Parser.h"

const int spaceWidth = 30;

LL1Parser::LL1Parser(const LL1Table &table, const NonTerminator &startNonTerminator)
    : m_table(table), m_startNonTerminator(startNonTerminator) {
    _setSymbolSet();
}

bool LL1Parser::parse(std::string sentence) {
    sentence += "$";
    std::vector<Symbol> symbolStack;
    symbolStack.push_back("$");
    symbolStack.push_back(m_startNonTerminator);
    Symbol a = _getSymbol(sentence);

    std::cout.setf(std::ios_base::left);
    std::cout << std::endl
              << std::setw(spaceWidth) << "Stack" << std::setw(spaceWidth) << "Sentence"
              << "Production" << std::endl;
    auto _outputState = [&]() {
        std::string content;
        for (auto &symbol : symbolStack) {
            content += symbol;
        }
        std::cout << std::setw(spaceWidth) << content;
        std::cout << std::setw(spaceWidth) << a + sentence;
    };

    auto _outputProduction = [&](const Production &production) {
        size_t addition = 2;
        std::string content = production.first + "→";
        for (auto &symbol : production.second) {
            content += symbol;
            if (symbol == "ε") {
                addition++;
            }
        }
        std::cout << content;
    };

    while (!symbolStack.empty()) {
        _outputState();
        const Symbol &X = symbolStack.back();
        if (_isTerminator(X)) {
            if (X == a) {
                symbolStack.pop_back();
                a = _getSymbol(sentence);
            } else {
                _error(a + sentence);
                return false;
            }
        } else if (_isNonTerminator(X)) {
            if (m_table[X].find(a) == m_table[X].end()) {
                _error(a + sentence);
                return false;
            }
            const Production &production = m_table[X][a];
            const SententialForm &sententialForm = production.second;
            _outputProduction(production);
            symbolStack.pop_back();
            for (auto iter = sententialForm.rbegin(); iter < sententialForm.rend(); iter++) {
                if (*iter != "ε") {
                    symbolStack.push_back(*iter);
                }
            }
        } else {
            _error(a);
            return false;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << "Accepted!" << std::endl;
    return true;
}

void LL1Parser::_setSymbolSet() {
    m_terminatorSet.clear();
    m_nonTerminatorSet.clear();
    if (m_table.empty())
        return;
    for (auto &state : m_table) {
        m_nonTerminatorSet.insert(state.first);
    }
    for (auto &state : m_table) {
        for (auto &option : state.second) {
            m_terminatorSet.insert(option.first);
        }
    }
}

Symbol LL1Parser::_getSymbol(std::string &sentence) {
    Symbol a;
    while (!_isTerminator(a) && !_isNonTerminator(a) && !sentence.empty()) {
        a += sentence.front();
        sentence = sentence.substr(1);
    }
    return a;
}

bool LL1Parser::_isNonTerminator(const Symbol &symbol) {
    return m_nonTerminatorSet.find(symbol) != m_nonTerminatorSet.end();
}

bool LL1Parser::_isTerminator(const Symbol &symbol) {
    return m_terminatorSet.find(symbol) != m_terminatorSet.end();
}

void LL1Parser::_error(const std::string &sentence) {
    std::cout << std::endl << std::endl << "Can not parse this sentence, ";
    std::cout << "remaining:\"" << sentence << '\"' << std::endl;
}