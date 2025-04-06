# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2

# Remove command
RM = rm -f

# Targets
all: dpllSolver sudokuSolver

# DPLL Solver target
dpllSolver: dpllSolver/solver.cpp dpllSolver/CNF.cpp dpllSolver/dpll.cpp
	$(CXX) $(CXXFLAGS) -o dpllSolver dpllSolver/solver.cpp dpllSolver/CNF.cpp dpllSolver/dpll.cpp

# Sudoku Solver target
sudokuSolver: sudokuSolverDir/sudoku.cpp dpllSolver/CNF.cpp dpllSolver/dpll.cpp
	$(CXX) $(CXXFLAGS) -o sudokuSolver sudokuSolverDir/sudoku.cpp dpllSolver/CNF.cpp dpllSolver/dpll.cpp

# Clean target
clean:
	$(RM) dpllSolver sudokuSolver