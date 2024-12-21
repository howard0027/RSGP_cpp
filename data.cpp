
#include "data.h"

#include <cstdio>
#include <cassert>
#include <vector>
#include <string>
#include <algorithm>

#include "type.h"
#include "random.h"


void load_data(std::vector<std::vector<T>>& X, std::vector<T>& y, const std::string& task, T n, T tcas, const std::string& folder)
{
    std::string filename = folder + "/d" + task + "_n" + std::to_string(n) + "_" + std::to_string(tcas) + ".txt";
    FILE *f = fopen(filename.c_str(), "r");
    int _n;
    assert(f);
    fscanf(f, "%d", &_n);
    assert(n == _n);
    X.clear();
    y.clear();
    X.resize(n);
    y.resize(n);
    for (int i = 0; i < n; ++i)
    {
        T val;
        char c;
        while (true)
        {
            fscanf(f, "%d%c", &val, &c);
            if (c == '\n')
            {
                y[i] = val;
                break;
            }
            X[i].emplace_back(val);
        }
        
    }
    fclose(f);
    return;
}

#include <cctype>
void parsing(const std::string& s, std::vector<T>& X, T& y)
{
    //{"input1":[1,2,-2,-1],"output1":1}
    //printf("TODO = %s\n", s.c_str());
    size_t idx = 10 + 1;
    T cur = 0, flag = 1;
    bool inside = false;
    
    X.clear();
    while (idx < s.size() && s[idx] != ']')
    {
        inside = true;
        if (!isdigit(s[idx]) && s[idx] != '-')
        {
            X.emplace_back(flag * cur);
            cur = 0;
            flag = 1;
        }
        else
        {
            if (s[idx] == '-') flag = -flag;
            else cur = cur * 10 + s[idx] - '0';
        }
        ++idx;
    }
    if (inside) X.emplace_back(flag * cur);
    idx += 12;
    cur = 0;
    flag = 1;
    while (idx < s.size() && s[idx] != '}')
    {
        if (s[idx] == '-') flag = -flag;
        else cur = cur * 10 + s[idx] - '0';
        ++idx;
    }
    y = flag * cur;
    /*
    printf("%s:\n", s.c_str());
    for (const T& it : X) printf("%d ", it);
    printf("| %d\n\n", y);
    */
    return;
}

void print_data(std::vector<std::vector<T>>& X, std::vector<T>& y)
{
    for (int i = 0; i < X.size(); ++i)
    {
        printf("[");
        for (int j = 0; j < X[i].size(); ++j)
        {
            printf("%d,", X[i][j]);
        }
        printf("]");
        printf(" | %d\n", y[i]);
    }
    return;
}

void load_data_json(std::vector<std::vector<T>>& X_train, std::vector<T>& y_train, std::vector<std::vector<T>>& X_test, std::vector<T>& y_test, const std::string& task, size_t train_n, size_t test_n, const std::string& folder)
{
    std::string edge_filename = folder + "/" + task + "-" + "edge" + ".json";
    std::string random_filename = folder + "/" + task + "-" + "random" + ".json";
    FILE *edge_f = fopen(edge_filename.c_str(), "r");
    FILE *random_f = fopen(random_filename.c_str(), "r");
    char *buf = new char[65536];
    assert(edge_f && random_f);
    //printf("hrr\n");
    X_train.clear();
    y_train.clear();
    X_train.resize(train_n);
    y_train.resize(train_n);

    X_test.clear();
    y_test.clear();
    X_test.resize(test_n);
    y_test.resize(test_n);

    std::vector<std::string> random_buffer, edge_buffer;

    while (fgets(buf, 65534, random_f))
    {
        random_buffer.emplace_back(buf);
        /*
        static int counter = 0;
        ++counter;
        if (counter <= 10)
        {
            printf("= %s\n", buf);
        }
        */
    }
    while (fgets(buf, 65534, edge_f))
    {
        edge_buffer.emplace_back(buf);
    }
    //printf("random size = %d, edge_size = %d\n", random_buffer.size(), edge_buffer.size());
    std::random_shuffle(random_buffer.begin(), random_buffer.end());

    assert(train_n >= (T)edge_buffer.size());
    assert(edge_buffer.size() + random_buffer.size() >= train_n + test_n);
    for (size_t i = 0; i < edge_buffer.size(); ++i)
    {
        parsing(edge_buffer[i], X_train[i], y_train[i]);
        printf("edge = %s\n", edge_buffer[i].c_str());
    }
    for (size_t i = 0; edge_buffer.size() + i < train_n && i < random_buffer.size(); ++i)
    {
        parsing(random_buffer[i], X_train[edge_buffer.size() + i], y_train[edge_buffer.size() + i]);
    }
    for (size_t i = 0, j = random_buffer.size() - 1; i < test_n; ++i, --j)
    {
        parsing(random_buffer[j], X_test[i], y_test[i]);
    }

    print_data(X_train, y_train);
    //printf("\n");
    //print_data(X_test, y_test);

    fclose(edge_f);
    fclose(random_f);
    delete []buf;
    return;
}

