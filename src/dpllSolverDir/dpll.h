#ifndef DPLL_H
#define DPLL_H

#include "CNF.h"

bool dpll(const std::shared_ptr<CNF>& cnf);
void solve(const std::string& filename);

#endif // DPLL_H