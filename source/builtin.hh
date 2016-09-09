// evaluation module
// Andrew Thomas

//#include <regex>
#include <iostream>
#include <chrono>
#include <random>

#ifndef BUILTINH
#define BUILTINH

namespace Leviathan
{
    
    Lexeme* evalPrint(Lexeme* args, Lexeme* env)
    {
        Lexeme* eargs = args; // The args are already evaluated
        // This is a variadic function -- no need for checking length of args
        while (eargs != NULL)
        {
            printf("%s", car(eargs)->toValue().c_str());
            eargs = cdr(eargs);
        }
        return new Lexeme(NIL); // returns none
    }
    
    Lexeme* evalPrintln(Lexeme* args, Lexeme* env)
    {
        // This is a variadic function -- no need for checking length of args
        evalPrint(args, env);
        printf("\n");
        return new Lexeme(NIL); // returns none
    }
    
    Lexeme* evalRange(Lexeme* eargs, Lexeme* env)
    {
        if (!(listLength(eargs) == 2 || listLength(eargs) == 3))
            throw std::runtime_error(
                "The Kraken rears his ugly head at a Runtime error!\nThe function 'range' expects 2 to 3 arguments. Found "
                + std::to_string(listLength(eargs))
                + " argument(s) (line "
                + std::to_string(car(eargs)->getLineNumber())
                + ")");
        
        Lexeme* startRange = car(eargs);
        Lexeme* endRange = car(cdr(eargs));
        Lexeme* step = NULL;
        if (cdr(cdr(eargs)) != NULL)
            step = car(cdr(cdr(eargs)));
        
        // We need all three to be integers. no exceptions
        if (startRange->type != INTEGER)
            throw std::runtime_error(
                "The Kraken rears his ugly head at a Runtime error!\nFunction 'range' expects parameter 'startRange' to be of type INTEGER. Found "
                + String(startRange->type)
                + " instead (line "
                + std::to_string(startRange->getLineNumber())
                + ")");
        if (endRange->type != INTEGER)
            throw std::runtime_error(
                "The Kraken rears his ugly head at a Runtime error!\nFunction 'range' expects parameter 'endRange' to be of type INTEGER. Found "
                + String(endRange->type)
                + " instead (line "
                + std::to_string(endRange->getLineNumber())
                + ")");
        if (step != NULL && step->type != INTEGER)
            throw std::runtime_error(
                "The Kraken rears his ugly head at a Runtime error!\nFunction 'range' expects parameter 'step' to be of type INTEGER. Found "
                + String(step->type)
                + " instead (line "
                + std::to_string(step->getLineNumber())
                + ")");
        if (step != NULL && step->ivalue == 0)
            throw std::runtime_error(
                "The Kraken rears his ugly head at a Runtime error!\nFunction 'range' cannot have a 'step' of 0 (line "
                + std::to_string(step->getLineNumber())
                + ")");
        
        int start = startRange->ivalue;
        int end = endRange->ivalue;
        int stepInt = step == NULL ? 1 : step->ivalue;
        int size = (int) ((end - start) / stepInt) + ((end - start) % stepInt > 0 ? 1 : 0);
        
        std::vector<Lexeme*> vect(size);
        int j = 0;
        for (int i = start; ((stepInt > 0) ? (i < end) : (i > end)); i = i + stepInt)
        {
            vect[j] = new Lexeme(INTEGER, startRange->getLineNumber(), i);
            j++;
        }
        
        return new Lexeme(ARRAY, startRange->getLineNumber(), vect);
    }
    
    bool is_number(String s)
    {
        for (int i = 0; i < (int) s.size(); i++)
        {
            if (!(isdigit(s[i]) || (i == 0 && s[i] == '-')))
                return false;
        }
        return true;
        //printf("'%s'\n", s.c_str());
        //std::regex r ("*");//("^-?\\d+$");
        //printf("'%s'\n", s.c_str());
        //return std::regex_match (s,r);
    }
    
    Lexeme* evalIntegerFunc(Lexeme* eargs, Lexeme* env)
    {
        if (!(listLength(eargs) == 1))
            throw std::runtime_error(
                "The Kraken rears his ugly head at a Runtime error!\nThe function 'int' expects 1 argument. Found "
                + std::to_string(listLength(eargs))
                + " argument(s) (line "
                + std::to_string(car(eargs)->getLineNumber())
                + ")");
        Lexeme* toInt = car(eargs);
        
        if (toInt->type == BOOLEAN)
        {   
            toInt->type = INTEGER;
            return toInt;
        }
        if (toInt->type == INTEGER)
            return toInt;
        if (toInt->type == REAL)
            return new Lexeme(INTEGER, toInt->getLineNumber(), (int) toInt->rvalue);
        if (toInt->type == STRING)
        {
            if (is_number(toInt->svalue))
                return new Lexeme(INTEGER, toInt->getLineNumber(), std::stoi(toInt->svalue));
            if (toInt->svalue.size() != 1)
                throw std::runtime_error(
                    "The Kraken rears his ugly head at a Runtime error!\nFunction 'int' expects the STRING to have length of 1. Found length of "
                    + std::to_string(toInt->svalue.size())
                    + " instead (line "
                    + std::to_string(toInt->getLineNumber())
                    + ")");
            return new Lexeme(INTEGER, toInt->getLineNumber(), (int) toInt->svalue.at(0));
        }
        
        throw std::runtime_error(
            "The Kraken rears his ugly head at a Runtime error!\nFunction 'int' expects parameter 'toInt' to be of type BOOLEAN, INTEGER, REAL, or STRING. Found "
                + String(toInt->type)
                + " instead (line "
                + std::to_string(toInt->getLineNumber())
                + ")");
    }
    
    bool is_number_real(String s)
    {
        bool hasDecimal = false;
        for (int i = 0; i < (int) s.size(); i++)
        {
            if (!(isdigit(s[i]) || (i == 0 && s[i] == '-') || (s[i] == '.' && !hasDecimal)))
                return false;
            if (s[i] == '.')
                hasDecimal = true;
        }
        return true;
    }
    
    Lexeme* evalRealFunc(Lexeme* eargs, Lexeme* env)
    {
        if (!(listLength(eargs) == 1))
            throw std::runtime_error(
                "The Kraken rears his ugly head at a Runtime error!\nThe function 'real' expects 1 argument. Found "
                + std::to_string(listLength(eargs))
                + " argument(s) (line "
                + std::to_string(car(eargs)->getLineNumber())
                + ")");
        Lexeme* toReal = car(eargs);
        
        if (toReal->type == BOOLEAN)
        {   
            return new Lexeme(REAL, toReal->getLineNumber(), (double) toReal->ivalue);
        }
        if (toReal->type == INTEGER)
            return new Lexeme(REAL, toReal->getLineNumber(), (double) toReal->ivalue);
        if (toReal->type == REAL)
            return toReal;
        if (toReal->type == STRING && is_number_real(toReal->svalue))
        {
            return new Lexeme(REAL, toReal->getLineNumber(), std::stod(toReal->svalue));
        }
        
        throw std::runtime_error(
                "The Kraken rears his ugly head at a Runtime error!\nFunction 'real' expects parameter 'toReal' to be of type BOOLEAN, INTEGER, REAL, or STRING. Found "
                + String(toReal->type)
                + " instead (line "
                + std::to_string(toReal->getLineNumber())
                + ")");
    }
    
    Lexeme* evalStringFunc(Lexeme* eargs, Lexeme* env)
    {
        if (!(listLength(eargs) == 1))
            throw std::runtime_error(
                "The Kraken rears his ugly head at a Runtime error!\nThe function 'string' expects 1 argument. Found "
                + std::to_string(listLength(eargs))
                + " argument(s) (line "
                + std::to_string(car(eargs)->getLineNumber())
                + ")");
        Lexeme* toString = car(eargs);
        
        return new Lexeme(STRING, toString->getLineNumber(), toString->toValue());
    }
    
    Lexeme* evalCharAt(Lexeme* eargs, Lexeme* env)
    {
        if (!(listLength(eargs) == 2))
            throw std::runtime_error(
                "The Kraken rears his ugly head at a Runtime error!\nThe function 'charAt' expects 2 arguments. Found "
                + std::to_string(listLength(eargs))
                + " argument(s) (line "
                + std::to_string(car(eargs)->getLineNumber())
                + ")");
        
        Lexeme* toChar = car(eargs);
        Lexeme* num = car(cdr(eargs));
        
        if (!(toChar->type == STRING))
            throw std::runtime_error(
                "The Kraken rears his ugly head at a Runtime error!\nFunction 'charAt' expects parameter 'str' to be of type STRING. Found "
                + String(toChar->type)
                + " instead (line "
                + std::to_string(toChar->getLineNumber())
                + ")");
        if (!(num->type == INTEGER))
            throw std::runtime_error(
                "The Kraken rears his ugly head at a Runtime error!\nFunction 'charAt' expects parameter 'index' to be of type INTEGER. Found "
                + String(num->type)
                + " instead (line "
                + std::to_string(num->getLineNumber())
                + ")");
        
        return new Lexeme(STRING, toChar->getLineNumber(), toChar->svalue.substr(num->ivalue, 1));
    }
    
    Lexeme* evalLen(Lexeme* eargs, Lexeme* env)
    {
        if (!(listLength(eargs) == 1))
            throw std::runtime_error(
                "The Kraken rears his ugly head at a Runtime error!\nThe function 'len' expects 1 argument. Found "
                + std::to_string(listLength(eargs))
                + " argument(s) (line "
                + std::to_string(car(eargs)->getLineNumber())
                + ")");
        Lexeme* getLen = car(eargs);
        
        if (!(getLen->type == STRING || getLen->type == ARRAY))
            throw std::runtime_error(
                "The Kraken rears his ugly head at a Runtime error!\nFunction 'len' expects parameter 'obj' to be of type STRING or ARRAY. Found "
                + String(getLen->type)
                + " instead (line "
                + std::to_string(getLen->getLineNumber())
                + ")");
        
        if (getLen->type == STRING)
            return new Lexeme(INTEGER, getLen->getLineNumber(), (int) getLen->svalue.size());
        return new Lexeme(INTEGER, getLen->getLineNumber(), (int) getLen->vectorPointer.size());
    }
    
    Lexeme* evalInput(Lexeme* eargs, Lexeme* env)
    {
        if (!(listLength(eargs) == 0))
            throw std::runtime_error(
                "The Kraken rears his ugly head at a Runtime error!\nThe function 'input' expects 0 arguments. Found "
                + std::to_string(listLength(eargs))
                + " argument(s) (line "
                + std::to_string(car(eargs)->getLineNumber())
                + ")");
        String s;
        std::getline(std::cin, s);
        return new Lexeme(STRING, 0, s);
    }
    
    Lexeme* evalRand(Lexeme* eargs, Lexeme* env)
    {
        if (!(listLength(eargs) == 0))
            throw std::runtime_error(
                "The Kraken rears his ugly head at a Runtime error!\nThe function 'rand' expects 0 arguments. Found "
                + std::to_string(listLength(eargs))
                + " argument(s) (line "
                + std::to_string(car(eargs)->getLineNumber())
                + ")");
        
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

        std::minstd_rand0 generator (seed);  // minstd_rand0 is a standard linear_congruential_engine
        return new Lexeme(INTEGER, 0, (int) generator());
    }
}

#endif