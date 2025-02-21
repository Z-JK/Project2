#pragma once
#ifndef _SCANNER_H_
#define _SCANNER_H_



#include "GLOBALS.H"
using std::string;


/* tokenString array stores the lexeme of each token */
extern string tokenString;

/* function getToken returns the
 * next token in source file
 */
TokenType getToken(void);
bool loadLine(void);
void printLineBuf(void);
#endif
