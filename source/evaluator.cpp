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

void startInteractivePrompt();

int main(int argc, char *argv[])
{
    try
    {
        
        if (argc < 2)
        {
            // Call the interactive prompt

            startInteractivePrompt();
            return 0;

            // String error = "Usage: "; error.append(argv[0]); error += " <f>";
            // throw std::runtime_error(error);
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

void startInteractivePrompt() 
{
    std::string dir = "./";
    printf("%s\n%s\n","Kraken (Leviathan) v1.1", "Press Ctrl+C to exit");

    std::string line;
    std::string runningline;
    std::string emptyString = "";

    // Create an Evaluation environment
    Leviathan::Evaluator eval = Leviathan::Evaluator(dir);

    printf("%s", "lev> ");
    while (std::getline(std::cin, line)) 
    {
        if (line == emptyString)
        {
            // Try to run whatever we have
            
            try
            {
                Leviathan::Lexeme* result = eval.startEvalString(runningline);

                // Print the result
                if (result != NULL)
                    printf("%s\n", result->toValue().c_str());
                else printf("\n");
            }
            catch (std::runtime_error e)
            {
                printf("%s\n", e.what());
            }

            // Reset the runningline
            runningline = "";
        }
        else
        {
            // Add the line to the running total
            runningline += line; 
            runningline += "\n";
        }
        printf("%s", "lev> ");
    }
}