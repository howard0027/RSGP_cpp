#ifndef __FITNESS_H
#define __FITNESS_H

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cassert>
#include <cmath>
#include <queue>
#include <string>
#include <utility>
#include "ast.h"
#include "type.h"

typedef long double DOUBLE;

DOUBLE calPearson(const std::vector<T>& x, const std::vector<T>& y);

void calLine(const std::vector<T>& x, const std::vector<T>& y, DOUBLE& a, DOUBLE& b, bool& vert);

void maeTest(const std::vector<std::vector<T>>& X, const std::vector<T>& y, const ASTNode* root);


std::pair<long long, long long> _count(const std::vector<std::vector<T>>& X, const std::vector<T>& y, const ASTNode* root, std::vector<T>& details);

#endif //_FITNESS_H
