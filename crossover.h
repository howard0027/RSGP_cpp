#ifndef __CROSSOVER_H
#define __CROSSOVER_H

#include <string>
#include "ast.h"

ASTNode* get_subtree(ASTNode *root, std::string output_type = "any");

ASTNode* crossover(ASTNode *receiver, ASTNode *donor);

ASTNode* subtree_mutation(ASTNode *ind);

#endif //__CROSSOVER_H