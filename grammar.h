#ifndef __GRAMMAR_H
#define __GRAMMAR_H

#include <algorithm>
#include <vector>
#include <string>
#include <stack>
#include "ast.h"
#include "random.h"
#include "type.h"
#include "container.h"


/* New Version!!!!


<real> : L(<list>, <f_lambda>, <g_lambda>) | <real> add/sub/mul/div/mod <real> | -1 / 0 / 1 | car/last(<list>) | IF(<bool>, <real>, <real>)

<f_lambda> : IF(<f_bool>, <f_lambda>, <f_lambda>) | <f_lambda> add/sub/mul/div/mod <f_lambda> | current | last | -1 / 0 / 1
<g_lambda> : IF(<g_bool>, <g_lambda>, <g_lambda>) | <g_lambda> add/sub/mul/div/mod <g_lambda> | current | -1 / 0 / 1

<list> : reverse(<list>) | cdr(<list>) | input | append(<list>, <list>) | mapcar(<g_lambda>, <list>) | filter(<g_bool>, <list>) | cons(<real>, <list>)

<bool> :  <bool> and/or <bool> | not <bool> | <real> >/>=/==/</<=/!= <real>

<f_bool> : <f_bool> and/or <f_bool> | not <f_bool> | current >/>=/==/</<=/!= <f_lambda>

<g_bool> : <g_bool> and/or <g_bool> | not <g_bool> | current >/>=/==/</<=/!= <g_lambda>


*/


T _const_gen();

ASTNode* _real(ASTNode *, T depth , T method);
ASTNode* _bool(ASTNode *, T depth , T method);
ASTNode* _list(ASTNode *, T depth , T method);

ASTNode* _f_lambda(ASTNode *, T depth , T method, bool recur);
ASTNode* _g_lambda(ASTNode *, T depth , T method, bool recur);

ASTNode* _f_bool(ASTNode *, T depth , T method);
ASTNode* _g_bool(ASTNode *, T depth , T method);

ASTNode* _noidx_g_lambda(ASTNode *, T depth , T method, bool recur);
ASTNode* _noidx_g_bool(ASTNode *, T depth , T method);

#endif //__GRAMMAR_H
