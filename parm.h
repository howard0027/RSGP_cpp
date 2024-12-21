#ifndef __PARM_H
#define __PARM_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include "type.h"

extern T CURTASK;
extern T DEBUGFLAG;

void parm(int argc, const char **argv, std::string& task, T& seed, std::string& folder, T& train_n, T& test_n);
//void parm(int argc, const char **argv, std::string& task, std::string& fitness_op, T& ts, T& seed, T& tcas, std::string& folder, T& train_n, T& test_n);


#endif //__PARM_H