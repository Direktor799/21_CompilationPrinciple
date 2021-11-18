#include "LRParser.h"

const int spaceWidth = 30;

LRParser::LRParser(const LRTable &table, const std::vector<Production> &productions)
    : m_table(table), m_productions(productions) {
    for (auto &production : m_productions) {
        m_nonTerminatorSet.insert(production.first);
    }
    for (auto &production : m_productions) {
        for (auto &symbol : production.second) {
            if (m_nonTerminatorSet.find(symbol) == m_nonTerminatorSet.end()) {
                m_terminatorSet.insert(symbol);
            }
        }
    }
}

void LRParser::_error(const std::string &sentence) {
    std::cout << std::endl << std::endl << "Can not parse this sentence, ";
    std::cout << "remaining:\"" << sentence << '\"' << std::endl;
}

Symbol LRParser::_getSymbol(std::string &sentence) {
    Symbol a;
    while (m_nonTerminatorSet.find(a) == m_nonTerminatorSet.end() &&
           m_terminatorSet.find(a) == m_terminatorSet.end() && !sentence.empty()) {
        a += sentence.front();
        sentence = sentence.substr(1);
    }
    return a;
}

bool LRParser::parse(std::string sentence) {
    sentence += "$";
    std::vector<State> stateStack;
    std::vector<Symbol> symbolStack;
    stateStack.push_back(0);
    symbolStack.push_back("-");
    Symbol a = _getSymbol(sentence);

    std::cout.setf(std::ios_base::left);
    std::cout << std::endl
              << std::setw(spaceWidth) << "Stack" << std::setw(spaceWidth) << "Sentence"
              << "Production" << std::endl;
    auto _outputState = [&]() {
        std::string content;
        for (int i = 0; i < symbolStack.size(); i++) {
            content += std::to_string(stateStack[i]);
            content += symbolStack[i];
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

    while (true) {
        _outputState();
        const State &S = stateStack.back();
        std::string &action = m_table[S][a];
        if (m_table[S].find(a) == m_table[S].end()) {
            _error(a + sentence);
            return false;
        }
        if (action.front() == 's') {
            stateStack.push_back(std::stoi(action.substr(1)));
            symbolStack.push_back(a);
            a = _getSymbol(sentence);
        } else if (action.front() == 'r') {
            const Production &production = m_productions[std::stoi(action.substr(1))];
            for (int i = 0; i < production.second.size(); i++) {
                stateStack.pop_back();
                symbolStack.pop_back();
            }
            const State &curState = stateStack.back();
            stateStack.push_back(std::stoi(m_table[curState][production.first]));
            symbolStack.push_back(production.first);
            _outputProduction(production);
        } else {
            std::cout << std::endl << "Accepted!" << std::endl;
            return true;
        }
        std::cout << std::endl;
    }
}
