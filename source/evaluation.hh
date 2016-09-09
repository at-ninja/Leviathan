// environment module
// Andrew Thomas

//#include <stack>
#include <vector>
#include "builtin.hh"

#ifndef EVAL_H
#define EVAL_H

#define BUILTIN_NUMBER 13

typedef std::string String;

namespace Leviathan
{
    class Evaluator
    {
        private:
            Lexeme* globalEnv;
            std::map<const char*, int> typeHeirarchy;
            String fileDir;
            

        
            void loadBuiltIns() 
            {
                String ids[BUILTIN_NUMBER] = {
                    "print", "P",
                    "println", "PL",
                    "range", "R",
                    "int",
                    "real",
                    "string",
                    "charAt",
                    "len",
                    "input",
                    "rand"
                };
                Lexeme* (*fval[BUILTIN_NUMBER]) (Lexeme*, Lexeme*) = {
                    &evalPrint, &evalPrint,
                    &evalPrintln, &evalPrintln,
                    &evalRange, &evalRange,
                    &evalIntegerFunc,
                    &evalRealFunc,
                    &evalStringFunc,
                    &evalCharAt,
                    &evalLen,
                    &evalInput,
                    &evalRand
                };
                Lexeme* funcLexeme;
                Lexeme* id;
                
                for (int i = 0; i < BUILTIN_NUMBER; i++)
                {
                    funcLexeme = new Lexeme(BUILTIN, 0, fval[i]);
                    id = new Lexeme(IDENTIFIER, 0, ids[i]);
                    insert(id, funcLexeme, globalEnv);
                }
            }
        
            void loadTypeHeirarchy()
            {
                typeHeirarchy[BOOLEAN] = 0;
                typeHeirarchy[INTEGER] = 1;
                typeHeirarchy[REAL] = 2;
            }
        
            Lexeme* eval(Lexeme* t, Lexeme* e)
            {
                if (t == NULL) return NULL;
                
                // Basic types of the language
                if (t->type == INTEGER) return t;
                else if (t->type == STRING) return t;
                else if (t->type == BOOLEAN) return t;
                else if (t->type == REAL) return t;
                else if (t->type == NIL) return t;
                else if (t->type == THIS) return e;
        
                // operators and statements
                else if (t->type == STATEMENTLIST) return evalStatementList(t, e);
                else if (t->type == ARGLIST) return evalArgs(t, e);
                else if (t->type == IF) return evalIf(t, e);
                else if (t->type == WHILE) return evalWhile(t, e);
                else if (t->type == ARRAYDEF) return evalArray(t, e);
                else if (t->type == ARRAYREF) return evalArrayRef(t, e);
                else if (t->type == FOR) return evalFor(t, e);
                else if (t->type == NOT) return evalNot(t, e);
                else if (t->type == NEGATIVE) return evalNegative(t, e);
                else if (t->type == DOT) return evalDot(t, e);
                else if (t->type == AND) return evalAnd(t, e);
                else if (t->type == EQUALITY) return evalEquality(t, e);
                else if (t->type == GTHAN) return evalGThan(t, e);
                else if (t->type == LTHAN) return evalLThan(t, e);
                else if (t->type == OR) return evalOr(t, e);
                else if (t->type == DIVIDE) return evalDivide(t, e);
                else if (t->type == MOD) return evalMod(t, e);
                else if (t->type == TIMES) return evalTimes(t, e);
                else if (t->type == MINUS) return evalMinus(t, e);
                else if (t->type == PLUS) return evalPlus(t, e);
                else if (t->type == ASSIGN) return assign(t, e);
                else if (t->type == IDENTIFIER) return lookup(t, e);
                else if (t->type == VAR) return evalVar(t, e);
                else if (t->type == FUNCCALL) return evalCall(t, e);
                else if (t->type == FUNCTION) return evalDef(t, e);
                else if (t->type == CLASS) return evalClassDef(t, e);
                else if (t->type == INCLUDE) return evalInclude(t, e);
        
                throw std::runtime_error(
                    "The Kraken rears his ugly head at a Runtime error!\nAn unknown parse tree of type "
                    + String(t->type)
                    + " was found (line "
                    + std::to_string(t->getLineNumber())
                    + ")");
            }
            
            Lexeme* evalInclude(Lexeme* t, Lexeme* env) // This builtin needs to be here so it can run more lv code within the global scope
            {
                Lexeme* inc = eval(t->left, env);
                
                if (!(inc->type == STRING))
                    throw std::runtime_error(
                        "The Kraken rears his ugly head at a Runtime error!\nInclude requires a STRING. Found "
                        + String(inc->type)
                        + " instead (line "
                        + std::to_string(inc->getLineNumber())
                        + ")");
                
                // We want to create a filepath relative to the original source file if possible
                String oldFilePath = this->fileDir;
                
                String filePath;
                if (inc->svalue.find_first_of("/\\") == 0)
                    filePath = inc->svalue; // We already have a phyical path
                else
                    filePath = this->fileDir + inc->svalue; // append what we have to the relative path
                
                // now, extract the relative filepath
                int found = filePath.find_last_of("/\\");
        
                std::string dir;
                if (found >= (int) filePath.length())
                    dir = "./";
                else
                    dir = filePath.substr(0, found + 1);
                
                
                // since we will be evaluating from a new place, update the filePath
                this->fileDir = dir;
                
                Lexeme* result =  startEval(filePath);
                
                // change the filepath back
                this->fileDir = oldFilePath;
                
                return result;
                
            }
        
            Lexeme* evalNegative(Lexeme* t, Lexeme* e)
            {
                Lexeme* prim = eval(t->left, e);
                if (prim->type == INTEGER)
                {
                    return new Lexeme(INTEGER, t->getLineNumber(), -(prim->ivalue));
                }
                else if (prim->type == REAL)
                {
                    return new Lexeme(REAL, t->getLineNumber(), -(prim->rvalue));
                }
                throw std::runtime_error(
                    "The Kraken rears his ugly head at a Runtime error!\nNegative (-) requires an INTEGER or REAL. Found "
                    + String(prim->type)
                    + " instead (line "
                    + std::to_string(prim->getLineNumber())
                    + ")");
            }
        
            Lexeme* evalNot(Lexeme* t, Lexeme* e)
            {
                Lexeme* expr = eval(t->left, e);
                if (expr->type == BOOLEAN)
                    return new Lexeme(BOOLEAN, t->getLineNumber(), 1 - expr->ivalue);
                else
                    throw std::runtime_error(
                        "The Kraken rears his ugly head at a Runtime error!\nNot (!) requires an BOOLEAN. Found "
                        + String(expr->type)
                        + " instead (line "
                        + std::to_string(expr->getLineNumber())
                        + ")");
            }
        
            Lexeme* evalFor(Lexeme* t, Lexeme* e)
            {
                Lexeme* id = t->left;
                Lexeme* arr = eval(t->right->left, e); // this needs to be an array
                if (!(arr->type == ARRAY))
                    throw std::runtime_error(
                        "The Kraken rears his ugly head at a Runtime error!\nFor statement requires an ARRAY. Found "
                        + String(arr->type)
                        + " instead (line "
                        + std::to_string(arr->getLineNumber())
                        + ")");
                
                std::vector<Lexeme*>vect = arr->vectorPointer;
                
                Lexeme* stat = t->right->right;
                
                // we need a new scope to put the local variable in
                
                Lexeme* s /* new scope */ = extendEnv(NULL, NULL, e);
                insert(id, new Lexeme(NIL, id->getLineNumber()), s);
                
                
                for (int i = 0; i < (int) vect.size(); i++)
                {
                    updateEnv(id, vect.at(i), s);
                    
                    // evaluate the statement in s
                    eval(stat, s);
                }
                
                return new Lexeme(NIL, t->getLineNumber());
            }
        
            Lexeme* evalArrayRef(Lexeme* t, Lexeme* e)
            {
                Lexeme* array = eval(t->left, e);
                Lexeme* index = eval(t->right, e);
                
                // We want array to be of type ARRAY and index to be an INTEGER
                if (array->type == ARRAY and index->type == INTEGER)
                {
                    if (index->ivalue < (int) array->vectorPointer.capacity() &&
                       index->ivalue >= 0)
                        return array->vectorPointer.at(index->ivalue);
                    else
                        throw std::runtime_error(
                            "The Kraken rears his ugly head at a Runtime error!\nOut of bounds ARRAY reference (line "
                            + std::to_string(index->getLineNumber())
                            + ")");
                }
                else
                    throw std::runtime_error(
                        "The Kraken rears his ugly head at a Runtime error!\nInvalid ARRAY reference. Expected types ARRAY and INTEGER. Found types "
                        + String(array->type)
                        + " and "
                        + String(index->type)
                        + "(line "
                        + std::to_string(array->getLineNumber())
                        + ")");
            }
        
            Lexeme* evalArray(Lexeme* t, Lexeme* e)
            {
                int size = listLength(t->left); //size of array
                Lexeme* eargs = eval(t->left, e); //eval the args
                int arrLineNumber = t->getLineNumber();
                std::vector<Lexeme*> vect(size);
                
                
                for (int i = 0; i < size; i++)
                {
                    vect[i] = car(eargs);
                    eargs = cdr(eargs);
                }
                
                
                return new Lexeme(ARRAY, arrLineNumber, vect);
            }
        
            Lexeme* evalWhile(Lexeme* t, Lexeme* e)
            {
                Lexeme* condition = car(t);
                Lexeme* evalCondition = eval(condition, e);
                while (evalCondition->type == BOOLEAN && evalCondition->ivalue == 1) // while true do
                {
                    eval(cdr(t), e);
                    evalCondition = eval(condition, e);
                }
                if (evalCondition->type != BOOLEAN)
                    throw std::runtime_error(
                        "The Kraken rears his ugly head at a Runtime error!\nWhile statement condition expected BOOLEAN. Found "
                        + String(evalCondition->type)
                        + " instead (line "
                        + std::to_string(evalCondition->getLineNumber())
                        + ")");
                
                return evalCondition;
            }
        
            Lexeme* evalBlock(Lexeme* t, Lexeme* e)
            {
                if (t->left != NULL)
                    return eval(t->left, e);
                return new Lexeme(NIL);
            }
        
            Lexeme* promoteType(Lexeme* t)
            {
                if (t->type == BOOLEAN)
                    return new Lexeme(INTEGER, t->getLineNumber(), t->ivalue);
                else if (t->type == INTEGER)
                    return new Lexeme(REAL, t->getLineNumber(), (double) t->ivalue);
                else return new Lexeme(REAL, t->getLineNumber(), t->rvalue); // Currently, REAL is the highest promotion. no need to throw an error, just return a new lexeme
            }
        
            Lexeme* evalStatementList(Lexeme* t, Lexeme* e)
            {
                Lexeme* result = NULL;
                Lexeme* statements = t;
                
                while (statements != NULL)
                {
                    result = eval(car(statements), e);
                    statements = cdr(statements);
                }
                return result;
            }
        
            Lexeme* evalArgs(Lexeme* args, Lexeme* env)
            {
                if (args == NULL) return NULL;
                return cons(GLUE, eval(car(args), env), evalArgs(cdr(args), env));
            }
        
            Lexeme* evalIf(Lexeme* t, Lexeme* env)
            {
                Lexeme* condition = eval(t->left, env);
                
                if (condition->type == BOOLEAN) 
                {
                    if (condition->ivalue == 1) // if true
                        return eval(t->right->left, env);
                    else if (t->right->right != NULL)
                        return eval(t->right->right, env); // if there is an else, run it
                    else return condition; // otherwise return the condition (false)
                }
                else
                    throw std::runtime_error(
                        "The Kraken rears his ugly head at a Runtime error!\nIf statement condition expected BOOLEAN. Found "
                        + String(condition->type)
                        + " instead (line "
                        + std::to_string(condition->getLineNumber())
                        + ")");
                
            }
        
            Lexeme* evalDot(Lexeme* t, Lexeme* e)
            {
                // for dot, we need to have an environment on the left,
                // and an identifier on the right
                // so, we evaluate the left, and don't evaluate the right
                Lexeme* left = eval(t->left, e);
                Lexeme* right = t->right;
                
                
                if (left->type == ENVIRONMENT && right->type == IDENTIFIER)
                    return lookup(right, left); // lookup the right in the left, since the left is a scope
                else 
                    throw std::runtime_error(
                        "The Kraken rears his ugly head at a Runtime error!\nUnsupported types for operation '.' of "
                        + String(left->type)
                        + " and "
                        + String(right->type)
                        + " (line "
                        + std::to_string(t->getLineNumber())
                        + ")");
                
                
            }
        
            Lexeme* evalAnd(Lexeme* t, Lexeme* e)
            {
                Lexeme* left = eval(t->left, e);
                Lexeme* right = eval(t->right, e);
                
                if (left->type == BOOLEAN && right->type == BOOLEAN)
                    return new Lexeme(BOOLEAN, t->getLineNumber(), left->ivalue && right->ivalue);
                else if (left->type == INTEGER && right->type == BOOLEAN)
                    return new Lexeme(BOOLEAN, t->getLineNumber(), left->ivalue && right->ivalue);
                else if (left->type == BOOLEAN && right->type == INTEGER)
                    return new Lexeme(BOOLEAN, t->getLineNumber(), left->ivalue && right->ivalue);
                else if (left->type == INTEGER && right->type == INTEGER)
                    return new Lexeme(BOOLEAN, t->getLineNumber(), left->ivalue && right->ivalue);
                else 
                    throw std::runtime_error(
                        "The Kraken rears his ugly head at a Runtime error!\nUnsupported types for operation 'and' of "
                        + String(left->type)
                        + " and "
                        + String(right->type)
                        + " (line "
                        + std::to_string(t->getLineNumber())
                        + ")");
            }
        
            Lexeme* evalEquality(Lexeme* t, Lexeme* e)
            {
                Lexeme* left = eval(t->left, e);
                Lexeme* right = eval(t->right, e);
                
                if ((left->type == BOOLEAN || left->type == INTEGER || left->type == REAL)
                        && (right->type == BOOLEAN || right->type == INTEGER || right->type == REAL))
                {
                    while (typeHeirarchy[left->type] != typeHeirarchy[right->type])
                    {
                        if (typeHeirarchy[left->type] < typeHeirarchy[right->type])
                            left = promoteType(left);
                        else
                            right = promoteType(right);
                    }
                    if (left->type == BOOLEAN && right->type == BOOLEAN)
                        return new Lexeme(BOOLEAN, t->getLineNumber(), left->ivalue == right->ivalue);
                    else if (left->type == INTEGER && right->type == INTEGER)
                        return new Lexeme(BOOLEAN, t->getLineNumber(), left->ivalue == right->ivalue);
                    else if (left->type == REAL && right->type == REAL)
                        return new Lexeme(BOOLEAN, t->getLineNumber(), left->rvalue == right->rvalue);
                    else 
                        throw std::runtime_error(
                        "The Kraken rears his ugly head at a Runtime error!\nUnsupported types for operation '==' of "
                        + String(left->type)
                        + " and "
                        + String(right->type)
                        + " (line "
                        + std::to_string(t->getLineNumber())
                        + ")");
                }
                //else return new Lexeme(BOOLEAN, left->shallowEquals(right));
                else if (left->type == STRING && right->type == STRING)
                    return new Lexeme(BOOLEAN, t->getLineNumber(), left->svalue.compare(right->svalue) == 0);
                
                else if (left->type == NIL && right->type == NIL)
                    return new Lexeme(BOOLEAN, t->getLineNumber(), 1); // nil == nil
                else if (left->type == ENVIRONMENT && right->type == NIL)
                    return new Lexeme(BOOLEAN, t->getLineNumber(), 0); // an evironment is already not equal to nil
                else if (left->type == NIL && right->type == ENVIRONMENT)
                    return new Lexeme(BOOLEAN, t->getLineNumber(), 0);
                else
                    return new Lexeme(BOOLEAN, t->getLineNumber(), left->equals(right));
            }
        
            Lexeme* evalGThan(Lexeme* t, Lexeme* e)
            {
                Lexeme* left = eval(t->left, e);
                Lexeme* right = eval(t->right, e);
                
                
                if ((left->type == BOOLEAN || left->type == INTEGER || left->type == REAL)
                        && (right->type == BOOLEAN || right->type == INTEGER || right->type == REAL))
                {
                    while (typeHeirarchy[left->type] != typeHeirarchy[right->type])
                    {
                        if (typeHeirarchy[left->type] < typeHeirarchy[right->type])
                            left = promoteType(left);
                        else
                            right = promoteType(right);
                    }
                    if (left->type == BOOLEAN && right->type == BOOLEAN)
                        return new Lexeme(BOOLEAN, t->getLineNumber(), left->ivalue > right->ivalue);
                    else if (left->type == INTEGER && right->type == INTEGER)
                        return new Lexeme(BOOLEAN, t->getLineNumber(), left->ivalue > right->ivalue);
                    else if (left->type == REAL && right->type == REAL)
                        return new Lexeme(BOOLEAN, t->getLineNumber(), left->rvalue > right->rvalue);
                    else 
                        throw std::runtime_error(
                        "The Kraken rears his ugly head at a Runtime error!\nUnsupported types for operation '>' of "
                        + String(left->type)
                        + " and "
                        + String(right->type)
                        + " (line "
                        + std::to_string(t->getLineNumber())
                        + ")");
                }
                else 
                    throw std::runtime_error(
                        "The Kraken rears his ugly head at a Runtime error!\nUnsupported types for operation '>' of "
                        + String(left->type)
                        + " and "
                        + String(right->type)
                        + " (line "
                        + std::to_string(t->getLineNumber())
                        + ")");
            }
        
            Lexeme* evalLThan(Lexeme* t, Lexeme* e)
            {
                Lexeme* left = eval(t->left, e);
                Lexeme* right = eval(t->right, e);
                
                
                if ((left->type == BOOLEAN || left->type == INTEGER || left->type == REAL)
                        && (right->type == BOOLEAN || right->type == INTEGER || right->type == REAL))
                {
                    while (typeHeirarchy[left->type] != typeHeirarchy[right->type])
                    {
                        if (typeHeirarchy[left->type] < typeHeirarchy[right->type])
                            left = promoteType(left);
                        else
                            right = promoteType(right);
                    }
                    if (left->type == BOOLEAN && right->type == BOOLEAN)
                        return new Lexeme(BOOLEAN, t->getLineNumber(), left->ivalue < right->ivalue);
                    else if (left->type == INTEGER && right->type == INTEGER)
                        return new Lexeme(BOOLEAN, t->getLineNumber(), left->ivalue < right->ivalue);
                    else if (left->type == REAL && right->type == REAL)
                        return new Lexeme(BOOLEAN, t->getLineNumber(), left->rvalue < right->rvalue);
                    else 
                        throw std::runtime_error(
                        "The Kraken rears his ugly head at a Runtime error!\nUnsupported types for operation '<' of "
                        + String(left->type)
                        + " and "
                        + String(right->type)
                        + " (line "
                        + std::to_string(t->getLineNumber())
                        + ")");
                }
                else 
                    throw std::runtime_error(
                        "The Kraken rears his ugly head at a Runtime error!\nUnsupported types for operation '<' of "
                        + String(left->type)
                        + " and "
                        + String(right->type)
                        + " (line "
                        + std::to_string(t->getLineNumber())
                        + ")");
            }
        
            Lexeme* evalOr(Lexeme* t, Lexeme* e)
            {
                Lexeme* left = eval(t->left, e);
                Lexeme* right = eval(t->right, e);
                
                if (left->type == BOOLEAN && right->type == BOOLEAN)
                    return new Lexeme(BOOLEAN, t->getLineNumber(), left->ivalue || right->ivalue);
                else if (left->type == INTEGER && right->type == BOOLEAN)
                    return new Lexeme(BOOLEAN, t->getLineNumber(), left->ivalue || right->ivalue);
                else if (left->type == BOOLEAN && right->type == INTEGER)
                    return new Lexeme(BOOLEAN, t->getLineNumber(), left->ivalue || right->ivalue);
                else if (left->type == INTEGER && right->type == INTEGER)
                    return new Lexeme(BOOLEAN, t->getLineNumber(), left->ivalue || right->ivalue);
                else 
                    throw std::runtime_error(
                        "The Kraken rears his ugly head at a Runtime error!\nUnsupported types for operation 'or' of "
                        + String(left->type)
                        + " and "
                        + String(right->type)
                        + " (line "
                        + std::to_string(t->getLineNumber())
                        + ")");
            }
        
            Lexeme* evalDivide(Lexeme* t, Lexeme* e)
            {
                Lexeme* left = eval(t->left, e);
                Lexeme* right = eval(t->right, e);
                
                
                if ((left->type == INTEGER || left->type == REAL)
                        && (right->type == INTEGER || right->type == REAL))
                {
                    while (typeHeirarchy[left->type] != typeHeirarchy[right->type])
                    {
                        if (typeHeirarchy[left->type] < typeHeirarchy[right->type])
                            left = promoteType(left);
                        else
                            right = promoteType(right);
                    }
                    if (left->type == INTEGER && right->type == INTEGER)
                        return new Lexeme(INTEGER, t->getLineNumber(), left->ivalue / right->ivalue);
                    else if (left->type == REAL && right->type == REAL)
                        return new Lexeme(REAL, t->getLineNumber(), left->rvalue / right->rvalue);
                    else 
                        throw std::runtime_error(
                        "The Kraken rears his ugly head at a Runtime error!\nUnsupported types for operation '/' of "
                        + String(left->type)
                        + " and "
                        + String(right->type)
                        + " (line "
                        + std::to_string(t->getLineNumber())
                        + ")");
                }
                else 
                    throw std::runtime_error(
                        "The Kraken rears his ugly head at a Runtime error!\nUnsupported types for operation '/' of "
                        + String(left->type)
                        + " and "
                        + String(right->type)
                        + " (line "
                        + std::to_string(t->getLineNumber())
                        + ")");
            }
        
            Lexeme* evalMod(Lexeme* t, Lexeme* e)
            {
                Lexeme* left = eval(t->left, e);
                Lexeme* right = eval(t->right, e);
                
               if (left->type == INTEGER && right->type == INTEGER)
                    return new Lexeme(INTEGER, t->getLineNumber(), left->ivalue % right->ivalue);
                else 
                    throw std::runtime_error(
                        "The Kraken rears his ugly head at a Runtime error!\nUnsupported types for operation '%' of "
                        + String(left->type)
                        + " and "
                        + String(right->type)
                        + " (line "
                        + std::to_string(t->getLineNumber())
                        + ")");
            }
        
            Lexeme* evalTimes(Lexeme* t, Lexeme* e)
            {
                Lexeme* left = eval(t->left, e);
                Lexeme* right = eval(t->right, e);
                
                
                if ((left->type == INTEGER || left->type == REAL)
                        && (right->type == INTEGER || right->type == REAL))
                {
                    while (typeHeirarchy[left->type] != typeHeirarchy[right->type])
                    {
                        if (typeHeirarchy[left->type] < typeHeirarchy[right->type])
                            left = promoteType(left);
                        else
                            right = promoteType(right);
                    }
                    if (left->type == INTEGER && right->type == INTEGER)
                        return new Lexeme(INTEGER, t->getLineNumber(), left->ivalue * right->ivalue);
                    else if (left->type == REAL && right->type == REAL)
                        return new Lexeme(REAL, t->getLineNumber(), left->rvalue * right->rvalue);
                    else 
                        throw std::runtime_error(
                        "The Kraken rears his ugly head at a Runtime error!\nUnsupported types for operation '*' of "
                        + String(left->type)
                        + " and "
                        + String(right->type)
                        + " (line "
                        + std::to_string(t->getLineNumber())
                        + ")");
                }
                else 
                    throw std::runtime_error(
                        "The Kraken rears his ugly head at a Runtime error!\nUnsupported types for operation '*' of "
                        + String(left->type)
                        + " and "
                        + String(right->type)
                        + " (line "
                        + std::to_string(t->getLineNumber())
                        + ")");
            }
        
            Lexeme* evalMinus(Lexeme* t, Lexeme* e)
            {
                Lexeme* left = eval(t->left, e);
                Lexeme* right = eval(t->right, e);
                
                
                if ((left->type == INTEGER || left->type == REAL)
                        && (right->type == INTEGER || right->type == REAL))
                {
                    while (typeHeirarchy[left->type] != typeHeirarchy[right->type])
                    {
                        if (typeHeirarchy[left->type] < typeHeirarchy[right->type])
                            left = promoteType(left);
                        else
                            right = promoteType(right);
                    }
                    if (left->type == INTEGER && right->type == INTEGER)
                        return new Lexeme(INTEGER, t->getLineNumber(), left->ivalue - right->ivalue);
                    else if (left->type == REAL && right->type == REAL)
                        return new Lexeme(REAL, t->getLineNumber(), left->rvalue - right->rvalue);
                    else 
                        throw std::runtime_error(
                        "The Kraken rears his ugly head at a Runtime error!\nUnsupported types for operation '-' of "
                        + String(left->type)
                        + " and "
                        + String(right->type)
                        + " (line "
                        + std::to_string(t->getLineNumber())
                        + ")");
                }
                else 
                    throw std::runtime_error(
                        "The Kraken rears his ugly head at a Runtime error!\nUnsupported types for operation '-' of "
                        + String(left->type)
                        + " and "
                        + String(right->type)
                        + " (line "
                        + std::to_string(t->getLineNumber())
                        + ")");
            }
        
            Lexeme* evalPlus(Lexeme* t, Lexeme* e)
            {
                Lexeme* left = eval(t->left, e);
                Lexeme* right = eval(t->right, e);
                
                
                if ((left->type == INTEGER || left->type == REAL)
                        && (right->type == INTEGER || right->type == REAL))
                {
                    while (typeHeirarchy[left->type] != typeHeirarchy[right->type])
                    {
                        if (typeHeirarchy[left->type] < typeHeirarchy[right->type])
                            left = promoteType(left);
                        else
                            right = promoteType(right);
                    }
                    if (left->type == INTEGER && right->type == INTEGER)
                        return new Lexeme(INTEGER, t->getLineNumber(), left->ivalue + right->ivalue);
                    else if (left->type == REAL && right->type == REAL)
                        return new Lexeme(REAL, t->getLineNumber(), left->rvalue + right->rvalue);
                    else 
                        throw std::runtime_error(
                        "The Kraken rears his ugly head at a Runtime error!\nUnsupported types for operation '+' of "
                        + String(left->type)
                        + " and "
                        + String(right->type)
                        + " (line "
                        + std::to_string(t->getLineNumber())
                        + ")");
                }
                else if (left->type == STRING && right->type == STRING)
                    return new Lexeme(STRING, t->getLineNumber(), left->svalue + right->svalue);
                else if (left->type == ARRAY && right->type == ARRAY)
                {
                    std::vector<Lexeme*> concat(left->vectorPointer);
                    concat.insert(concat.end(), right->vectorPointer.begin(), right->vectorPointer.end());
                    return new Lexeme(ARRAY, t->getLineNumber(), concat);
                }
                else 
                    throw std::runtime_error(
                        "The Kraken rears his ugly head at a Runtime error!\nUnsupported types for operation '+' of "
                        + String(left->type)
                        + " and "
                        + String(right->type)
                        + " (line "
                        + std::to_string(t->getLineNumber())
                        + ")");
            }
        
            Lexeme* evalVar(Lexeme* t, Lexeme* e)
            {
                if (t->right != NULL)
                    return insert(t->left, eval(t->right, e), e);
                else
                    return insert(t->left, new Lexeme(NIL), e);
            }
        
            Lexeme* getArgs(Lexeme* t)
            {
                return cdr(t);
            }
    
            Lexeme* getName(Lexeme* t)
            {
                return car(t);
            }
    
            Lexeme* getParams(Lexeme* f)
            {
                return car(f);
            }
    
            Lexeme* getDefScope(Lexeme* f)
            {
                return car(cdr(cdr(f)));
            }
    
            Lexeme* getBody(Lexeme* f)
            {
                return car(cdr(f));
            }
        
            Lexeme* evalCall(Lexeme* t, Lexeme* e)
            {
                
                //printTree(t);
                //printf("\n\n");
                Lexeme* eargs = eval(getArgs(t), e); // this will call evalArgs
                Lexeme* f /* function closure */ = eval(getName(t), e); 
                // We need to use eval instead of lookup for this
                // because a function call can take two forms
                // Check to see if the closure is a builtin function. if so,
                // call the built in function evaluator
                if (f->type == BUILTIN)
                    return evalBuiltIn(f, eargs, e);
                
                if (listLength(eargs) != listLength(getParams(f))) 
                    throw std::runtime_error(
                    "The Kraken rears his ugly head at a Runtime error!\nThe function '"
                    + getName(t)->toValue()
                    + "' expects "
                    + std::to_string(listLength(getParams(f)))
                    + " argument(s). Found "
                    + std::to_string(listLength(eargs))
                    + " argument(s) (line "
                    + std::to_string(getName(t)->getLineNumber())
                    + ")");
    
                Lexeme* s /* new scope */ = extendEnv(getParams(f), eargs, getDefScope(f));
    
                return eval(getBody(f), s);
            }
        
            Lexeme* evalBuiltIn(Lexeme* bi, Lexeme* args, Lexeme* env)
            {
                Lexeme* result = (bi->fval)(args, env);
                result->lineNumber = bi->getLineNumber();
                return result;
            }
    
            Lexeme* evalDef(Lexeme* t, Lexeme* e)
            {
                return cons(CLOSURE, car(t), cons(GLUE, cdr(t), cons(GLUE, e, NULL)));
            }
    
            Lexeme* evalClassDef(Lexeme* t, Lexeme* e)
            {
                // for a class, make a normal function closure
                // so they can be instantiated as functions
                // just append a command to the end of the 
                // statement list to make them always return scopes instead
                // of other values
                Lexeme* statementList = cdr(t);
                if (statementList == NULL)
                {
                    t->right = cons(STATEMENTLIST, new Lexeme(THIS, t->getLineNumber()), NULL);
                }
                else if (statementList->type == STATEMENTLIST)
                {
                    while (cdr(statementList) != NULL)
                    {
                        statementList = cdr(statementList);
                    }
                    statementList->right = cons(STATEMENTLIST, new Lexeme(THIS, t->getLineNumber()), NULL);
                }
                
                else
                    throw std::runtime_error(
                    "The Kraken rears his ugly head at a Runtime error!\nIll-formed CLASS definition (line "
                    + std::to_string(t->getLineNumber())
                    + ")");
        
                // create the normal closure with the modified parse tree
                return cons(CLOSURE, car(t), cons(GLUE, cdr(t), cons(GLUE, e, NULL)));
            }
        
            Lexeme* assign(Lexeme* t, Lexeme* e) // t is OPERATOR tree
            {
                Lexeme* id = t->left;
                if (id->type == DOT)
                    return updateEnv(id->right, eval(t->right, e), eval(id->left, e));
                if (id->type == ARRAYREF)
                {
                    Lexeme* array = eval(id->left, e);
                    Lexeme* index = eval(id->right, e);
                
                    // We want array to be of type ARRAY and index to be an INTEGER
                    if (array->type == ARRAY and index->type == INTEGER)
                    {
                        if (index->ivalue < (int) array->vectorPointer.capacity() &&
                           index->ivalue >= 0)
                        {
                            Lexeme* result = eval(t->right, e);
                            array->vectorPointer[index->ivalue] = result;
                            return result;
                        }
                        else
                            throw std::runtime_error(
                            "The Kraken rears his ugly head at a Runtime error!\nOut of bounds ARRAY reference (line "
                            + std::to_string(index->getLineNumber())
                            + ")");
                    }
                    else
                        throw std::runtime_error(
                        "The Kraken rears his ugly head at a Runtime error!\nInvalid ARRAY reference. Expected types ARRAY and INTEGER. Found types "
                        + String(array->type)
                        + " and "
                        + String(index->type)
                        + "(line "
                        + std::to_string(array->getLineNumber())
                        + ")");
                }
                
                return updateEnv(id, eval(t->right, e), e);
            }
        
        public:
            // Define functions in the evaluator
            
        
            Evaluator(String dir)
            { 
                this->globalEnv = createEnv();
                loadBuiltIns();
                loadTypeHeirarchy();
                this->fileDir = dir;
                
            }
        
            Lexeme* startEval(String fileName)
            {
                Parser p = Parser();
                
                return this->eval(p.parse(fileName), this->globalEnv);
            }
            Lexeme* global()
            {
                return this->globalEnv;
            }
    };
}
#endif