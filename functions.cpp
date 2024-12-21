
#include "functions.h"

#include <cstdio>
#include <cmath>
#include <cassert>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "type.h"


typedef int32_t T;

std::vector<T> _if(const std::vector<T>& boolean_statement, const std::vector<T>& integer_statement1, const std::vector<T>& integer_statement2, T& EXECUTION_TIME, T& ERROR)
{
    if (boolean_statement.empty())
    {
        ERROR = 1;
        return std::vector<T> {};
    }
    assert(!boolean_statement.empty());
    ++EXECUTION_TIME;
    if (boolean_statement[0])
    {
        return integer_statement1;
    }
    return integer_statement2;
}

std::vector<T> _add(const std::vector<T>& x, const std::vector<T>& y, T& EXECUTION_TIME, T& ERROR)
{
    int n = (int)x.size();
    std::vector<T> ret(n);
    if (x.empty() || y.empty())
    {
        ERROR = 1;
        return std::vector<T> {};
    }
    assert(x.size() == y.size());
    ++EXECUTION_TIME;
    std::vector<T>::const_iterator itx = x.begin(), ity = y.begin();
    for (std::vector<T>::iterator itr = ret.begin(); itx != x.end(); ++itx, ++ity, ++itr)
    {
        *itr = *itx + *ity;
    }

    return ret;
}

std::vector<T> _sub(const std::vector<T>& x, const std::vector<T>& y, T& EXECUTION_TIME, T& ERROR)
{
    int n = (int)x.size();
    std::vector<T> ret(n);
    if (x.empty() || y.empty())
    {
        ERROR = 1;
        return std::vector<T> {};
    }
    //printf("x.size() = %d, y.size() = %d\n", (int)x.size(), (int)y.size());
    assert(x.size() == y.size());
    ++EXECUTION_TIME;
    std::vector<T>::const_iterator itx = x.begin(), ity = y.begin();
    for (std::vector<T>::iterator itr = ret.begin(); itx != x.end(); ++itx, ++ity, ++itr)
    {
        *itr = *itx - *ity;
    }
    
    return ret;
}

std::vector<T> _mul(const std::vector<T>& x, const std::vector<T>& y, T& EXECUTION_TIME, T& ERROR)
{
    int n = (int)x.size();
    std::vector<T> ret(n);
    if (x.empty() || y.empty())
    {
        ERROR = 1;
        return std::vector<T> {};
    }
    if (x.size() != y.size())
    {
        fprintf(stderr, "%d vs %d\n", (int)x.size(), (int)y.size());
    }
    assert(x.size() == y.size());
    ++EXECUTION_TIME;
    std::vector<T>::const_iterator itx = x.begin(), ity = y.begin();
    for (std::vector<T>::iterator itr = ret.begin(); itx != x.end(); ++itx, ++ity, ++itr)
    {
        *itr = (*itx) * (*ity);
    }
    
    return ret;
}

std::vector<T> _div(const std::vector<T>& x, const std::vector<T>& y, T& EXECUTION_TIME, T& ERROR)
{
    int n = (int)x.size();
    std::vector<T> ret(n);
    if (x.empty() || y.empty())
    {
        ERROR = 1;
        return std::vector<T> {};
    }
    assert(x.size() == y.size());
    ++EXECUTION_TIME;
    std::vector<T>::const_iterator itx = x.begin(), ity = y.begin();
    for (std::vector<T>::iterator itr = ret.begin(); itx != x.end(); ++itx, ++ity, ++itr)
    {
        if (*itx == -2147483648 && *ity == -1)
        {
            *itr = *itx;
            //ERROR = 1;
            //return std::vector<T> {};
        }
        
        else if (*ity != 0)
        {
            //printf("before, *itx = %d, *ity = %d\n", *itx, *ity);
            //*itr = (*itx) / (*ity);
            *itr = static_cast<T>(floor(static_cast<double>(*itx) / static_cast<double>(*ity)));
            //printf("after\n");
        }
        else
        {
            *itr = 0;
            //ERROR = 1;
            //return std::vector<T> {};
        }
    }
    
    return ret;
}

std::vector<T> _mod(const std::vector<T>& x, const std::vector<T>& y, T& EXECUTION_TIME, T& ERROR)
{
    int n = (int)x.size();
    std::vector<T> ret(n);
    if (x.empty() || y.empty())
    {
        ERROR = 1;
        return std::vector<T> {};
    }
    assert(x.size() == y.size());
    ++EXECUTION_TIME;
    //std::vector<T> res_div = _div(x, y);
    std::vector<T>::const_iterator itx = x.begin(), ity = y.begin();
    for (std::vector<T>::iterator itr = ret.begin(); itx != x.end(); ++itx, ++ity, ++itr)
    {
        if (*itx == -2147483648 && *ity == -1)
        {
            *itr = 0;
            //ERROR = 1;
            //return std::vector<T> {};
        }
        
        else if (*ity != 0)
        {
            //printf("before, *itx = %d, *ity = %d\n", *itx, *ity);
            //*itr = (*itx) / (*ity);
            *itr = static_cast<T>(floor(static_cast<double>(*itx) / static_cast<double>(*ity)));
            *itr = (*itx) - (*itr) * (*ity);
            //printf("after\n");
        }
        else
        {
            *itr = 0;
            //ERROR = 1;
            //return std::vector<T> {};
        }
    }
    
    return ret;
}

std::vector<T> _geq(const std::vector<T>& x, const std::vector<T>& y, T& EXECUTION_TIME, T& ERROR)
{
    int n = (int)x.size();
    std::vector<T> ret(n);
    if (x.empty() || y.empty())
    {
        ERROR = 1;
        return std::vector<T> {};
    }
    assert(x.size() == y.size());
    ++EXECUTION_TIME;
    std::vector<T>::const_iterator itx = x.begin(), ity = y.begin();
    for (std::vector<T>::iterator itr = ret.begin(); itx != x.end(); ++itx, ++ity, ++itr)
    {
        *itr = (*itx) >= (*ity);
    }
    
    return ret;
}

std::vector<T> _gt(const std::vector<T>& x, const std::vector<T>& y, T& EXECUTION_TIME, T& ERROR)
{
    int n = (int)x.size();
    std::vector<T> ret(n);
    if (x.empty() || y.empty())
    {
        ERROR = 1;
        return std::vector<T> {};
    }
    assert(x.size() == y.size());
    ++EXECUTION_TIME;
    std::vector<T>::const_iterator itx = x.begin(), ity = y.begin();
    for (std::vector<T>::iterator itr = ret.begin(); itx != x.end(); ++itx, ++ity, ++itr)
    {
        *itr = (*itx) > (*ity);
    }
    
    return ret;
}

std::vector<T> _leq(const std::vector<T>& x, const std::vector<T>& y, T& EXECUTION_TIME, T& ERROR)
{
    int n = (int)x.size();
    std::vector<T> ret(n);
    if (x.empty() || y.empty())
    {
        ERROR = 1;
        return std::vector<T> {};
    }
    assert(x.size() == y.size());
    ++EXECUTION_TIME;
    std::vector<T>::const_iterator itx = x.begin(), ity = y.begin();
    for (std::vector<T>::iterator itr = ret.begin(); itx != x.end(); ++itx, ++ity, ++itr)
    {
        *itr = (*itx) <= (*ity);
    }
    
    return ret;
}

std::vector<T> _lt(const std::vector<T>& x, const std::vector<T>& y, T& EXECUTION_TIME, T& ERROR)
{
    int n = (int)x.size();
    std::vector<T> ret(n);
    if (x.empty() || y.empty())
    {
        ERROR = 1;
        return std::vector<T> {};
    }
    assert(x.size() == y.size());
    ++EXECUTION_TIME;
    std::vector<T>::const_iterator itx = x.begin(), ity = y.begin();
    for (std::vector<T>::iterator itr = ret.begin(); itx != x.end(); ++itx, ++ity, ++itr)
    {
        *itr = (*itx) < (*ity);
    }
    
    return ret;
}

std::vector<T> _eq(const std::vector<T>& x, const std::vector<T>& y, T& EXECUTION_TIME, T& ERROR)
{
    int n = (int)x.size();
    std::vector<T> ret(n);
    if (x.empty() || y.empty())
    {
        ERROR = 1;
        return std::vector<T> {};
    }
    assert(x.size() == y.size());
    ++EXECUTION_TIME;
    std::vector<T>::const_iterator itx = x.begin(), ity = y.begin();
    for (std::vector<T>::iterator itr = ret.begin(); itx != x.end(); ++itx, ++ity, ++itr)
    {
        *itr = (*itx) == (*ity);
    }
    
    return ret;
}

std::vector<T> _neq(const std::vector<T>& x, const std::vector<T>& y, T& EXECUTION_TIME, T& ERROR)
{
    int n = (int)x.size();
    std::vector<T> ret(n);
    if (x.empty() || y.empty())
    {
        ERROR = 1;
        return std::vector<T> {};
    }
    assert(x.size() == y.size());
    ++EXECUTION_TIME;
    std::vector<T>::const_iterator itx = x.begin(), ity = y.begin();
    for (std::vector<T>::iterator itr = ret.begin(); itx != x.end(); ++itx, ++ity, ++itr)
    {
        *itr = (*itx) != (*ity);
    }
    
    return ret;
}

std::vector<T> _and(const std::vector<T>& x, const std::vector<T>& y, T& EXECUTION_TIME, T& ERROR)
{
    int n = (int)x.size();
    std::vector<T> ret(n);
    if (x.empty() || y.empty())
    {
        ERROR = 1;
        return std::vector<T> {};
    }
    assert(x.size() == y.size());
    ++EXECUTION_TIME;
    std::vector<T>::const_iterator itx = x.begin(), ity = y.begin();
    for (std::vector<T>::iterator itr = ret.begin(); itx != x.end(); ++itx, ++ity, ++itr)
    {
        *itr = (*itx) & (*ity);
    }
    
    return ret;
}

std::vector<T> _or(const std::vector<T>& x, const std::vector<T>& y, T& EXECUTION_TIME, T& ERROR)
{
    int n = (int)x.size();
    std::vector<T> ret(n);
    if (x.empty() || y.empty())
    {
        ERROR = 1;
        return std::vector<T> {};
    }
    assert(x.size() == y.size());
    ++EXECUTION_TIME;
    std::vector<T>::const_iterator itx = x.begin(), ity = y.begin();
    for (std::vector<T>::iterator itr = ret.begin(); itx != x.end(); ++itx, ++ity, ++itr)
    {
        *itr = (*itx) | (*ity);
    }
    
    return ret;
}

std::vector<T> _not(const std::vector<T>& x, T& EXECUTION_TIME, T& ERROR)
{
    std::vector<T> ret(x.size());
    if (x.empty())
    {
        ERROR = 1;
        return std::vector<T> {};
    }
    assert(x.size() > 0);
    ++EXECUTION_TIME;
    std::vector<T>::const_iterator itx = x.begin();
    for (std::vector<T>::iterator itr = ret.begin(); itx != x.end(); ++itx, ++itr)
    {
        if (*itx)
        {
            *itr = 0;
        }
        else
        {
            *itr = 1;
        }
    }
    
    return ret;
}

std::vector<T> _append(const std::vector<T>& x, const std::vector<T>& y, T& EXECUTION_TIME, T& ERROR)
{
    std::vector<T> ret(x);
    EXECUTION_TIME += static_cast<T>(y.size());
    ret.insert(ret.end(), y.begin(), y.end());
    return ret;
}

std::vector<T> _car(const std::vector<T>& x, T& EXECUTION_TIME, T& ERROR)
{
    std::vector<T> ret;
    if (x.empty())
    {
        ERROR = 1;
        return std::vector<T> {};
    }
    ++EXECUTION_TIME;
    ret.emplace_back(x[0]);
    return ret;
}

std::vector<T> _last(const std::vector<T>& x, T& EXECUTION_TIME, T& ERROR)
{
    std::vector<T> ret;
    if (x.empty())
    {
        ERROR = 1;
        return std::vector<T> {};
    }
    ++EXECUTION_TIME;
    ret.emplace_back(x.back());
    return ret;
}

std::vector<T> _cdr(const std::vector<T>& x, T& EXECUTION_TIME, T& ERROR)
{
    std::vector<T> ret(x);
    if (x.empty())
    {
        ERROR = 1;
        return std::vector<T> {};
    }
    ++EXECUTION_TIME;
    ret.erase(ret.begin());
    return ret;
}


std::vector<T> _cons(const std::vector<T>& x, const std::vector<T>& y, T& EXECUTION_TIME, T& ERROR)
{
    std::vector<T> ret(x);
    if (x.empty())
    {
        ERROR = 1;
        return std::vector<T> {};
    }
    ++EXECUTION_TIME;
    ret.insert(ret.end(), y.begin(), y.end());
    return ret;
}

std::vector<T> _reverse(const std::vector<T>& x, T& EXECUTION_TIME, T& ERROR)
{
    std::vector<T> ret(x);
    EXECUTION_TIME += static_cast<T>(x.size());
    std::reverse(ret.begin(), ret.end());
    return ret;
}

std::vector<T> _sort(const std::vector<T>& x, T& EXECUTION_TIME, T& ERROR)
{
    std::vector<T> ret(x);
    std::sort(ret.begin(), ret.end());
    return ret;
}


