
#ifndef __MRMR_H
#define __MRMR_H

#include <vector>

#include "individual.h"
#include "type.h"

class MRMRInfo
{
public:
    long index;
    FLOAT score;
    MRMRInfo();
    MRMRInfo(long index, FLOAT score);
};

class DataTable
{
public:
    FLOAT *data;
    FLOAT **data2d;
    long nsample;
    long nvar;

    DataTable();
    ~DataTable();
    int buildData2d();
    void destroyData2d();
    
};

class MRMRData {
public:
    FLOAT relevance;
    FLOAT redundancy_sum;
    long redundancy_cnt;
    long fidx;
    FLOAT wr;
    long rank;
    MRMRData() {}
    MRMRData(FLOAT relevance, FLOAT redundancy_sum, long redundancy_cnt, long fidx, FLOAT wr, long rank)
     : relevance(relevance), redundancy_sum(redundancy_sum), redundancy_cnt(redundancy_cnt), fidx(fidx), wr(wr), rank(rank) {}
    
    bool operator < (const MRMRData& rhs) const;
    FLOAT getScore() const;
};

DataTable * makeDataTable(const std::vector<FLOAT>& target, const std::vector<std::vector<FLOAT>>& outputs);

std::vector<MRMRInfo> mRMRSelection (DataTable * myData, long nfea, FLOAT wr);

std::vector<MRMRInfo> mrmrSolver (const std::vector<FLOAT>& target, const std::vector<std::vector<FLOAT>>& outputs, int nfea, FLOAT w);

std::vector<MRMRInfo> mrmrSolver (const std::vector<FLOAT>& target, const std::vector<Individual>& population, int nfea, FLOAT w);


#endif //__MRMR_H
