#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_set>
#include <algorithm>

using namespace std;

struct CNF {
    int variableCount;
    int clauseCount;
    vector<vector<int>> clauses;
    vector<int> variables;
};

CNF read_cnf(const string& filename) {
    CNF cnf;
    ifstream file(filename);
    string line;
    unordered_set<int> variables;

    if (!file.is_open()) {
        cerr << "Error al abrir el archivo: " << filename << endl;
        exit(1);
    }

    // Read the header line
    while (getline(file, line)) {

        // Skip comments
        if(line.empty() || line[0] == 'c') continue;

        // Check for the problem line. The line should start with 'p cnf'
        if (line[0] == 'p') {
            istringstream iss(line);
            string temp;
            iss >> temp >> temp >> cnf.variableCount >> cnf.clauseCount;
            continue;
        }

        // Read clauses
        istringstream iss(line);
        vector<int> clause;
        int literal;

        while (iss >> literal && literal != 0) {
            clause.push_back(literal);
            if (find(variables.begin(), variables.end(), abs(literal)) == variables.end()) {
                variables.insert(abs(literal));
            }
        }
        if (!clause.empty()) {
            cnf.clauses.push_back(clause);
        }
    }
    cnf.variables.assign(variables.begin(), variables.end());
    sort(cnf.variables.begin(), cnf.variables.end());
    return cnf;
};

void print_cnf(const CNF& cnf) {
    cout << "Variables: ";
    for (int var : cnf.variables) {
        cout << var << " ";
    }
    cout << "\nClausulas:\n";
    
    for (const auto& clause : cnf.clauses) {
        for (int lit : clause) {
            cout << lit << " ";
        }
        cout << "0\n";
    }
}

// Pa probar
// int main() {
//     string filename = "prueba.cnf";
//     CNF cnf = read_cnf(filename);
    
//     cout << "Archivo CNF leido: " << filename << endl;
//     cout << "Numero de variables: " << cnf.variableCount << endl;
//     cout << "Numero de clausulas: " << cnf.clauseCount << endl;
    
    
//     return 0;
// }