#ifndef __DATA_H
#define __DATA_H

#include <cstdio>
#include <cassert>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

#include "type.h"
#include "random.h"


void load_data(std::vector<std::vector<T>>& X, std::vector<T>& y, const std::string& task, T n, T tcas, const std::string& folder);


void parsing(const std::string& s, std::vector<T>& X, T& y);

void print_data(std::vector<std::vector<T>>& X, std::vector<T>& y);

void load_data_json(std::vector<std::vector<T>>& X_train, std::vector<T>& y_train, std::vector<std::vector<T>>& X_test, std::vector<T>& y_test, const std::string& task, size_t train_n, size_t test_n, const std::string& folder);

#endif //__DATA_H