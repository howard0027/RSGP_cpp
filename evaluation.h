// evaluation.h
#ifndef __EVALUATION_H
#define __EVALUATION_H

#include <vector>

#include "type.h"
#include "individual.h"

void evaluation(const std::vector<std::vector<T>>& X, const std::vector<T>& y, std::vector<Individual>& population, long sz = -1);

#endif //__EVALUATION_H

