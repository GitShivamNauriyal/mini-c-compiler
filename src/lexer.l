%{
#include <iostream>
#include <string>
#include <cstdlib>

#include "parser.tab.h"

int line_num = 1;
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

"int"       { return 258; }
"float"     { return 259; }
"char"      { return 260; }
"if"        { return 261; }
"else"      { return 262; }
"while"     { return 263; }
"for"       { return 264; }

"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MULTIPLY; }
"/"         { return DIVIDE; }
"="         { return ASSIGN; }
"=="        { return EQ; }
"<"         { return LESS; }
">"         { return GREATER; }
"("         { return LPAREN; }
")"         { return RPAREN; }
"{"         { return LBRACE; }
"}"         { return RBRACE; }
";"         { return SEMICOLON; }

{INT_CONST}   { 
                /* yylval.ival = atoi(yytext); // Will pass the actual number to Bison later */
                return INT_LITERAL; 
              }

{FLOAT_CONST} { 
                /* yylval.fval = atof(yytext); // Will pass the actual float to Bison later */
                return FLOAT_LITERAL; 
              }

{ID}          { 
                /* yylval.sval = strdup(yytext); // Passes the variable name */
                return IDENTIFIER; 
              }

[ \t\r]+      { }
\n            { line_num++; }

"//".*        { }

"/*"([^*]|\*+[^*/])*\*+"/" {  }

.             { std::cerr << "Lexical Error at line " << line_num << ": Unrecognized character '" << yytext << "'\n"; }

%%