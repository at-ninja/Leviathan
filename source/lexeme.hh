// lexeme module
// Andrew Thomas

#include <vector>

#ifndef LEXEME_H
#define LEXEME_H

typedef std::string String;

namespace Leviathan
{
    class Lexeme
    {
        public:
            const char* type;
            int ivalue;
            int lineNumber;
            bool ivalPop;
            String svalue; 
            double rvalue;
            bool rvalPop;
            Lexeme* left;
            Lexeme* right;
            Lexeme* (*fval) (Lexeme*, Lexeme*);// Built in eval function pointer
            std::vector<Lexeme*> vectorPointer;
            Lexeme ()
            {
                this->type = NULL;
                this->ivalue = 0;
                this->ivalPop = false;
                this->svalue = "";
                this->rvalue = 0.;
                this->rvalPop = false;
                this->left = NULL;
                this->right = NULL;
                this->fval = NULL;
                this->lineNumber = 0;
                //this->vectorPointer = NULL;
            };
            Lexeme (Lexeme* copy)
            {
                this->type = copy->type;
                this->ivalue = copy->ivalue;
                this->ivalPop = copy->ivalPop;
                this->svalue = copy->svalue;
                this->rvalue = copy->rvalue;
                this->rvalPop = copy->rvalPop;
                this->left = copy->left;
                this->right = copy->right;
                this->fval = copy->fval;
                this->lineNumber = copy->lineNumber;
                this->vectorPointer = copy->vectorPointer;
            };
            Lexeme (const char* type)
            {
                this->type = type;
                this->ivalPop = false;
                this->rvalPop = false;
                this->left = NULL;
                this->right = NULL;
                this->fval = NULL;
                this->lineNumber = 0;
                //this->vectorPointer = NULL;
            };
            Lexeme (const char* type, int line)
            {
                this->type = type;
                this->ivalPop = false;
                this->rvalPop = false;
                this->left = NULL;
                this->right = NULL;
                this->fval = NULL;
                this->lineNumber = line;
            }
            Lexeme (const char* type, Lexeme* l, Lexeme* r)
            {
                this->type = type;
                this->ivalPop = false;
                this->rvalPop = false;
                this->left = l;
                this->right = r;
                this->fval = NULL;
                this->lineNumber = 0;
                //this->vectorPointer = NULL;
            };
            Lexeme (const char* type, int line, int ival)
            {
                this->type = type;
                this->ivalue = ival;
                this->ivalPop = true;
                this->rvalPop = false;
                this->left = NULL;
                this->right = NULL;
                this->fval = NULL;
                this->lineNumber = line;
                //this->vectorPointer = NULL;
            };
            Lexeme (const char* type, int line, String sval)
            {
                this->type = type;
                this->svalue = sval;
                this->ivalPop = false;
                this->rvalPop = false;
                this->left = NULL;
                this->right = NULL;
                this->fval = NULL;
                this->lineNumber = line;
                //this->vectorPointer = NULL;
            };
            Lexeme (const char* type, int line, double rval)
            {
                this->type = type;
                this->rvalue = rval;
                this->rvalPop = true;
                this->ivalPop = false;
                this->left = NULL;
                this->right = NULL;
                this->fval = NULL;
                this->lineNumber = line;
                //this->vectorPointer = NULL;
            };
            Lexeme (const char* type, int line, Lexeme* (*f)(Lexeme*, Lexeme*))
            {
                this->type = type;
                this->rvalPop = false;
                this->ivalPop = false;
                this->left = NULL;
                this->right = NULL;
                this->fval = f;
                this->lineNumber = line;
                //this->vectorPointer = NULL;
            };
            Lexeme (const char* type, int line, std::vector<Lexeme*> vect)
            {
                this->type = type;
                this->ivalue = 0;
                this->ivalPop = false;
                this->svalue = "";
                this->rvalue = 0.;
                this->rvalPop = false;
                this->left = NULL;
                this->right = NULL;
                this->fval = NULL;
                this->lineNumber = line;
                this->vectorPointer = vect;
            };
            String toString()
            {
                String result = "{ Type: ";
                result.append(type);
                if (!svalue.empty()) result += ", Value: " + svalue;
                if (ivalPop) result += ", Value: " + std::to_string(ivalue);
                if (rvalPop) result += ", Value: " + std::to_string(rvalue);
                return result + " }";
            };
            String toValue()
            {
                if      (type == STRING) return svalue;
                else if (type == INTEGER) return std::to_string(ivalue);
                else if (type == REAL) return std::to_string(rvalue);
                else if (type == BOOLEAN) return ivalue == 1 ? "true" : "false";
                
                // The below types should be impossible to print in the language
                // however, they are useful for environment testing
                // and for pretty-printing/printing in the language
                else if (type == IDENTIFIER) return svalue;

                else if (type == AND) return "and";
                else if (type == ARRAY) 
                {
                    String result = "[";
                    if (this->vectorPointer.size() > 0) {
                        for (int i = 0; i < (int) this->vectorPointer.size() - 1; i++)
                        {
                            result += this->vectorPointer[i]->toValue();
                            result += ", ";
                        }
                        result += this->vectorPointer[this->vectorPointer.size() - 1]->toValue();
                    }
                    result += "]";
                    return result;
                }
                else if (type == ASSIGN) return "=";
                else if (type == CBRACE) return "}";
                else if (type == CBRACKET) return "]";
                else if (type == CLASS) return "class";
                else if (type == COMMA) return ",";
                else if (type == CLOSURE) return "<Closure>";
                else if (type == CPAREN) return ")";
                else if (type == DIVIDE) return "/";
                else if (type == DOT) return ".";
                else if (type == ELSE) return "else";
                else if (type == END_OF_INPUT) return "EoF";
                else if (type == ENVIRONMENT) return "<Environment>";
                else if (type == EQUALITY) return "==";
                else if (type == FOR) return "for";
                else if (type == FUNCTION) return "function";
                else if (type == GTHAN) return ">";
                else if (type == IF) return "if";
                else if (type == IN) return "in";
                else if (type == LTHAN) return "<";
                else if (type == MINUS) return "-";
                else if (type == NEGATIVE) return "-";
                else if (type == NIL) return "none";
                else if (type == MOD) return "%";
                else if (type == NOT) return "!";
                else if (type == OBRACE) return "{";
                else if (type == OBRACKET) return "[";
                else if (type == OPAREN) return "(";
                else if (type == OR) return "or";
                else if (type == PLUS) return "+";
                else if (type == TIMES) return "*";
                else if (type == VAR) return "var";
                else if (type == WHILE) return "while";

                else return toString();
            };
            
            bool equals(Lexeme* lex)
            {
                if (lex == NULL) return false;
                if (this->type == lex->type &&
                   (this->ivalPop == lex->ivalPop ||
                   this->rvalPop == lex->rvalPop ||
                   this->svalue.empty() == lex->svalue.empty()))
                {
                    if (this->ivalPop == true && lex->ivalPop == true)
                        return this->ivalue == lex->ivalue;
                    if (this->rvalPop == true && lex->rvalPop == true)
                        return this->rvalue == lex->rvalue;
                    if (!(this->svalue.empty()) == true && !lex->svalue.empty())
                        return this->svalue.compare(lex->svalue);
                    return true;
                }
                else return false;
            }
        
            int getLineNumber()
            {
                return this->lineNumber;
            }
    };
}

#endif