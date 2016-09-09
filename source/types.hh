// types module
// Andrew Thomas

#include <map>
//#include "extern.h"

#ifndef TYPES_H
#define TYPES_H

namespace Leviathan
{   
    
    
    static std::map<std::string, const char*> Keywords;

    void initKeywords()
    {
        // Define the keywords of Leviathan
        Keywords["class"] = CLASS;
        Keywords["C"] = CLASS;
        Keywords["for"] = FOR;
        Keywords["L"] = FOR; //L for loop
		Keywords["function"] = FUNCTION;
		Keywords["F"] = FUNCTION;
		Keywords["if"] = IF;
		Keywords["I"] = IF;
		Keywords["in"] = IN;
		Keywords["else"] = ELSE;
		Keywords["E"] = ELSE;
		Keywords["while"] = WHILE;
		Keywords["W"] = WHILE;
		Keywords["var"] = VAR;
		Keywords["V"] = VAR;
		Keywords["and"] = AND;
		Keywords["A"] = AND;
		Keywords["or"] = OR;
		Keywords["O"] = OR;
		Keywords["none"] = NIL;
		Keywords["N"] = NIL;
		Keywords["this"] = THIS;
		Keywords["include"] = INCLUDE;
		Keywords["U"] = INCLUDE;
    };
}

#endif