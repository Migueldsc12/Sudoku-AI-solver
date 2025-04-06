#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>

using namespace std;

const int D = 3; // Dimensión de subcuadrícula
const int N = D*D; // Dimensión del tablero (9 para Sudoku estándar)

// Función para calcular el número de variable DIMACS
int var(int r, int c, int v) {
    return (r-1)*N*N + (c-1)*N + (v-1) + 1;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <archivo_sudoku.txt>" << endl;
        return 1;
    }

    // Leer el archivo de entrada
    ifstream input(argv[1]);
    if (!input.is_open()) {
        cerr << "Error al abrir el archivo: " << argv[1] << endl;
        return 1;
    }

    vector<string> clues;
    string line;
    
    while (getline(input, line)) {
        if (line.empty()) continue;
        assert(line.length() == N*N && "El Sudoku debe tener exactamente N² caracteres");
        for (int i = 0; i < N; ++i) {
            clues.push_back(line.substr(i*N, N));
        }break;
    }

    vector<vector<int>> clauses;

    // 1. Cada celda contiene al menos un número
    for (int r = 1; r <= N; r++) {
        for (int c = 1; c <= N; c++) {
            vector<int> clause;
            for (int v = 1; v <= N; v++) {
                clause.push_back(var(r, c, v));
            }
            clauses.push_back(clause);
        }
    }

    // 2. Cada celda contiene como máximo un número
    for (int r = 1; r <= N; r++) {
        for (int c = 1; c <= N; c++) {
            for (int v = 1; v <= N; v++) {
                for (int w = v+1; w <= N; w++) {
                    clauses.push_back({-var(r, c, v), -var(r, c, w)});
                }
            }
        }
    }

    // 3. Cada número aparece exactamente una vez en cada fila
    for (int v = 1; v <= N; v++) {
        for (int r = 1; r <= N; r++) {
            vector<int> clause;
            for (int c = 1; c <= N; c++) {
                clause.push_back(var(r, c, v));
            }
            clauses.push_back(clause);
        }
    }

    // 4. Cada número aparece exactamente una vez en cada columna
    for (int v = 1; v <= N; v++) {
        for (int c = 1; c <= N; c++) {
            vector<int> clause;
            for (int r = 1; r <= N; r++) {
                clause.push_back(var(r, c, v));
            }
            clauses.push_back(clause);
        }
    }

    // 5. Cada número aparece exactamente una vez en cada subcuadrícula
    for (int v = 1; v <= N; v++) {
        for (int sr = 0; sr < D; sr++) {
            for (int sc = 0; sc < D; sc++) {
                vector<int> clause;
                for (int rd = 1; rd <= D; rd++) {
                    for (int cd = 1; cd <= D; cd++) {
                        clause.push_back(var(sr*D + rd, sc*D + cd, v));
                    }
                }
                clauses.push_back(clause);
            }
        }
    }

    // 6. Respetar las pistas iniciales
    for (int r = 1; r <= N; r++) {
        for (int c = 1; c <= N; c++) {
            char cell = clues[r-1][c-1];
            if (cell != '.') {
                int v = cell - '0';
                assert(v >= 1 && v <= 9);
                clauses.push_back({var(r, c, v)});
            }
        }
    }

    ofstream output("sudo.cnf");
    if (!output.is_open()) {
        cerr << "Error al crear archivo de salida" << endl;
        return 1;
    }

    output << "p cnf " << N*N*N << " " << clauses.size() << endl;
    for (const auto& clause : clauses) {
        for (int lit : clause) {
            output << lit << " ";
        }
        output << "0" << endl;
    }
    output.close();

    return 0;
}