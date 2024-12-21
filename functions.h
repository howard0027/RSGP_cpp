#ifndef __FUNCTIONS_H
#define __FUNCTIONS_H

#include <string>
#include <vector>
#include <unordered_map>

#include "type.h"


std::vector<T> _if(const std::vector<T>& boolean_statement, const std::vector<T>& integer_statement1, const std::vector<T>& integer_statement2, T& EXECUTION_TIME, T& ERROR);

std::vector<T> _add(const std::vector<T>& x, const std::vector<T>& y, T& EXECUTION_TIME, T& ERROR);

std::vector<T> _sub(const std::vector<T>& x, const std::vector<T>& y, T& EXECUTION_TIME, T& ERROR);

std::vector<T> _mul(const std::vector<T>& x, const std::vector<T>& y, T& EXECUTION_TIME, T& ERROR);

std::vector<T> _div(const std::vector<T>& x, const std::vector<T>& y, T& EXECUTION_TIME, T& ERROR);

std::vector<T> _mod(const std::vector<T>& x, const std::vector<T>& y, T& EXECUTION_TIME, T& ERROR);

std::vector<T> _geq(const std::vector<T>& x, const std::vector<T>& y, T& EXECUTION_TIME, T& ERROR);

std::vector<T> _gt(const std::vector<T>& x, const std::vector<T>& y, T& EXECUTION_TIME, T& ERROR);

std::vector<T> _leq(const std::vector<T>& x, const std::vector<T>& y, T& EXECUTION_TIME, T& ERROR);

std::vector<T> _lt(const std::vector<T>& x, const std::vector<T>& y, T& EXECUTION_TIME, T& ERROR);

std::vector<T> _eq(const std::vector<T>& x, const std::vector<T>& y, T& EXECUTION_TIME, T& ERROR);

std::vector<T> _neq(const std::vector<T>& x, const std::vector<T>& y, T& EXECUTION_TIME, T& ERROR);

std::vector<T> _and(const std::vector<T>& x, const std::vector<T>& y, T& EXECUTION_TIME, T& ERROR);

std::vector<T> _or(const std::vector<T>& x, const std::vector<T>& y, T& EXECUTION_TIME, T& ERROR);

std::vector<T> _not(const std::vector<T>& x, T& EXECUTION_TIME, T& ERROR);

std::vector<T> _append(const std::vector<T>& x, const std::vector<T>& y, T& EXECUTION_TIME, T& ERROR);

std::vector<T> _car(const std::vector<T>& x, T& EXECUTION_TIME, T& ERROR);

std::vector<T> _last(const std::vector<T>& x, T& EXECUTION_TIME, T& ERROR);

std::vector<T> _cdr(const std::vector<T>& x, T& EXECUTION_TIME, T& ERROR);


std::vector<T> _cons(const std::vector<T>& x, const std::vector<T>& y, T& EXECUTION_TIME, T& ERROR);

std::vector<T> _reverse(const std::vector<T>& x, T& EXECUTION_TIME, T& ERROR);

std::vector<T> _sort(const std::vector<T>& x, T& EXECUTION_TIME, T& ERROR);

typedef std::vector<T> (*_function1) (const std::vector<T>&, T&, T&);
typedef std::vector<T> (*_function2) (const std::vector<T>&, const std::vector<T>&, T&, T&);
typedef std::vector<T> (*_function3) (const std::vector<T>&, const std::vector<T>&, const std::vector<T>&, T&, T&);

const std::unordered_map<std::string, _function1> _function_map1 = {
    {"not", _not},
    {"reverse", _reverse}, {"cdr", _cdr},
    {"car", _car}, {"last", _last},
    {"sort", _sort}
};

const std::unordered_map<std::string, _function2> _function_map2 = {
    {"and", _and}, {"or", _or},
    {"add", _add}, {"sub", _sub}, {"mul", _mul}, {"div", _div}, {"mod", _mod},
    {"eq", _eq}, {"neq", _neq}, {"geq", _geq}, {"gt", _gt}, {"leq", _leq}, {"lt", _lt},
    {"append", _append}, {"cons", _cons}
};

const std::unordered_map<std::string, _function3> _function_map3 = {
    {"if", _if}
};

#endif // __FUNCTIONS_H