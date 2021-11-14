#pragma once

#include "LL1TableCreator.h"

class LL1Parser
{
private:
    /* data */
    LL1Table m_table;
    void _outputState();

public:
    LL1Parser(LL1Table table);
    void setLL1Table(LL1Table table);
    void parse(std::string sentence);
    ~LL1Parser();
};