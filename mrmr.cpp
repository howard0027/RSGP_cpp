
#include "mrmr.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <set>
#include <unordered_set>
#include <utility>

#include "individual.h"
#include "type.h"

MRMRInfo::MRMRInfo() {}
MRMRInfo::MRMRInfo(long index, FLOAT score) : index(index), score(score) {}

bool MRMRData::operator < (const MRMRData& rhs) const
{
    FLOAT r0 = this->getScore(), r1 = rhs.getScore();
    if (fabs(r0 - r1) >= static_cast<FLOAT>(1e-6)) return r0 > r1;
    return rank < rhs.rank;
}

FLOAT MRMRData::getScore() const
{
    FLOAT r = this->relevance;
    if (this->redundancy_cnt > 0)
    {
        r -= wr * this->redundancy_sum / static_cast<FLOAT>(this->redundancy_cnt);
    }
    return r;
}

DataTable * makeDataTable(const std::vector<FLOAT>& target, const std::vector<std::vector<FLOAT>>& outputs)
{
    size_t populationSize = outputs.size(), num_of_datapoint = outputs[0].size();
    DataTable *myData = new DataTable;

    assert(target.size() == num_of_datapoint);

    myData->nsample = static_cast<long>(num_of_datapoint);
    myData->nvar = static_cast<long>(populationSize + 1);

    myData->data = new FLOAT[myData->nsample * myData->nvar];

    for (long i = 0; i < myData->nsample; ++i)
    {
        myData->data[i * myData->nvar] = static_cast<FLOAT>(target[i]);
    }

    for (long j = 0; j < static_cast<long>(populationSize); ++j)
    {
        for (long i = 0; i < static_cast<long>(num_of_datapoint); ++i)
        {
            myData->data[i * myData->nvar + j + 1] = static_cast<FLOAT>(outputs[j][i]);
        }
    }

    myData->buildData2d ();

    return myData;
}

DataTable::DataTable ()
{
    data = NULL;
    data2d = NULL;
    nsample = 0;
    nvar = 0;
    return;
}

DataTable::~DataTable ()
{
    if (data)
    {
        delete[]data;
        data = NULL;
    }
    if (data2d)
    {
        destroyData2d ();
    }
    nsample = nvar = 0;
    return;
}

int DataTable::buildData2d ()
{
    if (!data)
    {
        fprintf (stderr, "The data is not ready yet: data point is NULL");
    }

    if (data2d)
        destroyData2d ();
  
    if (nsample <= 0 || nvar <= 0)
    {
        fprintf (stderr, "The data is not ready yet: nsample=%d nvar=%d", nsample, nvar);
        return 0;
    }
    data2d = new FLOAT *[nsample];
    if (!data2d)
    {
        fprintf (stderr, "Line %d: Fail to allocate memory.\n", __LINE__);
        return 0;
    }

    else
    {
        for (long i = 0; i < nsample; i++)
        {
            data2d[i] = data + i * nvar;
        }
    }
    return 1;
}
  
void DataTable::destroyData2d ()
{
    if (data2d)
    {
        delete[]data2d;
        data2d = NULL;
    }
}


FLOAT calPearsonFast(DataTable * myData, long v1, long v2, FLOAT *v1data, FLOAT *v2data, FLOAT down1, FLOAT down2, FLOAT down1sq, FLOAT down2sq)
{
    FLOAT r = -1;		//initialized as an illegal value

    if (!myData || !myData->data || !myData->data2d)
    {
        fprintf (stderr, "Line %d: The input data is NULL.\n", __LINE__);
        exit (1);
        return r;
    }

    long nsample = myData->nsample;
    long nvar = myData->nvar;

    if (v1 >= nvar || v2 >= nvar || v1 < 0 || v2 < 0)
    {
        fprintf (stderr, "Line %d: The input variable indexes are invalid (out of range).\n", __LINE__);
        exit (1);
        return r;
    }

    long i, j;

    if (!v1data || !v2data)
    {
        fprintf (stderr, "Line %d: v1 or v2 are nullptr.\n", __LINE__);
        exit (1);
        return r;
    }

    if (down1 < -(1e-6) || down2 < -(1e-6))
    {
        return 0.0;
    }

    FLOAT down = down1sq * down2sq;

    if (fabs(down) < 1e-6 )
    {
        return 0.0;
    }

    FLOAT up = 0.0;
    for (int i = 0; i < nsample; ++i)
    {
        up += v1data[i] * v2data[i];
    }

    r = up / down;
    
    return fabs(r);
}

std::vector<MRMRInfo>  mRMRSelection (DataTable * myData, long nfea, FLOAT wr)
{
    if (!myData || !myData->data || !myData->data2d)
    {
        fprintf (stderr, "Line %d: The input data is NULL.\n", __LINE__);
        exit (1);
        return std::vector<MRMRInfo> {};
    }

    if (nfea < 0)
    {
        fprintf (stderr, "Line %d: The input data nfea is negative.\n", __LINE__);
        exit (1);
        return std::vector<MRMRInfo> {};
    }

    if (myData->nvar < 0)
    {
        fprintf (stderr, "Line %d: The input data nvar is negative.\n", __LINE__);
        exit (1);
        return std::vector<MRMRInfo> {};
    }

    
    long poolUseFeaLen = myData->nvar - 1;

    if (nfea > poolUseFeaLen)
    {
        nfea = poolUseFeaLen;
    }
  
    long i, j;

    //determine the pool
  
    FLOAT **rtable = new FLOAT*[myData->nvar];
    for (int i = 0; i < myData->nvar; ++i)
    {
        rtable[i] = new FLOAT[myData->nvar];
    }
  
    FLOAT **v_data = new FLOAT*[myData->nvar];
    for (int i = 0; i < myData->nvar; ++i)
    {
        v_data[i] = new FLOAT[myData->nsample];
        for (int j = 0; j < myData->nsample; ++j)
        {
            v_data[i][j] = myData->data2d[j][i];
        }
    }
    FLOAT *avg = new FLOAT[myData->nvar];
    FLOAT *down = new FLOAT[myData->nvar];
    FLOAT *downsq = new FLOAT[myData->nvar];

    // Can be parallize
    for (int i = 0; i < myData->nvar; ++i)
    {
        avg[i] = 0.0;
        for (int j = 0; j < myData->nsample; ++j)
        {
            avg[i] += static_cast<FLOAT>(myData->data2d[j][i]);
        }
        avg[i] /= static_cast<FLOAT>(myData->nsample);

        down[i] = 0.0;
        for (int j = 0; j < myData->nsample; ++j)
        {
            v_data[i][j] -= avg[i];
            down[i] += pow(static_cast<FLOAT>(myData->data2d[j][i]) - avg[i], 2.0);
        }
        downsq[i] = sqrt(down[i]);
    }

    // Can be parallize
    for (i = 0; i < myData->nvar; ++i)
    {
        rtable[i][i] = 1.0;
        for (j = i + 1; j < myData->nvar; ++j)
        {
            rtable[i][j] = rtable[j][i] = calPearsonFast(myData, i, j, v_data[i], v_data[j], down[i], down[j], downsq[i], downsq[j]); 
        }
    }

    for (int i = 0; i < myData->nvar; ++i) delete [] v_data[i];
    delete [] v_data;
    delete [] avg;
    delete [] down;
    delete [] downsq;

    std::vector<long> pers(myData->nvar, 0);
    for (i = 1; i < myData->nvar; ++i)
    {
        pers[i] = i;
    }

    std::random_shuffle(pers.begin() + 1, pers.end());
  
    /*
    // Old version begin  
    std::set<MRMRData>::iterator *dict = new std::set<MRMRData>::iterator[myData->nvar];
    std::set<MRMRData> st;
    std::vector<MRMRInfo> mrmrs(myData->nvar - 1);
    long usedIndCounter = 0;
    std::unordered_set<long> notUsedInd;


    for (i = 1; i < myData->nvar; ++i)
    {
        notUsedInd.insert(i);
    }

    // initialize
    for (i = 1; i < myData->nvar; ++i)
    {
        dict[i] = st.insert(MRMRData(rtable[0][i], 0.0, 0, i, wr, pers[i])).first;
    }
  
  
  
    // kernel
    while (st.size())
    {
        MRMRData u = *st.begin();
        st.erase(st.begin());
        mrmrs[usedIndCounter] = MRMRInfo(u.fidx, u.getScore());
        ++usedIndCounter;
        notUsedInd.erase(notUsedInd.find(u.fidx));
        
        for (const long& ridx : notUsedInd)
        {
            std::set<MRMRData>::iterator it = dict[ridx];
            MRMRData v = *it;
            v.redundancy_sum += rtable[u.fidx][ridx];
            ++v.redundancy_cnt;
            st.erase(it);
            dict[ridx] = st.insert(v).first;
        }
    }
    delete [] dict;
    // Old version end
    */

    
    std::vector<MRMRInfo> mrmrs(nfea);
    MRMRData *cands = new MRMRData[myData->nvar - 1];

    for (i = 1; i < myData->nvar; ++i)
    {
        cands[i - 1] = MRMRData(rtable[0][i], 0.0, 0, i, wr, pers[i]);
    }

    for (i = 0; i < nfea; ++i)
    {
        long idx = 0;
        for (j = 1; j < myData->nvar - 1 - i; ++j)
        {
            if (cands[j] < cands[idx])
            {
                idx = j;
            }
        }

        mrmrs[i] = MRMRInfo(cands[idx].fidx, cands[idx].getScore());
        std::swap(cands[idx], cands[myData->nvar - 1 - i - 1]);

        for (j = 0; j < myData->nvar - 1 - i - 1; ++j)
        {
            cands[j].redundancy_sum += rtable[cands[myData->nvar - 1 - i - 1].fidx][cands[j].fidx];
            ++cands[j].redundancy_cnt;
        }

    }

    delete [] cands;
    

    for (i = 0; i < myData->nvar; ++i)
    {
        delete [] rtable[i];
    }

    delete [] rtable;

    std::sort(mrmrs.begin(), mrmrs.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.score > rhs.score;
    });
  
    return mrmrs;
}

//============================================


std::vector<MRMRInfo> mrmrSolver (const std::vector<FLOAT>& target, const std::vector<std::vector<FLOAT>>& outputs, int nfea, FLOAT w)
{
    DataTable *myData = makeDataTable(target, outputs);
    std::vector<MRMRInfo> ret = mRMRSelection(myData, nfea, w);

    for (auto& it : ret)
    {
        --it.index;
    }
  
    if (myData)
    {
        delete myData;
    }

    return ret;

}

std::vector<MRMRInfo> mrmrSolver (const std::vector<FLOAT>& target, const std::vector<Individual>& population, int nfea, FLOAT w)
{
    if (target.empty() || population.empty())
    {
        return std::vector<MRMRInfo>{};
    }
    std::vector<std::vector<FLOAT>> outputs(population.size(), std::vector<FLOAT>(population[0].details.size()));
    for (size_t i = 0; i < population.size(); ++i)
    {
        assert(population[i].details.size() > 0 && population[i].details.size() == outputs[i].size());
        for (size_t j = 0; j < population[i].details.size(); ++j)
        {
            outputs[i][j] = static_cast<FLOAT>(population[i].details[j]);
        }
    }

    return mrmrSolver(target, outputs, nfea, w);
}
