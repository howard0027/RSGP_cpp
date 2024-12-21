#ifndef __RAND_H
#define __RAND_H

#include <random>
#include "type.h"
#include <cassert>


extern std::mt19937_64 rng;
extern std::uniform_int_distribution<T> dis;



T random_state(T lb, T ub);

T random_state(T ub);


#endif //__RAND_H