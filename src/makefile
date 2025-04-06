# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2

# Remove command
RM = rm -f

# Targets
all: dpllSolver sudokuSolver

# DPLL Solver target
dpllSolver: dpllSolverDir/solver.cpp dpllSolverDir/CNF.cpp dpllSolverDir/dpll.cpp
	$(CXX) $(CXXFLAGS) -o dpllSolver dpllSolverDir/solver.cpp dpllSolverDir/CNF.cpp dpllSolverDir/dpll.cpp

# Sudoku Solver target
sudokuSolver: sudokuSolverDir/sudoku.cpp dpllSolverDir/CNF.cpp dpllSolverDir/dpll.cpp
	$(CXX) $(CXXFLAGS) -o sudokuSolver sudokuSolverDir/sudoku.cpp dpllSolverDir/CNF.cpp dpllSolverDir/dpll.cpp

# Clean target
clean:
	$(RM) dpllSolver sudokuSolver