// parser module
// Andrew Thomas

#include <string>
#include "extern.hh"
#include "types.hh"
#include "lexeme.hh"
#include "lexer.hh"
#include "listOperations.hh"
#include "parser.hh"
#include "environment.hh"
#include "evaluation.hh"

int main(int argc, char *argv[])
{
    try
    {
        
        if (argc < 2)
        {
            String error = "Usage: "; error.append(argv[0]); error += " <f>";
            throw std::runtime_error(error);
        }
        
        int found = std::string(argv[1]).find_last_of("/\\");
        
        std::string dir;
        if (found >= (int) std::string(argv[1]).length())
            dir = "./";
        else
            dir = std::string(argv[1]).substr(0, found + 1);
        
        
        Leviathan::Evaluator eval = Leviathan::Evaluator(dir);
        eval.startEval(argv[1]);
        
        return 0;
    }
    catch (std::runtime_error error)
    {
        printf("%s\n", error.what());
    }
    
}