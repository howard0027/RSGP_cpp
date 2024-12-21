//#ifndef __CROSSOVER_H
//#define __CROSSOVER_H

#include "crossover.h"

#include <cstdio>
#include <cassert>

#include <string>
#include <vector>
#include "ast.h"
#include "random.h"
#include "grammar.h"


ASTNode* get_subtree(ASTNode *root, std::string output_type)
{
    std::vector<ASTNode*> flatten_ind = root->flatten();
    size_t n = flatten_ind.size();
    std::vector<T> probs(n, 0);
    T prob_sum = 0;
    T select_index = 0;
    for (size_t i = 0; i < n; ++i)
    {
        if (output_type != "any" && flatten_ind[i]->node.output_type != output_type)
        {
            continue;
        }

        if (flatten_ind[i]->node.instance == Container::LOOP || 
            flatten_ind[i]->node.instance == Container::FUNCTION1 || 
            flatten_ind[i]->node.instance == Container::FUNCTION2 || 
            flatten_ind[i]->node.instance == Container::FUNCTION3)
        {
            probs[i] = 9;
        }
        else
        {
            probs[i] = 1;
        }
        prob_sum += probs[i];
    }
    if (prob_sum == 0)
    {
        return nullptr;
    }
    select_index = random_state(prob_sum) + 1;
    
    for (size_t i = 0; i < n; ++i)
    {
        if (probs[i] == 0)
        {
            if (i > 0) probs[i] += probs[i - 1];
            continue;
        }
        if (i > 0)
        {
            probs[i] += probs[i - 1];
        }
        if (select_index <= probs[i])
        {
            return flatten_ind[i];
        }
        
    }
    // never to here
    assert(0);
    return nullptr;
}

ASTNode* crossover(ASTNode *receiver, ASTNode *donor)
{
    assert(receiver && donor);
    ASTNode *donor_buffer = nullptr;
    ASTNode *receiver_copied = receiver->deepcopy(), *donor_copied = donor->deepcopy();
    //printf("after deep copy = %s\n", receiver_copied->print().c_str());
    ASTNode *receiver_selected = get_subtree(receiver_copied, "any"), *donor_selected = nullptr;
    assert(receiver_selected);
    //printf("> =%s\n", receiver_selected->node.output_type.c_str());
    donor_selected = get_subtree(donor_copied, receiver_selected->node.output_type);
    if (donor_selected == nullptr)
    {
        delete donor_copied;
        return receiver_copied;
    }
    //printf("receiver = %s (selected = %s)\n", receiver->print().c_str(), receiver_selected->print().c_str());
    //printf("donor = %s\n", donor_buffer->print().c_str());
    
    receiver_selected->delete_children();
    *receiver_selected = *donor_selected;

    for (size_t i = 0; i < receiver_selected->children.size(); ++i)
    {
        receiver_selected->children[i] = receiver_selected->children[i]->deepcopy();
    }
    
    delete donor_copied;
    //printf("result = %s\n\n", receiver_copied->print().c_str());
    return receiver_copied;
}

ASTNode* subtree_mutation(ASTNode *ind)
{
    ASTNode* donor = _real(nullptr, random_state(3, 6), random_state(2) == 0 ? GROW : GROW);
    ASTNode* ret = nullptr;
    
    if (random_state(2) == 0)
    {
        ret = crossover(donor, ind);
    }
    else
    {
        ret = crossover(ind, donor);
    }
    
    delete donor;
    
    return ret;
}

//#endif //__CROSSOVER_H