#include "dpll.h"
#include "CNF.h"
#include <iostream>
#include <chrono>
#include <fstream>

using namespace std;

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
    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(stop - start);
    cout << "Tiempo de ejecucion: " << duration.count() << " s\n";
    return 0;
}