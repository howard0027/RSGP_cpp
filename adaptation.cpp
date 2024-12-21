// adaptation.cpp
#include "adaptation.h"

#include <cstdlib>
#include <cmath>
#include <vector>

#include "individual.h"

double adaptation(const std::vector<Individual>& plow, const std::vector<Individual>& pbase, const std::vector<Individual>& phigh, double w, double step
, long long ofLow, long long ofBase, long long ofHigh)
{
    long long bestLow = (1ll << 62), bestBase = (1ll << 62), bestHigh = (1ll << 62);
    T population_size = (T)plow.size();
    assert(population_size == (T)pbase.size() && population_size == (T)phigh.size());
    
    for (T i = 0; i < population_size; ++i)
    {
        bestLow = std::min(bestLow, plow[i].raw_fitness);
        bestBase = std::min(bestBase, pbase[i].raw_fitness);
        bestHigh = std::min(bestHigh, phigh[i].raw_fitness);
        if (i > 0)
        {
            assert(plow[i - 1].raw_fitness <= plow[i].raw_fitness);
            assert(pbase[i - 1].raw_fitness <= pbase[i].raw_fitness);
            assert(phigh[i - 1].raw_fitness <= phigh[i].raw_fitness);
        }
    }

    //ofLow = ofBase = ofHigh = std::min(ofLow, std::min(ofBase, ofHigh));
    
    double sumLow = (double)(ofLow - bestLow)/ (double)ofLow;
    double sumBase = (double)(ofBase - bestBase) / (double)ofBase;
    double sumHigh = (double)(ofHigh - bestHigh) / (double)ofHigh;
    sumLow *= -1.0; sumBase *= -1.0; sumHigh *= -1.0;
    //printf("Low = %lld, Base = %lld, High = %lld\n", sumLow, sumBase, sumHigh);
    printf("Low = %lf, Base = %lf, High = %lf\n", sumLow, sumBase, sumHigh);
    
    double margin = 1e-6, ratio = 1 + 1e-6;
    if (fabs(w - 0.9) <= 1e-6)
    {
        if (sumHigh < -margin && sumHigh + margin < sumBase && sumHigh + margin < sumLow)
        {
            return w;
        }
        if (sumLow < -margin && sumLow + margin < sumBase && sumLow + margin < sumHigh)
        {
            return w - step;
        }
        if (sumBase < -margin && sumBase + margin < sumLow && sumBase + margin < sumHigh)
        {
            return w;
        }
        return w;
    }
    if (fabs(w - 0.1) <= 1e-6)
    {
        if (sumHigh < -margin && sumHigh + margin < sumBase && sumHigh + margin < sumLow)
        {
            return w + step;
        }
        if (sumLow < -margin && sumLow + margin < sumBase && sumLow + margin < sumHigh)
        {
            return w;
        }
        if (sumBase < -margin && sumBase + margin < sumLow && sumBase + margin < sumHigh)
        {
            return w;
        }
        return w;
    }

    if (sumHigh < -margin && sumHigh + margin < sumLow && sumHigh + margin < sumBase)
    {
        return w + step;
    }
    if (sumLow < -margin && sumLow + margin < sumBase && sumLow + margin < sumHigh)
    {
        return w - step;
    }
    if (sumBase < -margin && sumBase + margin < sumLow && sumBase + margin < sumHigh)
    {
        return w;
    }

    return w;

}

