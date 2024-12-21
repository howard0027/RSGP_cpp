

#include <vector>
#include <cstdint>
#include <string>
#include <stack>
#include <cassert>
#include <unordered_map>
#include <algorithm>
#include <random>
#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "functions.h"
#include "grammar.h"
#include "type.h"
#include "container.h"
#include "ast.h"
#include "data.h"
#include "fitness.h"
#include "crossover.h"
#include "random.h"
#include "parm.h"
#include "progsimp.h"
#include "evaluation.h"
#include "evolution.h"
#include "adaptation.h"

#include "mrmr.h"
#include "individual.h"

T CURTASK;
T DEBUGFLAG;

std::mt19937_64 rng;
std::uniform_int_distribution<T> dis(0, 2147483647);

ASTNode* run(const std::vector<std::vector<T>>& X_train, const std::vector<T>& y_train, const std::vector<std::vector<T>>& X_test, const std::vector<T>& y_test, int populationSize, int generations)
{
    std::vector<Individual> population_pool(populationSize * 3);
    std::vector<Individual> population_low(populationSize);
    std::vector<Individual> population_base(populationSize);
    std::vector<Individual> population_high(populationSize);

    double step = 0.1;
    double weight = 0.9;

    

    time_t st, ed;
    int gen;
    ASTNode *trainBestInd = nullptr;

    long long oldFitnessLow = (1ll << 62), oldFitnessBase = (1ll << 62), oldFitnessHigh = (1ll << 62);

    for (gen = 0; gen < generations; ++gen)
    {
        st = clock();        

        if (gen == 0)
        {
            for (size_t i = 0; i < populationSize; ++i)
            {
                population_low[i].root = _real(nullptr, random_state(3, 6), GROW);
                population_base[i].root = _real(nullptr, random_state(3, 6), GROW);
                population_high[i].root = _real(nullptr, random_state(3, 6), GROW);
            }
            
            evaluation(X_train, y_train, population_low, -1);
            evaluation(X_train, y_train, population_base, -1);
            evaluation(X_train, y_train, population_high, -1);
            
            std::sort(population_low.begin(), population_low.end(), [](const Individual &lhs, const Individual &rhs) {
                return lhs.raw_fitness < rhs.raw_fitness;
            });
            std::sort(population_base.begin(), population_base.end(), [](const Individual &lhs, const Individual &rhs) {
                return lhs.raw_fitness < rhs.raw_fitness;
            });
            std::sort(population_high.begin(), population_high.end(), [](const Individual &lhs, const Individual &rhs) {
                return lhs.raw_fitness < rhs.raw_fitness;
            });
            
            oldFitnessLow = population_low[0].raw_fitness;
            oldFitnessBase = population_base[0].raw_fitness;
            oldFitnessHigh = population_high[0].raw_fitness;

        }
        else
        {
            
            evolution(X_train, y_train, population_low, populationSize / 2, 0.9993, weight - step, 0);
            evolution(X_train, y_train, population_base, populationSize / 2, 0.9993, weight, 0);
            evolution(X_train, y_train, population_high, populationSize / 2, 0.9993, weight + step, 0);
            // evolution(X_train, y_train, population_low, populationSize / 2, 0.9993, 1.0, 0);
            // evolution(X_train, y_train, population_base, populationSize / 2, 0.9993, 1.0, 0);
            // evolution(X_train, y_train, population_high, populationSize / 2, 0.9993, 1.0, 0);
            
            if (gen % 5 == 0)
            {
                weight = adaptation(population_low, population_base, population_high, weight, step
                , oldFitnessLow, oldFitnessBase, oldFitnessHigh);

                
                for (size_t i = 0; i < populationSize; ++i)
                {
                    population_pool[i * 3] = population_low[i];
                    population_pool[i * 3 + 1] = population_base[i];
                    population_pool[i * 3 + 2] = population_high[i];
                }

                std::random_shuffle(population_pool.begin(), population_pool.end());
                    
                for (size_t i = 0; i < populationSize; ++i)
                {
                    population_low[i] = population_pool[i * 3];
                    population_base[i] = population_pool[i * 3 + 1];
                    population_high[i] = population_pool[i * 3 + 2];
                }
                    
                std::sort(population_low.begin(), population_low.end(), [](const Individual &lhs, const Individual &rhs) {
                    return lhs.raw_fitness < rhs.raw_fitness;
                });

                std::sort(population_base.begin(), population_base.end(), [](const Individual &lhs, const Individual &rhs) {
                    return lhs.raw_fitness < rhs.raw_fitness;
                });

                std::sort(population_high.begin(), population_high.end(), [](const Individual &lhs, const Individual &rhs) {
                    return lhs.raw_fitness < rhs.raw_fitness;
                });

                oldFitnessLow = population_low[0].raw_fitness;
                oldFitnessBase = population_base[0].raw_fitness;
                oldFitnessHigh = population_high[0].raw_fitness;
                    
                for (size_t i = 0; i < populationSize; ++i)
                {
                    oldFitnessLow = std::min(oldFitnessLow, population_low[i].raw_fitness);
                    oldFitnessBase = std::min(oldFitnessBase, population_base[i].raw_fitness);
                    oldFitnessHigh = std::min(oldFitnessHigh, population_high[i].raw_fitness);
                }
                long long optimal = std::min(std::min(oldFitnessLow, oldFitnessBase), oldFitnessHigh);
                oldFitnessLow = oldFitnessBase = oldFitnessHigh = optimal;
            }
        }
        
        ed = clock();

        long long bestFitness = std::min(std::min(population_low[0].raw_fitness, population_base[0].raw_fitness), population_high[0].raw_fitness);
        T bestDepth, bestProgramSize, bestExecutionTime;
        std::string bestStr;
        ASTNode* bestAst;

        if (population_low[0].raw_fitness == bestFitness)
        {
            bestDepth = population_low[0].root->depth();
            bestProgramSize = population_low[0].root->program_size();
            bestExecutionTime = population_low[0].execution_time;
            bestStr = population_low[0].root->print();
            bestAst = population_low[0].root;
        }
        else if (population_base[0].raw_fitness == bestFitness)
        {
            bestDepth = population_base[0].root->depth();
            bestProgramSize = population_base[0].root->program_size();
            bestExecutionTime = population_base[0].execution_time;
            bestStr = population_base[0].root->print();
            bestAst = population_base[0].root;
        }
        else if (population_high[0].raw_fitness == bestFitness)
        {
            bestDepth = population_high[0].root->depth();
            bestProgramSize = population_high[0].root->program_size();
            bestExecutionTime = population_high[0].execution_time;
            bestStr = population_high[0].root->print();
            bestAst = population_high[0].root;
        }

        printf("Gen %4d: %9lld %5.2f, best depth = %d, size = %d, exe = %d\n", gen, bestFitness, (float)(ed - st) / CLOCKS_PER_SEC, bestDepth, bestProgramSize, bestExecutionTime);
        printf("%s %lld\n", bestStr.c_str(), bestFitness);
        printf("mRMR weight = %lf\n", weight);

        trainBestInd = bestAst;
        if (bestFitness == 0)
        {
            printf("Evaluate the absolute error on the train data...\n");
            maeTest(X_train, y_train, bestAst);
            break;
        }
        
    }

    trainBestInd = trainBestInd->deepcopy();

    for (int i = 0; i < populationSize; ++i)
    {   
        delete population_low[i].root;
        delete population_base[i].root;
        delete population_high[i].root;
    }

    return trainBestInd;
}

int main(int argc, char *argv[])
{
    T population_size = 332;
    T generations = 500;
    
    // initialize global variables
    CURTASK = MNP;
    DEBUGFLAG = 0;

    

    std::vector<std::vector<T>> X_train, X_test;
    std::vector<T> y_train, y_test;
    
    std::string task, folder;
    T seed;

    T train_n = 100, test_n = 1000;
    parm(argc, (const char**)argv, task, seed, folder, train_n, test_n);
    //printf("train_n = %d, test_n = %d\n", train_n, test_n);

    srand(seed);
    rng.seed(seed);

    
    load_data_json(X_train, y_train, X_test, y_test, task, train_n, test_n, folder);
    
    ASTNode* trainBestAst = run(X_train, y_train, X_test, y_test, population_size, generations);

    long long bestFitness;
    ASTNode *bestAst = progsimp(X_train, y_train, trainBestAst, bestFitness)->deepcopy();
    
    std::vector<Individual> bestInd(1);
    bestInd[0].root = bestAst;
    
    evaluation(X_test, y_test, bestInd, 1);
    printf("Gen test: %9lld %5.2f, best depth = %d, size = %d, exe = %d\n", bestInd[0].raw_fitness, 0.0, bestInd[0].root->depth(), bestInd[0].root->program_size(), bestInd[0].execution_time);
    printf("%s %lld\n", bestInd[0].root->print().c_str(), bestInd[0].raw_fitness);
        
    //printf("%s %d\n", population[0].root->print().c_str(), population[0].raw_fitness);
    printf("Best program: %s\n", bestInd[0].root->print().c_str());

    printf("Evaluate the absolute error on the test data...\n");
    maeTest(X_test, y_test, bestAst);
    
    delete trainBestAst;
    delete bestAst;

    return 0;
}
