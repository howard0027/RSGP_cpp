
#ifndef __CONTAINER_H

#define __CONTAINER_H

#include <string>
#include <vector>
#include <unordered_map>

#include "type.h"
#include "functions.h"


class Container {
public:
    Container() {}

    static const int LOOP = 0, FUNCTION1 = 1, FUNCTION2 = 2, FUNCTION3 = 3, VALUE = 4, LAMBDA_X = 5, LAMBDA_Y = 6, EMPTY_LIST = 7, INPUT_LIST = 8;
    static const int FILTER = 9, MAPCAR = 10, INDEXING = 11;
    int instance;
    std::string name; // if it is a function, print its name.
    std::string output_type;
    
    _function1 pfunc1;
    _function2 pfunc2;
    _function3 pfunc3;
    std::vector<T> val;
    
};

Container make_loop(const std::string &output_type, const std::string &fname = "loop_real");


Container make_filter(const std::string &output_type, const std::string &fname = "filter");

Container make_mapcar(const std::string &output_type, const std::string &fname = "mapcar");

Container make_indexing(const std::string &output_type, const std::string &fname = "indexing");

Container make_function1(const std::string &output_type, const std::string &fname);

Container make_function2(const std::string &output_type, const std::string &fname);

Container make_function3(const std::string &output_type, const std::string &fname);


Container make_value(const std::string &output_type, const T& val);

Container make_value(const std::string &output_type, const std::vector<T>& val);

Container make_lambda_x(const std::string &output_type);

Container make_lambda_y(const std::string &output_type);

Container make_empty_list(const std::string &output_type);

Container make_input_list(const std::string &output_type);


#endif //__CONTAINER_H