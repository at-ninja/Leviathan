// types module
// Andrew Thomas


#ifndef EXTERN_H
#define EXTERN_H

namespace Leviathan
{
    extern const char* AND;
    extern const char* ASSIGN;
    extern const char* BOOLEAN;
    extern const char* CBRACE;
    extern const char* CBRACKET;
    extern const char* CLASS;
    extern const char* COMMA;
    extern const char* CPAREN;
    extern const char* DIVIDE;
    extern const char* DOT;
    extern const char* ELSE;
    extern const char* END_OF_INPUT;
    extern const char* EQUALITY;
    extern const char* FOR;
    extern const char* FUNCTION;
    extern const char* GTHAN;
    extern const char* IDENTIFIER;
    extern const char* IF;
    extern const char* IN;
    extern const char* INTEGER;
    extern const char* LTHAN;
    extern const char* MINUS;
    extern const char* MOD;
    extern const char* NOT;
    extern const char* OBRACE;
    extern const char* OBRACKET;
    extern const char* OPAREN;
    extern const char* OR;
    extern const char* PLUS;
    extern const char* REAL;
    extern const char* SEMI;
    extern const char* STRING;
    extern const char* TIMES;
    extern const char* VAR;
    extern const char* WHILE;
    extern const char* INCLUDE;
    
    // Extra parse tree lexeme types
    //extern const char* PROGRAM;
    extern const char* PARAMLIST;
    extern const char* FUNCCALL;
    extern const char* ARGLIST;
    //extern const char* BLOCK;
    extern const char* EXPRESSION;
    //extern const char* STATEMENT;
    extern const char* STATEMENTLIST;
    extern const char* ARRAY;
    extern const char* ARRAYDEF;
    extern const char* ARRAYREF;
    extern const char* GLUE;
    extern const char* NEGATIVE;
    
    // lexemes for envs
    extern const char* CLOSURE;
    extern const char* ENVIRONMENT;
    extern const char* THIS;
    extern const char* NIL;
    extern const char* BUILTIN;
} 

#endif