#include "CNF.h"
#include "dpll.h"
#include <optional>
#include <set>
#include <iostream>
#include <chrono>
#include <fstream>

using namespace std;


struct PureSymbolResult {
    unordered_map<int, int> positiveCount;
    unordered_map<int, int> negativeCount;
    optional<pair<int, bool>> pureSymbol;
};

/* Find a pure symbol in the CNF
A pure symbol is a variable that appears with only one sign in the CNF*/
PureSymbolResult find_pure_symbol(const shared_ptr<CNF>& cnf) {
    PureSymbolResult result;
    
    for (const auto& clause : cnf->clauses) {
        if (is_clause_satisfied(clause, cnf->model)) continue; // Check if the clause is satisfied by the current model
        
        // Count the occurrences of each literal in the clause
        for (const auto& lit : clause) {
            if (cnf->model.find(lit.var) == cnf->model.end()) {
                if (lit.sign) result.positiveCount[lit.var]++;
                else result.negativeCount[lit.var]++;
            }
        }
    }
    
    for (int var : cnf->variables) {
        if (cnf->model.find(var) != cnf->model.end()) continue;
        
        // If the variable appears only with positive sign
        if (result.positiveCount[var] > 0 && result.negativeCount[var] == 0) {
            result.pureSymbol = make_pair(var, true);
            return result;
        }
        // If the variable appears only with negative sign
        if (result.negativeCount[var] > 0 && result.positiveCount[var] == 0) {
            result.pureSymbol = make_pair(var, false);
            return result;
        }
    }
    
    return result;
}

/* Find a unit clause in the CNF
A unit clause is a clause with exactly one unassigned literal
If a unit clause is found, return the variable and its value
If no unit clause is found, return nullopt*/

optional<pair<int, bool>> find_unit_clause(const shared_ptr<CNF>& cnf) {
    for (const auto& clause : cnf->clauses) {
        if (is_clause_satisfied(clause, cnf->model)) continue;
        
        int unassigned_count = 0;
        Literal unit_lit;

        // Count unassigned literals in the clause
        for (const auto& lit : clause) {
            if (cnf->model.find(lit.var) == cnf->model.end()) {
                unassigned_count++;
                unit_lit = lit;
                if (unassigned_count > 1) break; // No need to check further
            }
        }
        // If there is exactly one unassigned literal, return it
        if (unassigned_count == 1) {
            return make_pair(unit_lit.var, unit_lit.sign);
        }
    }
    
    return nullopt;// No unit clause found
}


bool dpll(const shared_ptr<CNF>& cnf) {

    // Check if all clauses are satisfied. If so, return true
    bool all_satisfied = true;
    for (const auto& clause : cnf->clauses) {
        if (!is_clause_satisfied(clause, cnf->model)) {
            all_satisfied = false;  // No clause is satisfied
            break;
        }
    }
    if (all_satisfied) return true; // All clauses are satisfied
    
    // Check if any clause is false. If so, return false
    for (const auto& clause : cnf->clauses) {
        if (is_clause_false(clause, cnf->model)) {
            return false;
        }
    }
    
    /*  Find pure symbols
    If a pure symbol is found, assign it and continue
    Otherwise, continue with unit propagation*/
    auto pure_result = find_pure_symbol(cnf);
    if (pure_result.pureSymbol) {
        auto [var, value] = pure_result.pureSymbol.value();
        auto new_cnf = make_shared<CNF>(*cnf); 
        new_cnf->model[var] = value;

        if (dpll(new_cnf)) {
            cnf->model = new_cnf->model;  // If satisfiable, copy the model
            return true;
        }
        return false;
    }
    
    // Find unit clauses
    auto unit_clause = find_unit_clause(cnf);
    if (unit_clause) {
        auto [var, value] = unit_clause.value();
        auto new_cnf = make_shared<CNF>(*cnf);
        new_cnf->model[var] = value;
        if (dpll(new_cnf)) {
            cnf->model = new_cnf->model;  // If satisfiable, copy the model
            return true;
        }
        return false;
    }
    
    // If no pure symbols or unit clauses are found, choose a variable to assign
    int var = -1;
    for (int v : cnf->variables) {
        if (cnf->model.find(v) == cnf->model.end()) {
            var = v;
            break;
        }
    }
    if (var == -1) return false;
    
    // try assigning true to the variable
    auto cnf_true = make_shared<CNF>(*cnf);
    cnf_true->model[var] = true;
    if (dpll(cnf_true)) {
        cnf->model = cnf_true->model;
        return true;
    }
    
    // try assigning false to the variable
    auto cnf_false = make_shared<CNF>(*cnf);
    cnf_false->model[var] = false;
    if (dpll(cnf_false)) {
        cnf->model = cnf_false->model;
        return true;
    }
    
    return false;
}

void solve(const string& filename) {
    auto cnf = read_cnf(filename);
    bool result = dpll(cnf);
    
    if (result) {
        cout << "SATISFIABLE" << endl;
    } else {
        cout << "INSATISFIABLE" << endl;
    }
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <archivo.cnf>" << endl;
        cerr << "Ejemplo: " << argv[0] << " formula.cnf" << endl;
        return 1;
    }

    string filename = argv[1];
    
    ifstream test_file(filename);
    if (!test_file.good()) {
        cerr << "Error: No se puede abrir el archivo '" << filename << "'" << endl;
        return 1;
    }
    test_file.close();

    auto start = std::chrono::high_resolution_clock::now();
    solve(filename);
    auto stop = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = stop - start;
    std::cout << "Tiempo de ejecucion: " << duration.count() << " s\n";

    return 0;

}
