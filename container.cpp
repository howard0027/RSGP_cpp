
#include "container.h"

#include <string>
#include <vector>
#include <unordered_map>

#include "type.h"
#include "functions.h"


Container make_loop(const std::string &output_type, const std::string &fname)
{
    Container ret;
    ret.instance = Container::LOOP;
    ret.name = fname;
    ret.output_type = output_type;
    return ret;
}


Container make_filter(const std::string &output_type, const std::string &fname)
{
    Container ret;
    ret.instance = Container::FILTER;
    ret.name = fname;
    ret.output_type = output_type;
    return ret;
}

Container make_mapcar(const std::string &output_type, const std::string &fname)
{
    Container ret;
    ret.instance = Container::MAPCAR;
    ret.name = fname;
    ret.output_type = output_type;
    return ret;
}

Container make_indexing(const std::string &output_type, const std::string &fname)
{
    Container ret;
    ret.instance = Container::INDEXING;
    ret.name = fname;
    ret.output_type = output_type;
    return ret;
}

Container make_function1(const std::string &output_type, const std::string &fname)
{
    std::unordered_map<std::string, _function1>::const_iterator it = _function_map1.find(fname);
    Container ret;
    if (it == _function_map1.end())
    {
        fprintf(stderr, "error 1\n");
        return ret;
    }
    ret.instance = Container::FUNCTION1;
    ret.name = fname;
    ret.output_type = output_type;
    ret.pfunc1 = it->second;
    return ret;
}

Container make_function2(const std::string &output_type, const std::string &fname)
{
    std::unordered_map<std::string, _function2>::const_iterator it = _function_map2.find(fname);
    Container ret;
    if (it == _function_map2.end())
    {
        fprintf(stderr, "error 2\n");
        return ret;
    }
    ret.instance = Container::FUNCTION2;
    ret.name = fname;
    ret.output_type = output_type;
    ret.pfunc2 = it->second;
    return ret;
}

Container make_function3(const std::string &output_type, const std::string &fname)
{
    std::unordered_map<std::string, _function3>::const_iterator it = _function_map3.find(fname);
    Container ret;
    if (it == _function_map3.end())
    {
        fprintf(stderr, "error 3\n");
        return ret;
    }
    ret.instance = Container::FUNCTION3;
    ret.name = fname;
    ret.output_type = output_type;
    ret.pfunc3 = it->second;
    return ret;
}


Container make_value(const std::string &output_type, const T& val)
{
    Container ret;
    ret.instance = Container::VALUE;
    ret.output_type = output_type;
    ret.val = std::vector<T>{val};
    return ret;
}

Container make_value(const std::string &output_type, const std::vector<T>& val)
{
    Container ret;
    ret.instance = Container::VALUE;
    ret.output_type = output_type;
    ret.val = val;
    return ret;
}

Container make_lambda_x(const std::string &output_type)
{
    Container ret;
    ret.instance = Container::LAMBDA_X;
    ret.output_type = output_type;
    return ret;
}

Container make_lambda_y(const std::string &output_type)
{
    Container ret;
    ret.instance = Container::LAMBDA_Y;
    ret.output_type = output_type;
    return ret;
}

Container make_empty_list(const std::string &output_type)
{
    Container ret;
    ret.instance = Container::EMPTY_LIST;
    ret.output_type = output_type;
    return ret;
}

Container make_input_list(const std::string &output_type)
{
    Container ret;
    ret.instance = Container::INPUT_LIST;
    ret.output_type = output_type;
    return ret;
}

