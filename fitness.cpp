
#include "fitness.h"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cassert>
#include <cmath>
#include <queue>
#include <string>
#include <utility>

#include "ast.h"
#include "type.h"

typedef long double DOUBLE;

DOUBLE calPearson(const std::vector<T>& x, const std::vector<T>& y)
{
    DOUBLE avgx = 0.0, avgy = 0.0;
    DOUBLE num = 0.0, domx = 0.0, domy = 0.0;
    DOUBLE r = 0.0;
    size_t n = x.size();
    assert(x.size() == n && y.size() == n);
    for (size_t i = 0; i < n; ++i)
    {
        avgx += static_cast<DOUBLE>(x[i]);
        avgy += static_cast<DOUBLE>(y[i]);
    }
    avgx /= static_cast<DOUBLE>(n);
    avgy /= static_cast<DOUBLE>(n);
    for (size_t i = 0; i < n; ++i)
    {
        num += (static_cast<DOUBLE>(x[i]) - avgx) * (static_cast<DOUBLE>(y[i] - avgy));
        domx += powl((static_cast<DOUBLE>(x[i]) - avgx), 2.0);
        domy += powl((static_cast<DOUBLE>(y[i]) - avgy), 2.0);
    }
    domx = sqrtl(domx);
    domy = sqrtl(domy);
    if (fabsl(domx * domy) < 1e-6)
    {
        return 0.0;
    }
    r = num / (domx * domy);
    return r;
}

void calLine(const std::vector<T>& x, const std::vector<T>& y, DOUBLE& a, DOUBLE& b, bool& vert)
{
    DOUBLE ux = 0.0, uy = 0.0;
    DOUBLE num = 0.0, dom = 0.0;
    size_t n = x.size();
    assert((int)y.size() == n);
    for (size_t i = 0; i < n; ++i)
    {
        ux += (DOUBLE)x[i];
        uy += (DOUBLE)y[i];
    }
    ux /= (DOUBLE)n;
    uy /= (DOUBLE)n;

    for (size_t i = 0; i < n; ++i)
    {
        num += (DOUBLE)(x[i] - ux) * (DOUBLE)(y[i] - uy);
        dom += powl((DOUBLE)(x[i] - ux), 2.0);
    }

    if (fabsl(dom) < 1e-9)
    {
        vert = true;
        return;
    }
    vert = false;
    b = num / dom;
    a = uy - b * ux;
    return;
}

void maeTest(const std::vector<std::vector<T>>& X, const std::vector<T>& y, const ASTNode* root)
{
    assert(X.size() == y.size());
    std::vector<T> predy(y.size());

    for (size_t i = 0; i < X.size(); ++i)
    {
        T ERROR = 0;
        T EXECUTION_TIME = 0;
        std::vector<T> res = root->calc(X[i], EXECUTION_TIME, ERROR);
        if (ERROR == 1)
        {
            printf("Error, your program %s has error.\n", root->print().c_str());
            return;
        }
        
        if (res.empty())
        {
            printf("Error, your program %s has error.\n", root->print().c_str());
            return;
        }
        else
        {
            predy[i] = res[0];
        }

        if (res.empty())
        {
            printf("Error, your program %s has error.\n", root->print().c_str());
            return;
        }
    }

    DOUBLE r = calPearson(predy, y);

    long long error = static_cast<long long>(fabsl(1.0 - fabsl(r)) * static_cast<DOUBLE>(1000000000000.0));
    if (error == 0)
    {
        DOUBLE a, b;
        bool vert;
        calLine(predy, y, a, b, vert);
        
        bool nega = false, negb = false;
        if (a < 0 && fabsl(a) > 1e-9)
        {
            nega = true;
            a = -a;
        }
        if (b < 0 && fabsl(b) > 1e-9)
        {
            negb = true;
            b = -b;
        }
        DOUBLE recr = 10.0;
        long long coef = 1;
        
        for (long long i = 1; i <= 2147483647ll; ++i)
        {
            DOUBLE na = (DOUBLE)i * a, nb = (DOUBLE)i * b;
            DOUBLE ra = fmodl(na + 1e-9, 1.0), rb = fmodl(nb + 1e-9, 1.0);
            
            if (recr > std::max(fabsl(ra), fabsl(rb)))
            {
                recr = std::max(fabsl(ra), fabsl(rb));
                coef = i;
            }
            if (fabsl(ra) < 2e-9 && fabsl(rb) < 2e-9)
            {
                coef = (long long)i;
                break;
            }
        }
        
        a *= (DOUBLE)coef; a += 1e-9;
        b *= (DOUBLE)coef; b += 1e-9;
        long long ae = 0;
        long long inta = (long long)a, intb = (long long)b;
        if (nega) inta = -inta;
        if (negb) intb = -intb;
        printf("Program after linear scaling: div(add(%lld, mul(%lld, %s)), %lld)\n", inta, intb, root->print().c_str(), coef);
        for (size_t i = 0; i < X.size(); ++i)
        {
            long long cur = vert ? (long long)predy[i] : (inta + intb * (long long)predy[i]) / coef;
            ae += llabs(cur - (long long)y[i]);
        }
        printf("Absolute error: %lld\n", ae);
    }
    else
    {
        printf("Absolute error: 2147483637\n");
        printf("Error, the PCC of %s is %lld.\n", root->print().c_str(), error);
    }
    return;
}


std::pair<long long, long long> _count(const std::vector<std::vector<T>>& X, const std::vector<T>& y, const ASTNode* root, std::vector<T>& ypred)
{
    long long ae = 0;
    long long error = 0;
    T exeTime = 0;

    std::queue<const ASTNode*> q;

    assert(X.size() == y.size());

    ypred.clear();
    ypred.resize(X.size());
    std::fill(ypred.begin(), ypred.end(), -2147483648);

    if (root->depth() > 15)
    {
        return std::make_pair(1ll << 62, 0);
    }
    
    q.push(root);

    while (q.size())
    {
        const ASTNode *p = q.front(); q.pop();

        if ( ( p->node.output_type.find("f_lambda") != std::string::npos || p->node.output_type.find("g_lambda") != std::string::npos)
            && p->depth() > 3 )
        {
            return std::make_pair(1ll << 62, 0);
        }
        
        for (const ASTNode* child : p->children)
        {
            q.push(child);
        }
        
    }

    
    for (size_t i = 0; i < X.size(); ++i)
    {
        T ERROR = 0;
        T EXETIME = 0;
        std::vector<T> res = root->calc(X[i], EXETIME, ERROR);

        exeTime = std::max(exeTime, EXETIME);

        if (ERROR == 1 || EXETIME > 5000 || res.empty())
        {
            error = 1ll << 62;
            std::fill(ypred.begin(), ypred.end(), -2147483648);
            return std::make_pair(error, exeTime);
        }
        
        ypred[i] = res[0];
        
        ae += llabs(static_cast<long long>(ypred[i]) - static_cast<long long>(y[i]));
        
    }
    
    
    DOUBLE r = calPearson(ypred, y);
    
    error = static_cast<long long>(fabsl(1.0 - fabsl(r)) * static_cast<DOUBLE>(1000000000000.0));
    
    return std::make_pair(error, exeTime);
}


