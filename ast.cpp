
#include "ast.h"

#include <cstdio>
#include <cassert>
#include <vector>
#include <string>
#include "container.h"
#include "type.h"


ASTNode::ASTNode() : parent(nullptr) {}
ASTNode::ASTNode(Container _node) : parent(nullptr), node(_node) {}
ASTNode::ASTNode(ASTNode *_parent, Container _node) : parent(_parent), node(_node) {}
ASTNode::ASTNode(ASTNode *_parent, Container _node, const std::vector<ASTNode*>& _children) : parent(_parent), node(_node), children(_children) {}
ASTNode::~ASTNode()
{
    this->delete_children();
}

std::vector<T> ASTNode::calc(const std::vector<T>& input_list, T& EXECUTION_TIME, T& ERROR) const
{
    ++EXECUTION_TIME;
    if (node.instance == Container::LOOP)
    {
        assert(this->children.size() == 3);
        return this->_loop(this->children[0]->calc(input_list, EXECUTION_TIME, ERROR), this->children[1], this->children[2], EXECUTION_TIME, ERROR);
    }
    if (node.instance == Container::FILTER)
    {
        assert(this->children.size() == 2);
        return this->_filter(this->children[0], this->children[1]->calc(input_list, EXECUTION_TIME, ERROR), EXECUTION_TIME, ERROR);
    }
    if (node.instance == Container::MAPCAR)
    {
        assert(this->children.size() == 2);
        return this->_mapcar(this->children[0], this->children[1]->calc(input_list, EXECUTION_TIME, ERROR), EXECUTION_TIME, ERROR);
    }
    if (node.instance == Container::INPUT_LIST)
    {
        return input_list;
    }
    if (node.instance == Container::VALUE)
    {
        return node.val;
    }
    if (node.instance == Container::FUNCTION1)
    {
        assert(this->children.size() == 1);
        return node.pfunc1(this->children[0]->calc(input_list, EXECUTION_TIME, ERROR), EXECUTION_TIME, ERROR);
    }
    if (node.instance == Container::FUNCTION2)
    {
        assert(this->children.size() == 2);
        return node.pfunc2(this->children[0]->calc(input_list, EXECUTION_TIME, ERROR), this->children[1]->calc(input_list, EXECUTION_TIME, ERROR), EXECUTION_TIME, ERROR);
    }
    if (node.instance == Container::FUNCTION3)
    {
        assert(this->children.size() == 3);
        return node.pfunc3(this->children[0]->calc(input_list, EXECUTION_TIME, ERROR), this->children[1]->calc(input_list, EXECUTION_TIME, ERROR), this->children[2]->calc(input_list, EXECUTION_TIME, ERROR), EXECUTION_TIME, ERROR);
    }
    ERROR = 1;
    return std::vector<T>{};
}

std::vector<T> ASTNode::calcf(const std::vector<T>& parm1, const std::vector<T>& parm2, T index, T& EXECUTION_TIME, T& ERROR) const
{
    ++EXECUTION_TIME;
    if (node.instance == Container::LAMBDA_X)
    {
        return parm1;
    }
    if (node.instance == Container::LAMBDA_Y)
    {
        return parm2;
    }
    if (node.instance == Container::VALUE)
    {
        return node.val;
    }
    if (node.instance == Container::INDEXING)
    {
        return std::vector<T>{index};
    }
    if (node.instance == Container::FUNCTION1)
    {
        assert(this->children.size() == 1);
        return node.pfunc1(this->children[0]->calcf(parm1, parm2, index, EXECUTION_TIME, ERROR), EXECUTION_TIME, ERROR);
    }
    if (node.instance == Container::FUNCTION2)
    {
        assert(this->children.size() == 2);
        return node.pfunc2(this->children[0]->calcf(parm1, parm2, index, EXECUTION_TIME, ERROR), this->children[1]->calcf(parm1, parm2, index, EXECUTION_TIME, ERROR), EXECUTION_TIME, ERROR);
    }
    if (node.instance == Container::FUNCTION3)
    {
        assert(this->children.size() == 3);
        return node.pfunc3(this->children[0]->calcf(parm1, parm2, index, EXECUTION_TIME, ERROR), this->children[1]->calcf(parm1, parm2, index, EXECUTION_TIME, ERROR), this->children[2]->calcf(parm1, parm2, index, EXECUTION_TIME, ERROR), EXECUTION_TIME, ERROR);
    }
    ERROR = 1;
    return std::vector<T>{};
}

std::vector<T> ASTNode::calcg(const std::vector<T>& parm1, T index, T& EXECUTION_TIME, T& ERROR) const
{
    ++EXECUTION_TIME;
    if (node.instance == Container::LAMBDA_X)
    {
        return parm1;
    }
    if (node.instance == Container::VALUE)
    {
        return node.val;
    }
    if (node.instance == Container::INDEXING)
    {
        return std::vector<T>{index};
    }
    if (node.instance == Container::FUNCTION1)
    {
        assert(this->children.size() == 1);
        return node.pfunc1(this->children[0]->calcg(parm1, index, EXECUTION_TIME, ERROR), EXECUTION_TIME, ERROR);
    }
    if (node.instance == Container::FUNCTION2)
    {
        assert(this->children.size() == 2);
        return node.pfunc2(this->children[0]->calcg(parm1, index, EXECUTION_TIME, ERROR), this->children[1]->calcg(parm1, index, EXECUTION_TIME, ERROR), EXECUTION_TIME, ERROR);
    }
    if (node.instance == Container::FUNCTION3)
    {
        assert(this->children.size() == 3);
        return node.pfunc3(this->children[0]->calcg(parm1, index, EXECUTION_TIME, ERROR), this->children[1]->calcg(parm1, index, EXECUTION_TIME, ERROR), this->children[2]->calcg(parm1, index, EXECUTION_TIME, ERROR), EXECUTION_TIME, ERROR);
    }
    ERROR = 1;
    return std::vector<T>{};
}

std::string ASTNode::print() const
{
    std::string ret;
    //printf("node.instance = %d, %p\n", node.instance, this);
    //printf("node.name = %s\n", node.name.c_str());
    if (node.instance == Container::LAMBDA_X)
    {
        return "x";
    }
    else if (node.instance == Container::LAMBDA_Y)
    {
       return "y";
    }
    else if (node.instance == Container::EMPTY_LIST)
    {
        return "[]";
    }
    else if (node.instance == Container::INPUT_LIST)
    {
        return "input_list";
    }
    else if (node.instance == Container::VALUE)
    {
        //printf("hrr\n");
        //printf("node.val[0] = %d\n", node.val[0]);
        return std::to_string(node.val[0]);
    }
    else
    {
            
        ret = node.name + "(";
        //printf("size = %d\n", (int)children.size());
            
        for (size_t i = 0; i < children.size(); ++i)
        {
            //printf("before, i = %d / %d\n", (int)i, (int)children.size());
            //printf("=%p\n", children[i]);
            ret += children[i]->print();
            //printf("after %p\n", children[i]);
            if (i < children.size() - 1)
            {
                ret += ",";
            }
        }
        ret += ")";
        
    }
    return ret;
}

T ASTNode::depth() const
{
    T ret = 0;
    if (this->children.empty())
    {
        return 0;
    }

    for (ASTNode* child : this->children)
    {
        ret = std::max(ret, child->depth());
    }
    return ++ret;
}

T ASTNode::function_depth() const
{
    return 0;
}

T ASTNode::program_size() const
{
    T ret = 1;
    if (this->children.empty())
    {
        return 1;
    }
    for (ASTNode* child : this->children)
    {
        ret += child->program_size();
    }
    return ret;
}
    
std::vector<ASTNode*> ASTNode::flatten()
{
    std::vector<ASTNode*> ret(1, this);
    for (ASTNode* child : this->children)
    {
        std::vector<ASTNode*> res = child->flatten();
        ret.insert(ret.end(), std::make_move_iterator(res.begin()), std::make_move_iterator(res.end()));
    }
    return ret;
}
    
ASTNode* ASTNode::deepcopy()
{
    ASTNode *ret = new ASTNode(this->parent, this->node);
    ret->children.clear();
    ret->children.resize(this->children.size());
    for (size_t i = 0; i < this->children.size(); ++i)
    {
        ret->children[i] = this->children[i]->deepcopy();
    }
    return ret;
}

void ASTNode::delete_children()
{
    for (size_t i = 0; i < this->children.size(); ++i)
    {
        delete this->children[i];
    }
    this->children.clear();
    return;
}


std::vector<T> ASTNode::_loop(const std::vector<T>& lst, const ASTNode* f, const ASTNode* g, T& EXECUTION_TIME, T& ERROR) const
{
    if (lst.empty())
    {
        //return std::vector<T> {0};
        ERROR = 1;
        return std::vector<T> {};
    }
    T index = (T)lst.size() - 1;
    std::vector<T>::const_reverse_iterator rit = lst.rbegin();
    std::vector<T> ret = g->calcg(std::vector<T>{*rit}, index, EXECUTION_TIME, ERROR);
        
    for (++rit, --index; rit != lst.rend(); ++rit, --index)
    {
        ++EXECUTION_TIME;
        ret = f->calcf(std::vector<T> {*rit}, ret, index, EXECUTION_TIME, ERROR);
    }
    //std::reverse(ret.begin(), ret.end());
    return ret;
}

std::vector<T> ASTNode::_filter(const ASTNode* g, const std::vector<T>& lst, T& EXECUTION_TIME, T& ERROR) const
{
    if (lst.empty())
    {
        //ERROR = 1;
        return std::vector<T> {};
    }
    T index = 0;
    std::vector<T> ret;
    for (std::vector<T>::const_iterator it = lst.begin(); it != lst.end(); ++it, ++index)
    {
        ++EXECUTION_TIME;
        std::vector<T> res = g->calcg(std::vector<T> {*it}, index, EXECUTION_TIME, ERROR);
        if (res.empty())
        {
            ERROR = 1;
            return std::vector<T> {};
        }
        if (res[0])
        {
            ret.emplace_back(*it);
        }
    }
    return ret;
}

std::vector<T> ASTNode::_mapcar(const ASTNode* g, const std::vector<T>& lst, T& EXECUTION_TIME, T& ERROR) const
{
    if (lst.empty())
    {
        //ERROR = 1;
        return std::vector<T> {};
    }
    T index = 0;
    std::vector<T> ret;
    for (std::vector<T>::const_iterator it = lst.begin(); it != lst.end(); ++it, ++index)
    {
        ++EXECUTION_TIME;
        std::vector<T> res = g->calcg(std::vector<T> {*it}, index, EXECUTION_TIME, ERROR);
        if (ERROR == 1 || res.empty())
        {
            ERROR = 1;
            return std::vector<T> {};
        }
        ret.emplace_back(res[0]);
    }
    return ret;
}




