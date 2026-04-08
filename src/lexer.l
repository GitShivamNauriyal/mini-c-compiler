%{
#include <iostream>
#include <string>
#include <cstdlib>

#include "parser.tab.h"

int line_num = 1;

/**
 * @file lexer.l
 * @brief Lexical analyzer for the Mini-C compiler.
 * 
 * Defines tokens for keywords, operators, identifiers, and literals using Flex.
 */
%}

/* Regular Definitions */
DIGIT       [0-9]
LETTER      [a-zA-Z]
ID          {LETTER}({LETTER}|{DIGIT}|_)*
INT_CONST   {DIGIT}+
FLOAT_CONST {DIGIT}+"."{DIGIT}+

/* Tell Flex to only read one input file and skip unused functions*/
%option noyywrap
%option nounput
%option noinput

%%

"int"       { return INT; }
"float"     { return FLOAT; }
"char"      { return CHAR; }
"if"        { return IF; }
"else"      { return ELSE; }
"while"     { return WHILE; }
"for"       { return FOR; }

"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MULTIPLY; }
"/"         { return DIVIDE; }
"="         { return ASSIGN; }
"=="        { return EQ; }
"!="        { return NEQ; }
"<"         { return LESS; }
">"         { return GREATER; }
"<="        { return LEQ; }
">="        { return GEQ; }
"("         { return LPAREN; }
")"         { return RPAREN; }
"{"         { return LBRACE; }
"}"         { return RBRACE; }
";"         { return SEMICOLON; }

{INT_CONST}   { 
                yylval.ival = std::atoi(yytext);
                return INT_LITERAL; 
              }

{FLOAT_CONST} { 
                yylval.fval = std::atof(yytext);
                return FLOAT_LITERAL; 
              }

{ID}          { 
                yylval.sval = strdup(yytext);
                return IDENTIFIER; 
              }

[ \t\r]+      { }
\n            { line_num++; }

"//".*        { }

"/*"([^*]|\*+[^*/])*\*+"/" {  }

.             { std::cerr << "Lexical Error at line " << line_num << ": Unrecognized character '" << yytext << "'\n"; }

%%
