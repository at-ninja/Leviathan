// environment module
// Andrew Thomas

//#include <string>
//#include "lexeme.h"
//#include "listOperations.h"

#ifndef ENV_H
#define ENV_H
typedef std::string String;

/*
        [ENVIRONMENT]
        /           \
    [ID LIST]       GLUE
                    /   \
            [VALS LIST] GLUE
                        /   \
                    CONTEXT  NULL
*/

/*
        The lists:
            [GLUE]
            /   \
            ID  list iter

*/

/*
        [CLOSURE]
        /       \
     paramlist  GLUE
                /   \
            body    GLUE
                    /   \
                def env  NULL
*/

namespace Leviathan
{
   
    Lexeme* insert(Lexeme* id, Lexeme* val, Lexeme* env)
    {
        Lexeme* ids = car(env);
        Lexeme* vals = car(cdr(env));
        env->left = cons(GLUE, id, ids);
        env->right->left = cons(GLUE, val, vals);
        return val;
    }

    Lexeme* extendEnv(Lexeme* vars, Lexeme* vals, Lexeme* env)
    {
        // Glue everything together
        return cons(ENVIRONMENT, vars, cons(GLUE, vals, cons(GLUE, env, NULL)));
    }

    Lexeme* createEnv()
    {
        return extendEnv(NULL, NULL, NULL);
    }

    Lexeme* lookup(Lexeme* id, Lexeme* env)
    {
        Lexeme* ids = car(env);
        Lexeme* vals = car(cdr(env));
        Lexeme* outer = car(cdr(cdr(env)));
        
        while (env != NULL)
        {
            ids = car(env);
            vals = car(cdr(env));
            outer = car(cdr(cdr(env)));
            while (ids != NULL)
            {
                if (id->svalue == car(ids)->svalue)
                {
                    return car(vals);
                }
                ids = cdr(ids);
                vals = cdr(vals);
            }
            env = outer;
        }
        
        String error = "The Kraken rears his ugly head at a Syntax error!\nThe identifier '"
            + id->toValue()
            + "' is undefined (line "
            + std::to_string(id->getLineNumber())
            + ")";
        
        throw std::runtime_error(error);
    }
    
    Lexeme* updateEnv(Lexeme* id, Lexeme* val, Lexeme* env)
    {
        Lexeme* ids = car(env);
        Lexeme* vals = car(cdr(env));
        Lexeme* outer = car(cdr(cdr(env)));
        
        //Lexeme* lastVal = NULL;
        
        while (env != NULL)
        {
            ids = car(env);
            vals = car(cdr(env));
            outer = car(cdr(cdr(env)));
            while (ids != NULL)
            {
                if (id->svalue == car(ids)->svalue)
                {
                    
                    vals->left = val;
                    
                    return val;
                }
                //lastVal = car(vals);
                ids = cdr(ids);
                vals = cdr(vals);
            }
            env = outer;
            //lastVal = NULL;
        }
        
        String error = "The Kraken rears his ugly head at a Syntax error!\nThe identifier '"
            + id->toValue()
            + "' is undefined (line "
            + std::to_string(id->getLineNumber())
            + ")";
        
        throw std::runtime_error(error);
    }
    
    void displayLocalEnv(Lexeme* env)
    {
        Lexeme* ids = car(env);
        Lexeme* vals = car(cdr(env));
        printf("The local environment is: \n");
        String val;
        while (ids != NULL)
        {
            printf("%s:%s\n", car(ids)->toValue().c_str(), car(vals)->toValue().c_str());
            ids = cdr(ids);
            vals = cdr(vals);
        }
        
        return;
    }
    
    void displayEnv(Lexeme* env)
    {
        Lexeme* ids = car(env);
        Lexeme* vals = car(cdr(env));
        Lexeme* outer = car(cdr(cdr(env)));
        printf("The environment is: \n");
        String val;
        while (env != NULL)
        {
            ids = car(env);
            vals = car(cdr(env));
            outer = car(cdr(cdr(env)));
            while (ids != NULL)
            {
                printf("%s:%s\n", car(ids)->toValue().c_str(), car(vals)->toValue().c_str());
                ids = cdr(ids);
                vals = cdr(vals);
            }
            env = outer;
        }
        
        return;
    }

}
#endif