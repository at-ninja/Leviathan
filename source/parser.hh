// recognizer module
// Andrew Thomas

//#include <string>
//#include "types.h"
//#include "lexeme.h"
//#include "lexer.h"
//#include "listOperations.h"

#ifndef PARSER_H
#define PARSER_H
typedef std::string String;

namespace Leviathan
{
    class Parser
    {
        private:
            Lexer i;
            Lexeme Pending;

            Lexeme* program()
            {
                Lexeme* a = optStatementList();
                return a;
            }
            
            Lexeme* statement()
            {
                if (varDefPending())
                    return varDef();
                else if (whileLoopPending())
                    return whileLoop();
                else if (forLoopPending())
                    return forLoop();
                else if (exprPending())
                    return expr();
                else if (ifStatementPending())
                    return ifStatement();
                else if (blockPending())
                    return block();
                else if (includeStatementPending())
                    return includeStatement();
                else
                {
                    String error = "The Kraken rears his ugly head at a Syntax error!\nLexeme "
                        + String(Pending.type) 
                        + " found while parsing 'Statement' (line " 
                        + std::to_string(i.getLineNumber())
                        + ")";
                    throw std::runtime_error(error);
                }
                return NULL;
            }
            
            bool statementPending()
            {
                return varDefPending() || whileLoopPending() 
                    || forLoopPending() || exprPending() || ifStatementPending() 
                    || blockPending() || includeStatementPending();
            }
        
            Lexeme* includeStatement()
            {
                match(INCLUDE);
                Lexeme* a = expr();
                return cons(INCLUDE, a, NULL);
            }
        
            bool includeStatementPending()
            {
                return check(INCLUDE);
            }
            
            Lexeme* whileLoop()
            {
                match(WHILE);
                Lexeme* a = expr();
                Lexeme* b = statement();
                return cons(WHILE, a, b);
            }
            
            bool whileLoopPending()
            {
                return check(WHILE);
            }
            
            Lexeme* ifStatement()
            {
                match(IF);
                Lexeme* a = expr();
                Lexeme* b = statement();
                Lexeme* c = optElse();
                return cons(IF, a, cons(GLUE, b, c));
            }
            
            bool ifStatementPending()
            {
                return check(IF);
            }
            
            Lexeme* optElse()
            {
                Lexeme* a = NULL;
                if (check(ELSE))
                {
                    advance();
                    a = statement();
                }
                
                return a;
            }
            
            Lexeme* varDef()
            {
                match(VAR);
                Lexeme* a = match(IDENTIFIER);
                Lexeme* b = optAssign();
                return cons(VAR, a, b);
            }
            
            bool varDefPending()
            {
                return check(VAR);
            }
            
            Lexeme* optAssign()
            {
                Lexeme* a = NULL;
                if (check(ASSIGN))
                {
                    advance();
                    a = expr();
                }
                
                return a;
            }
            
            Lexeme* block()
            {
                match(OBRACE);
                Lexeme* a = optStatementList();
                match(CBRACE);
                return a;
            }
            
            bool blockPending()
            {
                return check(OBRACE);
            }
            
            Lexeme* optStatementList()
            {
                Lexeme* a = NULL;
                if (statementListPending())
                    a = statementList();
                return a;
            }
            
            Lexeme* statementList()
            {
                Lexeme* a = statement();
                optSemi();
                Lexeme* b = NULL;
                if (statementListPending())
                    b = statementList();
                return cons(STATEMENTLIST, a, b);
            }
            
            bool statementListPending()
            {
                return statementPending();
            }
        
            Lexeme* optSemi()
            {
                if (check(SEMI))
                    return advance();
                return NULL;
            }
            
            Lexeme* expr()
            {
                return expr1();
            }
            
            bool exprPending()
            {
                return expr1Pending();
            }
        
            Lexeme* expr1()
            {
                Lexeme* tree = expr2();
                Lexeme* temp = NULL;
                if (op1Pending())
                {
                    temp = op1();
                    temp->left = tree;
                    temp->right = expr1();
                    tree = temp;
                }
                return tree;
            }
        
            bool expr1Pending()
            {
                return expr2Pending();
            }
        
            Lexeme* expr2()
            {
                Lexeme* tree = expr3();
                Lexeme* temp = NULL;
                if (op2Pending())
                {
                    temp = op2();
                    temp->left = tree;
                    temp->right = expr2();
                    tree = temp;
                }
                return tree;
            }
        
            bool expr2Pending()
            {
                return expr3Pending();
            }
        
            Lexeme* expr3()
            {
                Lexeme* tree = expr4();
                Lexeme* temp = NULL;
                if (op3Pending())
                {
                    temp = op3();
                    temp->left = tree;
                    temp->right = expr3();
                    tree = temp;
                }
                return tree;
            }
        
            bool expr3Pending()
            {
                return expr4Pending();
            }
        
            Lexeme* expr4()
            {
                Lexeme* tree = expr5();
                Lexeme* temp = NULL;
                if (op4Pending())
                {
                    temp = op4();
                    temp->left = tree;
                    temp->right = expr4();
                    tree = temp;
                }
                return tree;
            }
        
            bool expr4Pending()
            {
                return expr5Pending();
            }
        
            Lexeme* expr5()
            {
                Lexeme* tree = primary();
                Lexeme* temp = NULL;
                while (op5Pending())
                {
                    temp = op5();
                    temp->left = tree;
                    temp->right = primary();
                    tree = temp;
                }
                return tree;
            }
        
            bool expr5Pending()
            {
                return primaryPending();
            }
        
            Lexeme* op1()
            {
                if (op1Pending())
                    return advance();
                else
                {
                    String error = "The Kraken rears his ugly head at a Syntax error!\nLexeme "
                        + String(Pending.type)
                        + " found while parsing 'Operator1' (line " 
                        + std::to_string(i.getLineNumber())
                        + ")";
                    throw std::runtime_error(error);
                }
                return NULL;
            }
        
            bool op1Pending()
            {
                return check(ASSIGN);
            }
        
            Lexeme* op2()
            {
                if (op2Pending())
                    return advance();
                else
                {
                    String error = "The Kraken rears his ugly head at a Syntax error!\nLexeme "
                        + String(Pending.type)
                        + " found while parsing 'Operator2' (line " 
                        + std::to_string(i.getLineNumber())
                        + ")";
                    throw std::runtime_error(error);
                }
                return NULL;
            }
        
            bool op2Pending()
            {
                return check(MINUS) || check(PLUS);
            }
        
            Lexeme* op3()
            {
                if (op3Pending())
                    return advance();
                else
                {
                    String error = "The Kraken rears his ugly head at a Syntax error!\nLexeme "
                        + String(Pending.type)
                        + " found while parsing 'Operator3' (line " 
                        + std::to_string(i.getLineNumber())
                        + ")";
                    throw std::runtime_error(error);
                }
                return NULL;
            }
        
            bool op3Pending()
            {
                return check(DIVIDE) || check(MOD) || check(TIMES);
            }
            
            Lexeme* op4()
            {
                if (op4Pending())
                    return advance();
                else
                {
                    String error = "The Kraken rears his ugly head at a Syntax error!\nLexeme "
                        + String(Pending.type)
                        + " found while parsing 'Operator4' (line " 
                        + std::to_string(i.getLineNumber())
                        + ")";
                    throw std::runtime_error(error);
                }
                return NULL;
            }
        
            bool op4Pending()
            {
                return check(AND) || check(EQUALITY) || check(GTHAN) || check(LTHAN) || check(OR);
            }
        
            Lexeme* op5()
            {
                if (op5Pending())
                    return advance();
                else
                {
                    String error = "The Kraken rears his ugly head at a Syntax error!\nLexeme "
                        + String(Pending.type)
                        + " found while parsing 'Operator5' (line " 
                        + std::to_string(i.getLineNumber())
                        + ")";
                    throw std::runtime_error(error);
                }
                return NULL;
            }
        
            bool op5Pending()
            {
                return check(DOT);
            }
            
            Lexeme* primary()
            {
                Lexeme* a = NULL;
                if (check(STRING) || check(INTEGER) || check(REAL) || check(BOOLEAN) || check(THIS) || check(NIL))
                    a = advance();
                else if (check(OPAREN))
                {
                    advance();
                    a = expr();
                    match(CPAREN);
                }
                else if (defFuncPending())
                    a = defFunc();
                else if (defClassPending())
                    a = defClass();
                else if (check(MINUS))
                {
                    advance();
                    a = cons(NEGATIVE, primary(), NULL);
                }
                else if (check(NOT))
                {
                    advance();
                    match(OPAREN);
                    a = cons(NOT, expr(), NULL);
                    match(CPAREN);
                }
                else if (arrayPending())
                    a = array();
                else if (check(IDENTIFIER))
                {
                    a = advance();
                }
                else
                {
                    String error = "The Kraken rears his ugly head at a Syntax error!\nLexeme "
                        + String(Pending.type)
                        + " found while parsing 'Primary' (line " 
                        + std::to_string(i.getLineNumber())
                        + ")";
                    throw std::runtime_error(error);
                }
                
                if (check(OBRACKET))
                {
                    return arrayRef(a);
                }
                else if (check(OPAREN))
                {
                    return funcCall(a);
                }
                
                return a;
            }
            
            bool primaryPending()
            {
                return check(IDENTIFIER) || check(STRING) || check(INTEGER) 
                    || check(REAL) || check(BOOLEAN) || check(OPAREN) 
                    || defFuncPending() || check(MINUS) || check(NOT)
                    || defClassPending() || check(THIS) || check(NIL)
                    || arrayPending();
            }
            
            Lexeme* array()
            {
                match(OBRACKET);
                Lexeme* a = optArgList();
                match(CBRACKET);
                return cons(ARRAYDEF, a, NULL);
            }
            
            bool arrayPending()
            {
                return check(OBRACKET);
            }
            
            Lexeme* optArgList()
            {
                Lexeme* a = NULL;
                if (argListPending())
                    a = argList();
                return a;
            }
            
            Lexeme* argList()
            {
                Lexeme* a = expr();
                Lexeme* b = NULL;
                if (check(COMMA))
                {
                    advance();
                    b = argList();
                }
                return cons(ARGLIST, a, b);
            }
            
            bool argListPending()
            {
                return exprPending();
            }
            
            Lexeme* funcCall(Lexeme* prim)
            {
                match(OPAREN);
                Lexeme* a = optArgList();
                match(CPAREN);
                return cons(FUNCCALL, prim, a);
            }
        
            Lexeme* arrayRef(Lexeme* prim)
            {
                match(OBRACKET);
                Lexeme* a = expr();
                match(CBRACKET);
                return cons(ARRAYREF, prim, a);
            }
            
            Lexeme* defFunc()
            {
                match(FUNCTION);
                match(OPAREN);
                Lexeme* a = optParamList();
                match(CPAREN);
                Lexeme* b = statement();
                return cons(FUNCTION, a, b);
            }
            
            bool defFuncPending()
            {
                return check(FUNCTION);
            }
        
            Lexeme* defClass()
            {
                match(CLASS);
                match(OPAREN);
                Lexeme* a = optParamList();
                match(CPAREN);
                Lexeme* b = statement();
                return cons(CLASS, a, b);
            }
        
            bool defClassPending()
            {
                return check(CLASS);
            }
        
            Lexeme* optParamList()
            {
                Lexeme* a = NULL;
                if (paramListPending())
                    a = paramList();
                return a;
            }
            
            Lexeme* paramList()
            {
                Lexeme* a = match(IDENTIFIER);
                Lexeme* b = NULL;
                if (check(COMMA))
                {
                    advance();
                    b = paramList();
                }
                return cons(PARAMLIST, a, b);
            }
            
            bool paramListPending()
            {
                return check(IDENTIFIER);
            }
            
            Lexeme* forLoop()
            {
                match(FOR);
                Lexeme* a = match(IDENTIFIER);
                match(IN);
                Lexeme* b = expr();
                Lexeme* c = statement();
                return cons(FOR, a, cons(GLUE, b, c));
            }
            
            bool forLoopPending()
            {
                return check(FOR);
            }
            
            
            
            // END GRAMMAR RULES
                        
            bool check(const char* type) 
            { 
                return Pending.type == type; 
            }
          
            Lexeme* advance() // return the old Pending lexeme as a new Lexeme pointer
            { 
                Lexeme* ret = new Lexeme(&Pending);
                Pending = i.lex(); 
                return ret;
            } 
          
            Lexeme* match(const char* type)
            { 
                matchNoAdvance(type); 
                return advance(); 
            }
          
            void matchNoAdvance(const char* type)
            {
                if (!check(type))
                {
                    String error = "The Kraken rears his ugly head at a Syntax error!\nLexeme "
                        + String(Pending.type)
                        + " found while trying to match a " 
                        + String(type)
                        + " Lexeme (line " 
                        + std::to_string(i.getLineNumber())
                        + ")";
                    
                    throw std::runtime_error(error);
                }
            }
            int lexerLineNumber()
            {
                return i.getLineNumber();
            }
        public:
            Parser()
            {
                
            }
            Lexeme* parse(String fileName)
            {
                i = Lexer(fileName);
                Pending = i.lex();
                Lexeme* a = program();
                match(END_OF_INPUT);
                i.close();
                return a;
            }
            
    };
}

#endif
