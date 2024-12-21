#ifndef __AST_H
#define __AST_H

#include <cstdio>
#include <cassert>
#include <vector>
#include <string>
#include "container.h"
#include "type.h"


class ASTNode {
public:
    ASTNode *parent;
    Container node;
    std::vector<ASTNode*> children;
    ASTNode();
    ASTNode(Container _node);
    ASTNode(ASTNode *_parent, Container _node);
    ASTNode(ASTNode *_parent, Container _node, const std::vector<ASTNode*>& _children);
    ~ASTNode();

    std::vector<T> calc(const std::vector<T>& input_list, T& EXECUTION_TIME, T& ERROR) const;

    std::vector<T> calcf(const std::vector<T>& parm1, const std::vector<T>& parm2, T index, T& EXECUTION_TIME, T& ERROR) const;

    std::vector<T> calcg(const std::vector<T>& parm1, T index, T& EXECUTION_TIME, T& ERROR) const;

    std::string print() const;

    T depth() const;

    T function_depth() const;

    T program_size() const;
    
    std::vector<ASTNode*> flatten();
    
    ASTNode* deepcopy();

    void delete_children();

private:
    std::vector<T> _loop(const std::vector<T>& lst, const ASTNode* f, const ASTNode* g, T& EXECUTION_TIME, T& ERROR) const;
    std::vector<T> _filter(const ASTNode* g, const std::vector<T>& lst, T& EXECUTION_TIME, T& ERROR) const;
    std::vector<T> _mapcar(const ASTNode* g, const std::vector<T>& lst, T& EXECUTION_TIME, T& ERROR) const;
};


#endif //__AST_H
