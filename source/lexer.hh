// lexer module
// Andrew Thomas

#include <fstream>
//#include "types.h"
//#include "lexeme.h"

#ifndef LEXER_H
#define LEXER_H

// Create typedef for String to avoid having to call std::string
typedef std::string String;



namespace Leviathan
{
    class Lexer
    {
        private:
            int lineNumber;
            char ch;
            std::ifstream* file;
            int init(String fileName)
            {
                try 
                {
                    file = new std::ifstream(fileName.c_str());
                    lineNumber = 1;
            
                    initKeywords();
            
                }
                catch (int e)
                {
                    return 1; // 1 indicates file failure
                }
                return 0; // 0 indicates no problems
            }
        
            char getCharacter()
            {
                file->get(ch);
                return ch;
            };
    
            char peek()
            {
                return file->peek();
            };
    
            void putback()
            {
                file->putback(ch);
            };
    
            int skipWhiteSpace()
            {
                // Act as though whitespace is a lexeme. parse as such
                bool blockComment = ch == '#' and peek() == '<';
                if (blockComment) getCharacter(); //advance a char so ### isn't a full block comment
                bool inComment = ch == '#' && !blockComment;
                while ((ch == ' ' || ch == '\n' || ch == '\t' || inComment || blockComment) && !endOfInput())
                {
                    if (ch == '\n')
                        lineNumber++;
                        
                    getCharacter();
                    
                    if (ch == '#')
                    {
                        if (peek() == '>' && !inComment) 
                        {
                            getCharacter(); // ch == '>'
                            getCharacter(); // ch == next space/start of token
                            blockComment = false;
                        }
                        else 
                        {
                            if (peek() == '<' && !inComment)
                            {
                                blockComment = true; // dont need to worry about next char
                            }
                            else 
                                if (!inComment && !blockComment) inComment = true;
                        }
                    }
                                
                    if (ch == '\n')
                        inComment = false;
                        
                }
        
                return lineNumber;
            };
    
            bool endOfInput()
            {
                return ch == EOF || file->eof();
            };
    
            Lexeme lexString()
            {
                String buffer = "";
                getCharacter();
                while (ch != '\"')
                {
                    if (ch == '\\') 
                    {
                        getCharacter();
                        if (ch == 'n') ch = '\n';
                    }
                    buffer += ch;
                    getCharacter();
                    if (endOfInput())
                    {
                        String error = "Character '\"' not found to close string.";
                        throw std::runtime_error(error);
                    }
                }
                return Lexeme(STRING, getLineNumber(), buffer);
            };

            Lexeme lexNumber(char initialch)
            {
                String buffer = "";
                bool isReal = ch == '.';
                buffer += initialch;
                getCharacter();
                while ((isdigit(ch) || ch == '.') && !endOfInput())
                {
                
                    if (ch == '.')
                    {
                        if (!isReal) isReal = true;
                        else 
                        {
                            String error = "Bad Character on line " + std::to_string(lineNumber) + ": ";
                            error += ch;
                            throw std::runtime_error(error);
                        }
                    }
                    buffer += ch;
                    getCharacter();
                }
                if (!endOfInput())
                putback();
    
                return isReal ? Lexeme(REAL, getLineNumber(), stod(buffer)) : Lexeme(INTEGER, getLineNumber(), stoi(buffer));
            };

            Lexeme lexWord(char initialch)
            {
                String buffer = "";
                buffer += initialch;
                getCharacter();
                while ((isalpha(ch) || isdigit(ch) || ch == '_') && !endOfInput())
                {
                    buffer += ch;
                    getCharacter();
                }
                if (!endOfInput())
                putback();
    
                // keywords
                if (Keywords.find(buffer) != Keywords.end()) return Lexeme(Keywords.find(buffer)->second, getLineNumber());
                if (buffer == "T" || buffer == "true") return Lexeme(BOOLEAN, getLineNumber(), 1); //true and false are different from most. they act as ints, so they need to have a special case
                if (buffer == "false") return Lexeme(BOOLEAN, getLineNumber(), 0);
    
                return Lexeme(IDENTIFIER, getLineNumber(), buffer);
            };
        
        public:
            Lexer ()
            {
                lineNumber = 0;
                ch = '\0';
                file = NULL;
            }
            Lexer (String filename)
            {
                int a = init(filename);
                if (a == 1)
                {
                    String error = "There was an error opening file \"";
                    error += filename;
                    throw std::runtime_error(error);
                }
            };
            void close()
            {
                file->close();
                delete file;
            };
            int getLineNumber()
            {
                return lineNumber;
            };
            Lexeme lex()
            {
                    getCharacter();
                    
                    skipWhiteSpace();  // keeps track of comments and line numbers
                    
                    if (endOfInput()) return Lexeme(END_OF_INPUT, getLineNumber());
        
            
                    // Single character letters
                    if (ch == '.' && !isdigit(peek())) return Lexeme(DOT, getLineNumber());
                    if (ch == ',') return Lexeme(COMMA, getLineNumber());
                    if (ch == ')') return Lexeme(CPAREN, getLineNumber());
                    if (ch == '(') return Lexeme(OPAREN, getLineNumber());
                    if (ch == '[') return Lexeme(OBRACKET, getLineNumber());
                    if (ch == ']') return Lexeme(CBRACKET, getLineNumber());
                    if (ch == '{') return Lexeme(OBRACE, getLineNumber());
                    if (ch == '}') return Lexeme(CBRACE, getLineNumber());
                    if (ch == '!') return Lexeme(NOT, getLineNumber());
                    if (ch == '<') return Lexeme(LTHAN, getLineNumber());
                    if (ch == '>') return Lexeme(GTHAN, getLineNumber());
                    if (ch == '/') return Lexeme(DIVIDE, getLineNumber());
                    if (ch == '+') return Lexeme(PLUS, getLineNumber());
                    if (ch == '-') return Lexeme(MINUS, getLineNumber());
                    if (ch == '*') return Lexeme(TIMES, getLineNumber());
                    if (ch == '%') return Lexeme(MOD, getLineNumber());
                    if (ch == ';') return Lexeme(SEMI, getLineNumber());
            
                    // Characters with multiple cases
                    if (ch == '=') 
                    {
                        if (peek() == '=')
                        {
                            getCharacter();
                            return Lexeme(EQUALITY, getLineNumber());
                        }
                        return Lexeme(ASSIGN, getLineNumber());
                    }
            
                    if (ch == '\"') return lexString();
                    if (isdigit(ch) || (ch == '.' && isdigit(peek()))) return lexNumber(ch);
                    if (isalpha(ch) || (ch == '_')) return lexWord(ch);
        
                    String error = "Bad Character on line " + std::to_string(lineNumber) + ": ";
                    error += ch;
                    throw std::runtime_error(error);
            
            };
    };
}

#endif




