// evaluation.cpp

#include "evaluation.h"

#include <vector>

#include "individual.h"
#include "type.h"
#include "fitness.h"

void evaluation(const std::vector<std::vector<T>>& X, const std::vector<T>& y, std::vector<Individual>& population, long sz)
{
    if (sz == -1)
    {
        sz = (int)population.size();
    }
    
    #pragma omp parallel for num_threads(2)
    for (long i = 0; i < sz; ++i)
    {
        std::pair<long long, long long> res = _count(X, y, population[i].root, population[i].details);
        population[i].raw_fitness = res.first;
        population[i].execution_time = res.second;
        population[i].depth = population[i].root->depth();
        population[i].program_size = population[i].root->program_size();

    }
    return;
}

