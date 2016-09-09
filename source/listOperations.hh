// parser module
// Andrew Thomas

//#include "lexeme.h"

#ifndef LISTUTILS_H
#define LISTUTILS_H

namespace Leviathan
{
    Lexeme* cons(const char* type, Lexeme* left, Lexeme* right)
    {
        return new Lexeme(type, left, right);
    }
    
    Lexeme* car(Lexeme* l)
    {
        return l->left; // The car of the lexeme can be seen as its left pointer
    }
    
    Lexeme* cdr(Lexeme* l)
    {
        return l->right; // The cdr of the lexeme can be seen as its right pointer
    }
    
    int listLength(Lexeme* l)
    {
        if (l == NULL) return 0;
        else return 1 + listLength(cdr(l));
    }
    
    // This is here to help me visualize the parse trees
    void printTreeHelper(Lexeme* r, int level)
    {
        if (r == NULL) return;
        //printf("'%s'\n", r->toString().c_str());
        printTreeHelper(r->left, level+1);
        std::string s = std::to_string(level);
        for (int i = 0; i < level; i++) s += "    ";
        printf("%s%s\n", s.c_str(), r->toString().c_str());
        printTreeHelper(r->right, level+1);
        
    }
    void printTree(Lexeme* root)
    {
        printTreeHelper(root, 1);
    }
}

#endif