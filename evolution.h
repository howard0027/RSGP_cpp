// adaptation.h
#ifndef __EVOLUTION_H
#define __EVOLUTION_H

#include <vector>

#include "type.h"
#include "individual.h"

void evolution(const std::vector<std::vector<T>>& X, const std::vector<T>& y, std::vector<Individual>& population, size_t step, double p_scalar, double w, T xo);

#endif // __EVOLUTION_H