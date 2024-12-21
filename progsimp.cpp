
#include "progsimp.h"

#include <cstdio>
#include <cassert>
#include <queue>
#include <string>
#include <vector>

#include "type.h"
#include "container.h"
#include "ast.h"
#include "fitness.h"

// TODO: solve memory leak issue

ASTNode* progsimp(const std::vector<std::vector<T>>& X, const std::vector<T>& y, ASTNode *ind, long long& fitness)
{
    std::vector<T> details;
    std::pair<long long, long long> p = _count(X, y, ind, details);
    long long bestFitness = p.first;
    ASTNode* bestInd = ind;
    T bestPS = ind->program_size();
    
    // Local search
    for (ASTNode *child : ind->children)
    {
        if (child->node.output_type == ind->node.output_type)
        {
            p = _count(X, y, child, details);
            long long childFitness = p.first;
            if (childFitness < bestFitness)
            {
                bestFitness = childFitness;
                bestInd = child;
            }
            else if (childFitness == bestFitness && child->program_size() < bestInd->program_size())
            {
                bestInd = child;
            }
        }
    }
    if (bestInd != ind)
    {
        fitness = bestFitness;
        return progsimp(X, y, bestInd, fitness);
    }
    
    // program simplification
    while (true)
    {
        bool flag = false;
        std::queue<std::pair<std::pair<ASTNode*, ASTNode*>, size_t>> q;
        
        for (size_t i = 0; i < bestInd->children.size(); ++i)
        {
            q.push(std::make_pair(std::make_pair(bestInd->children[i], bestInd), i));
        }

        while (q.size())
        {
            std::pair<std::pair<ASTNode*, ASTNode*>, size_t> qp = q.front(); q.pop();
            ASTNode* root = qp.first.first;
            ASTNode* parent = qp.first.second;
            size_t idx = qp.second;
            
            flag = false;

            if (root->node.name == "if")
            {
                // if A then B else C may be simplified to either B or C.
                ASTNode* tmp = parent->children[idx];
                parent->children[idx] = root->children[1];
                p = _count(X, y, bestInd, details);
                if (p.first < bestFitness || (p.first == bestFitness && bestInd->program_size() < bestPS))
                {
                    bestFitness = p.first;
                    bestPS = bestInd->program_size();
                    flag = true;
                }
                else
                {
                    parent->children[idx] = tmp;
                }
                tmp = parent->children[idx];
                parent->children[idx] = root->children[2];
                p = _count(X, y, bestInd, details);
                if (p.first < bestFitness || (p.first == bestFitness && bestInd->program_size() < bestPS))
                {
                    bestFitness = p.first;
                    bestPS = bestInd->program_size();
                    flag = true;
                }
                else
                {
                    parent->children[idx] = tmp;
                }
            }
            else if (root->node.name == "and" || root->node.name == "or")
            {
                // boolean simplification
                ASTNode* tmp = parent->children[idx];
                parent->children[idx] = root->children[0];
                p = _count(X, y, bestInd, details);
                if (p.first < bestFitness || (p.first == bestFitness && bestInd->program_size() < bestPS))
                {
                    bestFitness = p.first;
                    bestPS = bestInd->program_size();
                    flag = true;
                }
                else
                {
                    parent->children[idx] = tmp;
                }

                tmp = parent->children[idx];
                parent->children[idx] = root->children[1];
                p = _count(X, y, bestInd, details);
                if (p.first < bestFitness || (p.first == bestFitness && bestInd->program_size() < bestPS))
                {
                    bestFitness = p.first;
                    bestPS = bestInd->program_size();
                    flag = true;
                }
                else
                {
                    parent->children[idx] = tmp;
                }

            }
            else if (root->node.name == "leq" || root->node.name == "lt" || root->node.name == "geq" || root->node.name == "gt" || root->node.name == "eq" || root->node.name == "neq")
            {
                // boolean simplification
                ASTNode *model = new ASTNode(), *model2;
                model->node = make_function2("bool", "gt");
                model->children.clear(); model->children.resize(2);
                for (int i = 0; i < 2; ++i) model->children[i] = new ASTNode();
                model->children[0]->node.instance = model->children[1]->node.instance = Container::VALUE;
                model->children[0]->node.val = model->children[1]->node.val = std::vector<T> {0};
                ASTNode* tmp = parent->children[idx];
                parent->children[idx] = model;
                ASTNode *tmpbest = bestInd->deepcopy();
                parent->children[idx] = tmp;
                p = _count(X, y, tmpbest, details);


                model2 = model->deepcopy();
                model2->node = make_function2("bool", "eq");

                if (model->print() != root->print() && model2->print() != root->print())
                {
                    if (p.first <= bestFitness)
                    {
                        bestInd = tmpbest;
                        bestFitness = p.first;
                        bestPS = bestInd->program_size();
                        flag = true;
                    }

                    if (flag == false)
                    {
                        tmp = parent->children[idx];
                        parent->children[idx] = model2;
                        tmpbest = bestInd->deepcopy();
                        parent->children[idx] = tmp;
                        p = _count(X, y, tmpbest, details);
                        if (p.first <= bestFitness)
                        {
                            bestInd = tmpbest;
                            bestFitness = p.first;
                            bestPS = bestInd->program_size();
                            flag = true;
                        }
                    }
                    
                }
            }
            else if (root->node.name == "cons")
            {
                // cons(1, [2, 3, 4, 5]) => [2, 3, 4, 5]
                ASTNode* tmp = parent->children[idx];
                parent->children[idx] = root->children[1];
                p = _count(X, y, bestInd, details);
                if (p.first <= bestFitness)
                {
                    bestFitness = p.first;
                    bestPS = bestInd->program_size();
                    flag = true;
                }
                else
                {
                    parent->children[idx] = tmp;
                }
            }

            if (flag == true)
            {
                // The function has simplified the program at this iteration. 
                break;
            }
            
            for (size_t i = 0; i < root->children.size(); ++i)
            {
                q.push(std::make_pair(std::make_pair(root->children[i], root), i));
            }
        }
        
        if (flag == false)
        {
            break;
        }
    }

    return bestInd;
    
}

