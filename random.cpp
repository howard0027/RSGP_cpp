
#include "random.h"

#include <random>
#include "type.h"
#include <cassert>


T random_state(T lb, T ub)
{
    assert(lb < ub);
    return dis(rng) % (ub - lb) + lb;
}

T random_state(T ub)
{
    assert(ub > 0);
    return dis(rng) % ub;
}

