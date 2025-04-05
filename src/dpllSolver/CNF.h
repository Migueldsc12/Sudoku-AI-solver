#ifndef CNF_H
#define CNF_H

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

struct Literal {
    int var;
    bool sign;
};

struct CNF {
    int variableCount;
    int clauseCount;
    std::vector<std::vector<Literal>> clauses;
    std::vector<int> variables;

    // solver state dpll
    std::unordered_map<int, bool> model;    // model of the CNF
    
    CNF() : variableCount(0), clauseCount(0) {};
};
std::shared_ptr<CNF> read_cnf(const std::string& filename);

bool is_clause_satisfied(const std::vector<Literal>& clause, 
                        const std::unordered_map<int, bool>& model);
bool is_clause_false(const std::vector<Literal>& clause,
                    const std::unordered_map<int, bool>& model);


#endif