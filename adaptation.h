// adaptation.h
#ifndef __ADAPTATION_H
#define __ADAPTATION_H

#include <vector>

#include "individual.h"

double adaptation(const std::vector<Individual>& plow, const std::vector<Individual>& pbase, const std::vector<Individual>& phigh, double w, double step
, long long ofLow, long long ofBase, long long ofHigh);

#endif // __ADAPTATION_H