#pragma once
#ifndef _GLOBALS_H_
#define _GLOBALS_H_


#include<string>
#include<vector>
#include<fstream>
#include<map>
#include<set>
#include<queue>
using std::string;
using std::vector;
using std::set;
using std::pair;
using std::queue;
using std::endl;

// \ & | * + ? [-] () 
typedef enum class TokenType
{
    //SPECIAL
    ERROR, ENDFILE, ENDLINE, EPSILLION,
    //OP
    EQ, OR, AND, CLOSURE, POSCLOSURE, OPTION, SQUARE, LPARENT, RPARENT,
    //EXP
    NAME, CHAR, STRING, NUM, UNDERLINE,
} TT;


extern std::ifstream in;
extern std::ofstream out;

extern int lineno;

/* Error = TRUE prevents further passes if an error occurs */
extern bool Error;

#endif
