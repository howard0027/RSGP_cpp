#ifndef __PROGSIMP_H
#define __PROGSIMP_H

#include <vector>

#include "type.h"
#include "ast.h"


ASTNode* progsimp(const std::vector<std::vector<T>>& X, const std::vector<T>& y, ASTNode *ind, long long& fitness);


#endif // __PROGSIMP_H