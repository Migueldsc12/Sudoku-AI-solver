#ifndef DPLL_H
#define DPLL_H

#include "CNF.h"

bool dpll(const std::shared_ptr<CNF>& cnf);

#endif // DPLL_H