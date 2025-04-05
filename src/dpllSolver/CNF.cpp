#include "CNF.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_set>
#include <algorithm>
#include <iostream>

using namespace std;

shared_ptr<CNF> read_cnf(const string& filename) {
    auto cnf = make_shared<CNF>();
    ifstream file(filename);
    string line;
    unordered_set<int> variables;

    if (!file.is_open()) {
        throw runtime_error("Error al abrir el archivo: " + filename);
    }

    // Read the header line
    while (getline(file, line)) {

        // Skip comments
        if(line.empty() || line[0] == 'c') continue;

        // Check for the problem line. The line should start with 'p cnf'
        if (line[0] == 'p') {
            istringstream iss(line);
            string temp;
            iss >> temp >> temp >> cnf-> variableCount >> cnf->clauseCount;
            continue;
        }

        // Read clauses
        istringstream iss(line);
        vector<Literal> clause;
        int literal;

        while (iss >> literal && literal != 0) {
            Literal lit;
            lit.var = abs(literal);
            lit.sign = (literal > 0);
            clause.push_back(lit);
            variables.insert(lit.var);
        }
    
        if (!clause.empty()) {
            cnf->clauses.push_back(clause);
        }
    }
    cnf->variables.assign(variables.begin(), variables.end());
    sort(cnf->variables.begin(), cnf->variables.end());
    return cnf;
};

bool is_clause_satisfied(const vector<Literal>& clause,const unordered_map<int, bool>& model) {
    for (const auto& lit : clause) {
        auto it = model.find(lit.var);
        if (it != model.end() && it->second == lit.sign) {
        return true;
        }
    }
    return false;
}

bool is_clause_false(const vector<Literal>& clause,const unordered_map<int, bool>& model) {
    for (const auto& lit : clause) {
        auto it = model.find(lit.var);
        if (it == model.end() || it->second == lit.sign) {
        return false;
        }
    }
    return true;
}

