#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <chrono>  // Para medir el tiempo
#include <iomanip> // Para setprecision
#include "../dpllSolverDir/CNF.h"
#include "../dpllSolverDir/dpll.h"

using namespace std;
using namespace std::chrono;  // Para medir el tiempo

const int D = 3;
const int N = D*D;

int var(int r, int c, int v) {
    return (r-1)*N*N + (c-1)*N + (v-1) + 1;
}

void printSudoku(const string& sudoku) {
    for (int i = 0; i < N; i++) {
        if (i % D == 0 && i != 0) cout << "------+-------+------" << endl;
        for (int j = 0; j < N; j++) {
            if (j % D == 0 && j != 0) cout << "| ";
            cout << sudoku[i*N + j] << " ";
        }
        cout << endl;
    }

    // Solución en una línea
    cout << "\nFormato lineal:\n";
    for (char c : sudoku) {
        cout << c;
    }
    cout << endl;
}

shared_ptr<CNF> sudokuToCNF(const string& sudoku) {
    auto cnf = make_shared<CNF>();
    unordered_set<int> used_vars;

    // 1. Cada celda contiene al menos un número
    for (int r = 1; r <= N; r++) {
        for (int c = 1; c <= N; c++) {
            vector<Literal> clause;
            for (int v = 1; v <= N; v++) {
                int variable = var(r, c, v);
                clause.push_back({variable, true});
                used_vars.insert(variable);
            }
            cnf->clauses.push_back(clause);
        }
    }

    // 2. Ninguna celda contiene más de un número
    for (int r = 1; r <= N; r++) {
        for (int c = 1; c <= N; c++) {
            for (int v = 1; v <= N; v++) {
                for (int w = v+1; w <= N; w++) {
                    cnf->clauses.push_back({
                        {var(r, c, v), false},
                        {var(r, c, w), false}
                    });
                }
            }
        }
    }

    // 3. Cada número aparece exactamente una vez en cada fila
    for (int v = 1; v <= N; v++) {
        for (int r = 1; r <= N; r++) {
            for (int c1 = 1; c1 <= N; c1++) {
                for (int c2 = c1+1; c2 <= N; c2++) {
                    cnf->clauses.push_back({
                        {var(r, c1, v), false},
                        {var(r, c2, v), false}
                    });
                }
            }
        }
    }

    // 4. Cada número aparece exactamente una vez en cada columna
    for (int v = 1; v <= N; v++) {
        for (int c = 1; c <= N; c++) {
            for (int r1 = 1; r1 <= N; r1++) {
                for (int r2 = r1+1; r2 <= N; r2++) {
                    cnf->clauses.push_back({
                        {var(r1, c, v), false},
                        {var(r2, c, v), false}
                    });
                }
            }
        }
    }

    // 5. Cada número aparece exactamente una vez en cada bloque 3x3
    for (int v = 1; v <= N; v++) {
        for (int br = 0; br < D; br++) {
            for (int bc = 0; bc < D; bc++) {
                for (int pos1 = 0; pos1 < N; pos1++) {
                    int r1 = br*D + pos1/D + 1;
                    int c1 = bc*D + pos1%D + 1;
                    for (int pos2 = pos1+1; pos2 < N; pos2++) {
                        int r2 = br*D + pos2/D + 1;
                        int c2 = bc*D + pos2%D + 1;
                        cnf->clauses.push_back({
                            {var(r1, c1, v), false},
                            {var(r2, c2, v), false}
                        });
                    }
                }
            }
        }
    }

    // 6. Valores fijos iniciales
    for (int r = 1; r <= N; r++) {
        for (int c = 1; c <= N; c++) {
            char cell = sudoku[(r-1)*N + (c-1)];
            if (cell != '.') {
                int v = cell - '0';
                cnf->clauses.push_back({{var(r, c, v), true}});
            }
        }
    }

    cnf->variables.assign(used_vars.begin(), used_vars.end());
    cnf->variableCount = N*N*N;

    cout << "Variables generadas: " << cnf->variables.size() << endl;
    cout << "Clausulas generadas: " << cnf->clauses.size() << endl;

    return cnf;
}

string decodeSolution(const unordered_map<int, bool>& model) {
    string solution(N*N, '.');
    for (const auto& [var_num, val] : model) {
        if (val && var_num > 0) {
            int v = (var_num-1) % N + 1;
            int c = ((var_num-1) / N) % N + 1;
            int r = (var_num-1) / (N*N) + 1;
            solution[(r-1)*N + (c-1)] = '0' + v;
        }
    }
    return solution;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <sudoku_string>" << endl;
        return 1;
    }

    string sudoku = argv[1];
    if (sudoku.length() != N*N) {
        cerr << "Error: El Sudoku debe tener exactamente " << N*N << " caracteres" << endl;
        return 1;
    }

    cout << "Sudoku de entrada:" << endl;
    printSudoku(sudoku);

    auto cnf = sudokuToCNF(sudoku);
    
    // Iniciar medición de tiempo
    auto start = high_resolution_clock::now();
    
    bool hasSolution = dpll(cnf);
    
    // Finalizar medición de tiempo
    auto stop = high_resolution_clock::now();
    duration<double> duration = stop - start;
    
    if (hasSolution) {
        string solution = decodeSolution(cnf->model);
        cout << "\nSolucion encontrada:" << endl;
        printSudoku(solution);
    } else {
        cout << "\nNo se encontro solucion." << endl;
    }
    
    // Imprimir tiempo con 4 decimales
    cout << fixed << setprecision(4);
    cout << "\nTiempo de resolucion: " << duration.count() << " segundos" << endl;
    cout << scientific;  // Restaurar formato científico por defecto

    return 0;
}