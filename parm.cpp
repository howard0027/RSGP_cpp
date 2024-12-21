
#include "parm.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include "type.h"

extern T CURTASK;
extern T DEBUGFLAG;

void parm(int argc, const char **argv, std::string& task, T& seed, std::string& folder, T& train_n, T& test_n)
{
    if (argc > 1)
    {
        task = argv[1];
    }
    else
    {
        task = "smallest";
    }
    
    if (argc > 2)
    {
        seed = static_cast<T>(atoi(argv[2]));
    }
    else
    {
        seed = 2023;
    }

    if (argc > 3)
    {   
        folder = argv[3];
    }
    else
    {
        folder = "../testdata_v4";
    }

    if (task == "mnp")
    {
        CURTASK = MNP;
        train_n = 100;
        test_n = 1000;
    }
    else if (task == "mpmn")
    {
        CURTASK = MPMN;
        train_n = 100;
        test_n = 1000;
    }
    else if (task == "mptmn")
    {
        CURTASK = MPTMN;
        train_n = 100;
        test_n = 1000;
    }
    else if (task == "smallest")
    {
        CURTASK = SMALLEST;
        train_n = 100;
        test_n = 1000;
    }
    else if (task == "count-odds")
    {
        CURTASK = COUNTODDS;
        train_n = 200;
        test_n = 2000;
    }
    else if (task == "last-index-of-zero")
    {
        CURTASK = LASTINDEXOFZERO;
        train_n = 150;
        test_n = 1000;
    }
    else if (task == "basement")
    {
        CURTASK = BASEMENT;
        train_n = 200;
        test_n = 2000;
    }
    else if (task == "fuel-cost")
    {
        CURTASK = FUELCOST;
        train_n = 200;
        test_n = 2000;
    }
    else if (task == "luhn")
    {
        CURTASK = LUHN;
        train_n = 200;
        test_n = 2000;
    }
    
}

