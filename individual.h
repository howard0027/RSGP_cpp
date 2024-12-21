//individual.h
#ifndef __INDIVIDUAL_H
#define __INDIVIDUAL_H

#include "ast.h"
#include "type.h"
#include <vector>


class Individual {
public:
    ASTNode *root;
    //T raw_fitness;
    long long raw_fitness;
    T depth;
    T program_size;
    T execution_time;
    std::vector<T> details;

    //const 

    

};

#endif //__INDIVIDUAL_H