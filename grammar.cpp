

#include "grammar.h"

#include <algorithm>
#include <vector>
#include <string>
#include <stack>
#include "ast.h"
#include "random.h"
#include "type.h"
#include "container.h"


/*!SECTION


<real> : L(<list>, <funf_real>, <fung_real>) | <real> add/sub/mul/div <real> | -1 / 0 / 1 | L(<list>, <f_real>, <fung_real>) | L(<list>, <funf_real>, <g_real>) | L(<list>, <f_real>, <g_real>)

<list> : L(<list>, <funf_list>, <fung_list>) | input | L(<list>, <f_list>, <fung_list>) | L(<list>, <funf_list>, <g_list>) | L(<list>, <f_list>, <g_list>)

<funf_real> : IF(<f_bool>, <f_real>, <f_real>) | IF(<f_bool>, <f_real>, <funf_real>)
<funf_list> : IF(<g_bool>, <f_list>, <f_list>) | IF(<g_bool>, <f_list>, <funf_list>)

<fung_real> : IF(<g_bool>, <g_real>, <g_real>) | IF(<g_bool>, <g_real>, <fung_real>)
<fung_list> : IF(<g_bool>, <g_list>, <g_list>) | IF(<g_bool>, <g_list>, <fung_list>)


<f_bool> : <f_bool> and/or <f_bool> | not <f_bool> | current >/>=/==/</<=/!= <f_real>
<f_real> : <f_real> add/sub/mul/div <f_real> | current | last | -1 / 0 / 1
<f_list> : last_list | {<g_real>, last_list}

<g_bool> : <g_bool> and/or <g_bool> | not <g_bool> | current >/>=/==/</<=/!= <g_real>
<g_real> : <g_real> add/sub/mul/div <g_real> | current | -1 / 0 / 1
<g_list> : {<g_real>}

<const> : -1 | 0 | 1



*/



/* New Version!!!!


<real> : L(<list>, <f_lambda>, <g_lambda>) | <real> add/sub/mul/div/mod <real> | -1 / 0 / 1 | car/last(<list>) | IF(<bool>, <real>, <real>)

<f_lambda> : IF(<f_bool>, <f_lambda>, <f_lambda>) | <f_lambda> add/sub/mul/div/mod <f_lambda> | current | last | -1 / 0 / 1
<g_lambda> : IF(<g_bool>, <g_lambda>, <g_lambda>) | <g_lambda> add/sub/mul/div/mod <g_lambda> | current | -1 / 0 / 1

<list> : reverse(<list>) | cdr(<list>) | input | append(<list>, <list>) | mapcar(<g_lambda>, <list>) | filter(<g_bool>, <list>) | cons(<real>, <list>)

<bool> :  <bool> and/or <bool> | not <bool> | <real> >/>=/==/</<=/!= <real>

<f_bool> : <f_bool> and/or <f_bool> | not <f_bool> | current >/>=/==/</<=/!= <f_lambda>

<g_bool> : <g_bool> and/or <g_bool> | not <g_bool> | current >/>=/==/</<=/!= <g_lambda>


*/

/*!SECTION

(Int, Int) returns Int : add, sub, mul, div, mod, max, min
(Int) returns Int : abs
(Float, Float) returns Float : add, sub, mul, div, max, min
(Float) returns Float : fabs
(Bool) returns Bool : not
(Bool, Bool) returns Bool : and, or
(Int, Int) returns Bool : gt, geq, lt, leq, eq, neq
(Float, Float) returns Bool : gt, geq, lt, leq, eq, neq
(Int) returns Float : Int2FLoat
(Float) returns Int : Float2Int
(IntList) returns Int : car
(FloatList) returns Float : car
(IntList) returns (IntList) : cdr
(FloatList) returns Float : cdr
(Int, IntList) returns IntList : cons
(Float, FloatList) returns FloatList : cons
(IntList) returns IntList : reverse
(FloatList) returns FloatList : reverse

(Bool, Int, Int) returns Int : if
(Bool, Float, Float) returns Float : if
(Bool, IntList, IntList) returns IntList : if
(Bool, FloatList, FloatList) returns FloatList : if

(LambdaInt) returns Int : mapcar
(LambdaFloat) returns Float : mapcar
(Bool, IntList) returns IntList : filter
(Bool, FloatList) returns IntList : filter

(Int) returns LambdaInt : 
(Float) returns Lambda Float :

(, LambdaInt, )

indexing, input, randomERC, constants, x, y

*/

T _const_gen()
{
    if (CURTASK == MNP || CURTASK == MPMN || CURTASK == MPTMN)
    {
        //printf("alyway return 0\n");
        return 0;
    }
    if (CURTASK == SMALLEST)
    {
        return random_state(-100, 100 + 1);
    }
    if (CURTASK == COUNTODDS)
    {
        T idx = random_state(4);
        if (idx < 3) return idx;
        return random_state(-100, 100 + 1);
    }

    if (CURTASK == LASTINDEXOFZERO)
    {
        return 0;
    }

    if (CURTASK == BASEMENT)
    {
        T idx = random_state(4);
        if (idx < 3)
        {
            return idx;
        }
        return random_state(-100, 100 + 1);
    }

    if (CURTASK == FUELCOST)
    {
        T idx = random_state(5);
        if (idx < 4)
        {
            return idx;
        }
        return random_state(6, 100000 + 1);
    }

    if (CURTASK == LUHN)
    {
        T idx = random_state(5);
        if (idx == 0)
        {
            return 0;
        }
        if (idx == 1)
        {
            return 2;
        }
        if (idx == 2)
        {
            return 9;
        }
        if (idx == 3)
        {
            return 10;
        }
        return random_state(-10, 10 + 1);
    }

    return 0;
}

// <real> : 
// IF(<bool>, <real>, <real>) |
// L(<list>, <f_real_lambda>, <g_real_lambda>) |
// car/last(<list>) | 
// <real> add/sub/mul/div/mod <real> |
// -1 / 0 / 1 | 
ASTNode* _real(ASTNode* parent = nullptr, T depth = 1, T method = GROW)
{
    std::string output_type = "real";
    
    ASTNode* program = new ASTNode();
    ASTNode *program0 = nullptr, *program1 = nullptr, *program2 = nullptr;

    program->parent = parent;
    program->node.output_type = output_type;
    /*
    while (choice == 3)
    {
        choice = random_state(5); 
    }
    */
    std::vector<T> choices;
    if (depth >= 2)
    {
        choices.emplace_back(0);
    }
    if (depth >= 1)
    {
        choices.emplace_back(1);
        choices.emplace_back(2);
        choices.emplace_back(3);
    }
    if (method == GROW || (method == FULL && choices.empty()))
    {
        choices.emplace_back(4);
    }

    T choice = choices[random_state(choices.size())];

    //if (depth >= 2 && 0 <= choice && choice <= 0)
    if (choice == 0)
    {
        program0 = _bool(program, depth - 1, method);
        program1 = _real(program, depth - 1, method);
        program2 = _real(program, depth - 1, method);
        program->node = make_function3(output_type, "if");
        program->children = std::vector<ASTNode*> {program0, program1, program2};
    }
    //else if (depth >= 1 && 0 <= choice && choice <= 1) // >= 1
    else if (choice == 1)
    {
        program0 = _list(program, depth - 1, method);
        program1 = _f_lambda(program, std::min(depth - 1, 3), method, false);
        program2 = _g_lambda(program, std::min(depth - 1, 3), method, false);
        program->node = make_loop(output_type, "L");
        program->children = std::vector<ASTNode*> {program0, program1, program2};
    }
    //else if (depth >= 1 && 0 <= choice && choice <= 2)
    else if (choice == 2)
    {
        program0 = _list(program, depth - 1, method);
        std::vector<std::string> cands {"car", "last"};
        choice = random_state((T)cands.size());
        program->node = make_function1(output_type, cands[choice]);
        program->children = std::vector<ASTNode*> {program0};
    }
    //else if (depth >= 1 && 0 <= choice && choice <= 3)
    else if (choice == 3)
    {
        program0 = _real(program, depth - 1, method);
        program1 = _real(program, depth - 1, method);
        std::vector<std::string> cands {"add", "sub", "mul", "div", "mod"};
        choice = random_state((T)cands.size());
        program->node = make_function2(output_type, cands[choice]);
        program->children = std::vector<ASTNode*> {program0, program1};
    }
    else
    {
        program->node = make_value(output_type, _const_gen());
    }
    return program;
}

// <bool> : 
// <bool> and/or <bool> |
// not <bool> | 
// <real> >/>=/==/</<=/!= <real>
ASTNode* _bool(ASTNode* parent = nullptr, T depth = 1, T method = GROW)
{
    std::string output_type = "bool";
    //T choice = random_state(3);
    ASTNode* program = new ASTNode();
    ASTNode *program0 = nullptr, *program1 = nullptr;

    program->parent = parent;
    program->node.output_type = output_type;

    std::vector<T> choices;
    if (depth >= 2)
    {
        choices.emplace_back(0);
        choices.emplace_back(1);
    }
    if (depth >= 1)
    {
        choices.emplace_back(2);
    }

    T choice = choices[random_state(choices.size())];

    //if (depth >= 2 && 0 <= choice && choice <= 0)
    if (choice == 0)
    {
        program0 = _bool(program, depth - 1, method);
        program1 = _bool(program, depth - 1, method);
        std::vector<std::string> cands {"and", "or"};
        choice = random_state((int)cands.size());
        program->node = make_function2(output_type, cands[choice]);
        program->children = std::vector<ASTNode*> {program0, program1};
    }
    //else if (depth >= 2 && 0 <= choice && choice <= 1) // >= 1
    else if (choice == 1)
    {
        program0 = _bool(program, depth - 1, method);
        program->node = make_function1(output_type, "not");
        program->children = std::vector<ASTNode*> {program0};
    }
    //else if (depth >= 1 && 0 <= choice && choice <= 2)
    else if (choice == 2)
    {
        program0 = _real(program, depth - 1, method);
        program1 = _real(program, depth - 1, method);
        std::vector<std::string> cands{"geq", "gt", "eq", "leq", "lt", "neq"};
        choice = random_state((int)cands.size());
        program->node = make_function2(output_type, cands[choice]);
        program->children = std::vector<ASTNode*> {program0, program1};
    }
    else
    {
        assert(0);
    }
    return program;
}

// <list> : 
// filter(<g_bool>, <list>) |
// mapcar(<g_lambda>, <list>) | 
// reverse(<list>) | 
// cdr(<list>) | 
// append(<list>, <list>) | 
// input | 
ASTNode *_list(ASTNode *parent = nullptr, T depth = 1, T method = GROW)
{
    std::string output_type = "list";
    //T choice = random_state(5);
    ASTNode* program = new ASTNode();
    ASTNode *program0 = nullptr, *program1 = nullptr;

    program->parent = parent;
    program->node.output_type = output_type;

    std::vector<T> choices;
    if (depth >= 2)
    {
        choices.emplace_back(0);
    }
    if (depth >= 1)
    {
        choices.emplace_back(1);
        choices.emplace_back(2);
        choices.emplace_back(3);
        //choices.emplace_back(4);
    }
    if (method == GROW || (method == FULL && choices.empty()))
    {
        choices.emplace_back(4);
        //choices.emplace_back(5);
    }

    T choice = choices[random_state(choices.size())];


    //if (depth >= 2 && 0 <= choice && choice <= 0)
    if (choice == 0)
    {
        //program0 = _g_bool(program, std::min(depth - 1, 3), method);
        program0 = _noidx_g_bool(program, std::min(depth - 1, 3), method);
        program1 = _list(program, depth - 1, method);
        program->node = make_filter(output_type, "filter");
        program->children = std::vector<ASTNode*> {program0, program1};
    }
    //else if (depth >= 1 && 0 <= choice && choice <= 1)
    else if (choice == 1)
    {
        //program0 = _g_lambda(program, std::min(depth - 1, 3), method, false);
        program0 = _noidx_g_lambda(program, std::min(depth - 1, 3), method, false);
        program1 = _list(program, depth - 1, method);
        program->node = make_mapcar(output_type, "mapcar");
        program->children = std::vector<ASTNode*> {program0, program1};
    }
    
    //else if (depth >= 1 && 0 <= choice && choice <= 2)
    else if (choice == 2)
    {
        program0 = _real(program, depth - 1, method);
        program1 = _list(program, depth - 1, method);
        program->node = make_function2(output_type, "cons");
        program->children = std::vector<ASTNode*> {program0, program1};
    }
    
    //else if (depth >= 1 && 0 <= choice && choice <= 3)
    else if (choice == 3)
    {
        program0 = _list(program, depth - 1, method);
        program->node = make_function1(output_type, "cdr");
        program->children = std::vector<ASTNode*> {program0};
    }
    /*
    else if (choice == 4)
    {
        program0 = _list(program, depth - 1);
        program->node = make_function1(output_type, "sort");
        program->children = std::vector<ASTNode*> {program0};
    }
    */
    /*
    //else if (depth >= 1 && 0 <= choice && choice <= 4)
    else if (choice == 4)
    {
        program0 = _list(program, depth - 1);
        program->node = make_function1(output_type, "reverse");
        program->children = std::vector<ASTNode*> {program0};
    }
    */
    /*
    else if (depth >= 1 && 0 <= choice && choice <= 5)
    else if (choice == 5)
    {
        program0 = _list(program, depth - 1);
        program1 = _list(program, depth - 1);
        program->node = make_function2(output_type, "append");
        program->children = std::vector<ASTNode*> {program0, program1};
    }
    */
    else
    {
        program->node = make_input_list(output_type);
    }
    return program;
}


bool check_f_lambda(ASTNode *root, bool &ok, bool first_layer = true)
{
    if (first_layer == true)
    {
        ok = true; // init
    }
    if (root->children.empty())
    {
        if (root->node.instance == Container::LAMBDA_X || root->node.instance == Container::LAMBDA_Y)
        {
            return true;
        }
        if (first_layer == true)
        {
            ok = false;
        }
        return false;
    }
    bool flag = false;
    for (size_t i = 0; i < root->children.size(); ++i)
    {
        if (check_f_lambda(root->children[i], ok, false) == true)
        {
            flag = true;
        }
        if (ok == false)
        {
            break;
        }
    }
    if (flag == false)
    {
        ok = false;
    }
    return flag;
}

// <f_lambda> :
// IF(<f_bool>, <f_lambda>, <f_lambda>) | 
// <f_lambda> add/sub/mul/div/mod <f_lambda> | 
// current | 
// last | 
// -1 / 0 / 1
ASTNode *_f_lambda(ASTNode *parent = nullptr, T depth = 1, T method = GROW, bool recur = false)
{
    //printf("hello, depth = %d, recur = %s\n", depth, recur ? "true" : "false");
    method = GROW;
    std::string output_type = "f_lambda";
    //T choice = random_state(6);

    ASTNode* program = new ASTNode();
    ASTNode *program0 = nullptr, *program1 = nullptr, *program2 = nullptr;

    program->parent = parent;
    program->node.output_type = output_type;

    std::vector<T> choices;
    if (depth >= 2)
    {
        choices.emplace_back(0);
    }
    if (depth >= 1)
    {
        choices.emplace_back(1);
    }
    if (method == GROW || (method == FULL && choices.empty()))
    {
        choices.emplace_back(2);
        choices.emplace_back(3);
        choices.emplace_back(4);
        choices.emplace_back(5);
    }

    T choice = choices[random_state(choices.size())];

    //printf("depth = %d\n", depth);
    //if (depth >= 2 && 0 <= choice && choice <= 0)
    if (choice == 0)
    {
        program0 = _f_bool(program, depth - 1, method);
        program1 = _f_lambda(program, depth - 1, method, true);
        program2 = _f_lambda(program, depth - 1, method, true);
        program->node = make_function3(output_type, "if");
        program->children = std::vector<ASTNode*> {program0, program1, program2};
    }
    //else if (depth >= 1 && 0 <= choice && choice <= 1)
    else if (choice == 1)
    {
        program0 = _f_lambda(program, depth - 1, method, true);
        program1 = _f_lambda(program, depth - 1, method, true);
        std::vector<std::string> cands{"add", "sub", "mul", "div", "mod"};
        choice = random_state((int)cands.size());
        program->node = make_function2(output_type, cands[choice]);
        program->children = std::vector<ASTNode*> {program0, program1};
    }
    //else if (0 <= choice && choice <= 2)
    else if (choice == 2)
    {
        program->node = make_lambda_x(output_type);
    }
    //else if (0 <= choice && choice <= 3)
    else if (choice == 3)
    {
        program->node = make_lambda_y(output_type);
    }
    //else if (0 <= choice && choice <= 4)
    else if (choice == 4)
    {
        program->node = make_indexing(output_type);
    }
    //else if (0 <= choice && choice <= 5)
    else if (choice == 5)
    {
        program->node = make_value(output_type, _const_gen());
    }
    else
    {
        assert(0);
    }

    if (false && recur == false)
    {
        // To check all functions in lambdaf have variants.
        std::vector<ASTNode*> astNodes = program->flatten();
        bool hasX = false, hasY = false;
        for (size_t i = 0; i < astNodes.size(); ++i)
        {
            if (astNodes[i]->node.instance == Container::LAMBDA_X)
            {
                hasX = true;
            }
            else if (astNodes[i]->node.instance == Container::LAMBDA_Y)
            {
                hasY = true;
            }
        }
        //printf("hasX = %s, hasY = %s, depth = %d\n", hasX?"true":"false", hasY?"true":"false", depth);
        if (depth == 0 && hasX == false)
        {
            program->delete_children();
            delete program;
            return _f_lambda(parent, depth, method, false);
        }
        else if (depth > 0 && ( hasX == false || hasY == false ) )
        {
            program->delete_children();
            delete program;
            return _f_lambda(parent, depth, method, false);
        }
        /*
        else if (depth > 0)
        {
            bool ok = true;
            check_f_lambda(program, ok);
            if (ok == false)
            {
                program->delete_children();
                delete program;
                return _f_lambda(parent, depth, method, false);
            }
        }
        */
        
    }
    //printf("return hrr\n");
    //printf("program = %p\n", program);
    return program;
}

// <g_lambda> : IF(<g_bool>, <g_lambda>, <g_real_lambda>) | <g_lambda> add/sub/mul/div/mod <g_lambda> | current | -1 / 0 / 1
ASTNode *_g_lambda(ASTNode *parent = nullptr, T depth = 1, T method = GROW, bool recur = false)
{
    method = GROW;
    std::string output_type = "g_lambda";
    //T choice = random_state(5);

    ASTNode* program = new ASTNode();
    ASTNode *program0 = nullptr, *program1 = nullptr, *program2 = nullptr;

    program->parent = parent;
    program->node.output_type = output_type;

    std::vector<T> choices;
    if (depth >= 2)
    {
        choices.emplace_back(0);
    }
    if (depth >= 1)
    {
        choices.emplace_back(1);
    }
    if (method == GROW || (method == FULL && choices.empty()))
    {
        choices.emplace_back(2);
        choices.emplace_back(3);
        choices.emplace_back(4);
    }

    T choice = choices[random_state(choices.size())];

    //printf("depth = %d\n", depth);
    //if (depth >= 2 && 0 <= choice && choice <= 0)
    if (choice == 0)
    {
        program0 = _g_bool(program, depth - 1, method);
        program1 = _g_lambda(program, depth - 1, method, true);
        program2 = _g_lambda(program, depth - 1, method, true);
        program->node = make_function3(output_type, "if");
        program->children = std::vector<ASTNode*> {program0, program1, program2};
    }
    //else if (depth >= 1 && 0 <= choice && choice <= 1)
    else if (choice == 1)
    {
        program0 = _g_lambda(program, depth - 1, method, true);
        program1 = _g_lambda(program, depth - 1, method, true);
        std::vector<std::string> cands{"add", "sub", "mul", "div", "mod"};
        choice = random_state((int)cands.size());
        program->node = make_function2(output_type, cands[choice]);
        program->children = std::vector<ASTNode*> {program0, program1};
    }
    //else if (0 <= choice && choice <= 2)
    else if (choice == 2)
    {
        program->node = make_lambda_x(output_type);
    }
    //else if (0 <= choice && choice <= 3)
    else if (choice == 3)
    {
        program->node = make_indexing(output_type);
    }
    //else if (0 <= choice && choice <= 4)
    else if (choice == 4)
    {
        program->node = make_value(output_type, _const_gen());
    }
    else
    {
        assert(0);
    }

    if (false && recur == false)
    {
        std::vector<ASTNode*> astNodes = program->flatten();
        bool hasX = false;
        for (size_t i = 0; i < astNodes.size(); ++i)
        {
            if (astNodes[i]->node.instance == Container::LAMBDA_X)
            {
                hasX = true;
            }
        }
        
        if (hasX == false )
        {
            program->delete_children();
            delete program;
            return _g_lambda(parent, depth, method, false);
        }
        /*
        else if (depth > 0)
        {
            bool ok = true;
            check_f_lambda(program, ok);
            if (ok == false)
            {
                program->delete_children();
                delete program;
                return _g_lambda(parent, depth, method, false);
            }
        }
        */
        
    }

    return program;
}


// <f_bool> : 
// not <f_bool> | 
// <f_bool> and/or <f_bool> | 
// current >/>=/==/</<=/!= <f_lambda>
ASTNode *_f_bool(ASTNode *parent = nullptr, T depth = 1, T method = GROW)
{
    std::string output_type = "f_bool";
    //T choice = random_state(3);

    ASTNode* program = new ASTNode();
    ASTNode *program0 = nullptr, *program1 = nullptr, *program2 = nullptr;

    program->parent = parent;
    program->node.output_type = output_type;


    std::vector<T> choices;
    if (depth >= 2)
    {
        choices.emplace_back(0);
        choices.emplace_back(1);
    }
    if (depth >= 1)
    {
        choices.emplace_back(2);
    }

    T choice = choices[random_state(choices.size())];


    //if (depth >= 2 && 0 <= choice && choice <= 0)
    if (choice == 0)
    {
        program0 = _f_bool(program, depth - 1, method);
        program->node = make_function1(output_type, "not");
        program->children = std::vector<ASTNode*> {program0};
    }
    //else if (depth >= 2 && 0 <= choice && choice <= 1)
    else if (choice == 1)
    {
        program0 = _f_bool(program, depth - 1, method);
        program1 = _f_bool(program, depth - 1, method);
        std::vector<std::string> cands{"and", "or"};
        choice = random_state((int)cands.size());
        program->node = make_function2(output_type, cands[choice]);
        program->children = std::vector<ASTNode*> {program0, program1};
    }
    //else if (depth >= 1 && 0 <= choice && choice <= 2)
    else if (choice == 2)
    {
        program0 = new ASTNode(program, make_lambda_x("f_lambda_fixed_x"));
        //program0 = _f_lambda(program, depth - 1, method);
        program1 = _f_lambda(program, depth - 1, method);
        std::vector<std::string> cands{"geq", "gt", "eq", "leq", "lt", "neq"};
        choice = random_state((int)cands.size());
        program->node = make_function2(output_type, cands[choice]);
        program->children = std::vector<ASTNode*> {program0, program1};
    }
    else
    {
        assert(0);
    }
    
    return program;
    
}

// <g_bool> : 
// not <g_bool> | 
// <g_bool> and/or <g_bool> | 
// current >/>=/==/</<=/!= <g_lambda>
ASTNode *_g_bool(ASTNode *parent = nullptr, T depth = 1, T method = GROW)
{
    std::string output_type = "g_bool";
    //T choice = random_state(3);

    ASTNode* program = new ASTNode();
    ASTNode *program0 = nullptr, *program1 = nullptr, *program2 = nullptr;

    program->parent = parent;
    program->node.output_type = output_type;

    std::vector<T> choices;
    if (depth >= 2)
    {
        choices.emplace_back(0);
        choices.emplace_back(1);
    }
    if (depth >= 1)
    {
        choices.emplace_back(2);
    }

    T choice = choices[random_state(choices.size())];
    //printf("choice = %d\n", choice);

    //if (depth >= 2 && 0 <= choice && choice <= 0)
    if (choice == 0)
    {
        program0 = _g_bool(program, depth - 1, method);
        program->node = make_function1(output_type, "not");
        program->children = std::vector<ASTNode*> {program0};
    }
    //else if (depth >= 2 && 0 <= choice && choice <= 1)
    else if (choice == 1)
    {
        program0 = _g_bool(program, depth - 1, method);
        program1 = _g_bool(program, depth - 1, method);
        std::vector<std::string> cands{"and", "or"};
        choice = random_state((int)cands.size());
        program->node = make_function2(output_type, cands[choice]);
        program->children = std::vector<ASTNode*> {program0, program1};
    }
    //else if (depth >= 1 && 0 <= choice && choice <= 2)
    else if (choice == 2)
    {
        program0 = new ASTNode(program, make_lambda_x("g_lambda_fixed_x"));
        //program0 = _g_lambda(program, depth - 1, method);
        program1 = _g_lambda(program, depth - 1, method);
        std::vector<std::string> cands{"geq", "gt", "eq", "leq", "lt", "neq"};
        choice = random_state((int)cands.size());
        program->node = make_function2(output_type, cands[choice]);
        program->children = std::vector<ASTNode*> {program0, program1};
    }
    else
    {
        assert(0);
    }
    
    return program;
}






// no index version

ASTNode *_noidx_g_lambda(ASTNode *parent = nullptr, T depth = 1, T method = GROW, bool recur = false)
{
    method = GROW;
    std::string output_type = "_noidx_g_lambda";
    //T choice = random_state(5);

    ASTNode* program = new ASTNode();
    ASTNode *program0 = nullptr, *program1 = nullptr, *program2 = nullptr;

    program->parent = parent;
    program->node.output_type = output_type;

    std::vector<T> choices;
    if (depth >= 2)
    {
        choices.emplace_back(0);
    }
    if (depth >= 1)
    {
        choices.emplace_back(1);
    }
    if (method == GROW || (method == FULL && choices.empty()))
    {
        choices.emplace_back(2);
        //choices.emplace_back(3);
        choices.emplace_back(4);
    }

    T choice = choices[random_state(choices.size())];

    //printf("depth = %d\n", depth);
    //if (depth >= 2 && 0 <= choice && choice <= 0)
    if (choice == 0)
    {
        program0 = _noidx_g_bool(program, depth - 1, method); // update in 0115
        program1 = _noidx_g_lambda(program, depth - 1, method, true);
        program2 = _noidx_g_lambda(program, depth - 1, method, true);
        program->node = make_function3(output_type, "if");
        program->children = std::vector<ASTNode*> {program0, program1, program2};
    }
    //else if (depth >= 1 && 0 <= choice && choice <= 1)
    else if (choice == 1)
    {
        program0 = _noidx_g_lambda(program, depth - 1, method, true);
        program1 = _noidx_g_lambda(program, depth - 1, method, true);
        std::vector<std::string> cands{"add", "sub", "mul", "div", "mod"};
        choice = random_state((int)cands.size());
        program->node = make_function2(output_type, cands[choice]);
        program->children = std::vector<ASTNode*> {program0, program1};
    }
    //else if (0 <= choice && choice <= 2)
    else if (choice == 2)
    {
        program->node = make_lambda_x(output_type);
    }
    //else if (0 <= choice && choice <= 3)
    else if (choice == 3)
    {
        assert(0);
        program->node = make_indexing(output_type);
    }
    //else if (0 <= choice && choice <= 4)
    else if (choice == 4)
    {
        program->node = make_value(output_type, _const_gen());
    }
    else
    {
        assert(0);
    }

    if (false && recur == false)
    {
        std::vector<ASTNode*> astNodes = program->flatten();
        bool hasX = false;
        for (size_t i = 0; i < astNodes.size(); ++i)
        {
            if (astNodes[i]->node.instance == Container::LAMBDA_X)
            {
                hasX = true;
            }
        }
        
        if (hasX == false )
        {
            program->delete_children();
            delete program;
            return _g_lambda(parent, depth, method, false);
        }
        /*
        else if (depth > 0)
        {
            bool ok = true;
            check_f_lambda(program, ok);
            if (ok == false)
            {
                program->delete_children();
                delete program;
                return _g_lambda(parent, depth, method, false);
            }
        }
        */
        
    }

    return program;
}


// <g_bool> : 
// not <g_bool> | 
// <g_bool> and/or <g_bool> | 
// current >/>=/==/</<=/!= <g_lambda>
ASTNode *_noidx_g_bool(ASTNode *parent = nullptr, T depth = 1, T method = GROW)
{
    std::string output_type = "_noidx_g_bool";
    //T choice = random_state(3);

    ASTNode* program = new ASTNode();
    ASTNode *program0 = nullptr, *program1 = nullptr, *program2 = nullptr;

    program->parent = parent;
    program->node.output_type = output_type;

    std::vector<T> choices;
    if (depth >= 2)
    {
        choices.emplace_back(0);
        choices.emplace_back(1);
    }
    if (depth >= 1)
    {
        choices.emplace_back(2);
    }

    T choice = choices[random_state(choices.size())];
    //printf("choice = %d\n", choice);

    //if (depth >= 2 && 0 <= choice && choice <= 0)
    if (choice == 0)
    {
        program0 = _noidx_g_bool(program, depth - 1, method);
        program->node = make_function1(output_type, "not");
        program->children = std::vector<ASTNode*> {program0};
    }
    //else if (depth >= 2 && 0 <= choice && choice <= 1)
    else if (choice == 1)
    {
        program0 = _noidx_g_bool(program, depth - 1, method);
        program1 = _noidx_g_bool(program, depth - 1, method);
        std::vector<std::string> cands{"and", "or"};
        choice = random_state((int)cands.size());
        program->node = make_function2(output_type, cands[choice]);
        program->children = std::vector<ASTNode*> {program0, program1};
    }
    //else if (depth >= 1 && 0 <= choice && choice <= 2)
    else if (choice == 2)
    {
        program0 = new ASTNode(program, make_lambda_x("_noidx_g_lambda_fixed_x"));
        //program0 = _g_lambda(program, depth - 1, method);
        program1 = _noidx_g_lambda(program, depth - 1, method);
        std::vector<std::string> cands{"geq", "gt", "eq", "leq", "lt", "neq"};
        choice = random_state((int)cands.size());
        program->node = make_function2(output_type, cands[choice]);
        program->children = std::vector<ASTNode*> {program0, program1};
    }
    else
    {
        assert(0);
    }
    
    return program;
}


