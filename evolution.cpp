// evolution.cpp
#include "evolution.h"

#include <algorithm>

#include "individual.h"
#include "type.h"
#include "random.h"
#include "mrmr.h"
#include "evaluation.h"
#include "crossover.h"

void evolution(const std::vector<std::vector<T>>& X, const std::vector<T>& y, std::vector<Individual>& population, size_t step, double p_scalar, double w, T xo)
{
    size_t n = population.size();
    size_t epoch = n / step;
    std::vector<double> cumulProbs(n);

    if (n % step != 0)
    {
        fprintf(stderr, "[Warning]: n = %zu, step = %zu\n", n, step);
    }

    // For the exponential ranking selection
    cumulProbs[0] = 1.0;
    for (size_t i = 1; i < n; ++i)
    {
        cumulProbs[i] = cumulProbs[i - 1] * p_scalar;
    }
    for (size_t i = 1; i < n; ++i)
    {
        cumulProbs[i] += cumulProbs[i - 1];
    }
    for (size_t i = 0; i < n; ++i)
    {
        cumulProbs[i] /= cumulProbs.back();
    }

    std::vector<FLOAT> target(y.size());
    for (size_t k = 0; k < y.size(); ++k)
    {
        target[k] = static_cast<FLOAT>(y[k]);
    }

    for (size_t i = 0; i < epoch; ++i)
    {
        std::vector<Individual> buffer(step + n);
        for (size_t j = 0; j < step; ++j)
        {
            if (random_state(10) == 0) // crossover
            {
                double select_value1 = static_cast<double>(random_state(2000000000)) / static_cast<double>(2000000000);
                size_t target_index1 = static_cast<size_t>(std::lower_bound(cumulProbs.begin(), cumulProbs.end(), select_value1) - cumulProbs.begin());
                double select_value2 = static_cast<double>(random_state(2000000000)) / static_cast<double>(2000000000);
                size_t target_index2 = static_cast<size_t>(std::lower_bound(cumulProbs.begin(), cumulProbs.end(), select_value2) - cumulProbs.begin());   

                buffer[j].root = crossover(population[target_index1].root, population[target_index2].root);
                
            }
            else
            {
                double select_value1 = static_cast<double>(random_state(2000000000)) / static_cast<double>(2000000000);
                size_t target_index1 = static_cast<size_t>(std::lower_bound(cumulProbs.begin(), cumulProbs.end(), select_value1) - cumulProbs.begin());

                buffer[j].root = subtree_mutation(population[target_index1].root);
                
            }
            
        }

        for (size_t j = 0; j < n; ++j)
        {
            buffer[step + j] = population[j];
        }

        evaluation(X, y, buffer, step);
        
        std::vector<MRMRInfo> mrmrRes = mrmrSolver(target, buffer, static_cast<int>(buffer.size()), w);

        for (size_t k = 0; k < population.size(); ++k)
        {
            population[k] = buffer[mrmrRes[k].index];
            population[k].root = population[k].root->deepcopy();
        }

        for (size_t k = 0; k < buffer.size(); ++k)
        {
            delete buffer[k].root;
        }

        std::sort(population.begin(), population.end(),  [](const Individual &lhs, const Individual &rhs) {
            return lhs.raw_fitness < rhs.raw_fitness;
        });

    }
    return;
}
