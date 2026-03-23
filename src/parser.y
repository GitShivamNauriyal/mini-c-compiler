%{
#include <iostream>
#include <string>

extern int yylex();
extern int line_num;
extern char* yytext;

void yyerror(const char* s);
%}

%union {
    int ival;
    float fval;
    char* sval;
}

%token INT FLOAT CHAR IF ELSE WHILE FOR
%token PLUS MINUS MULTIPLY DIVIDE ASSIGN EQ LESS GREATER
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON

%token <ival> INT_LITERAL
%token <fval> FLOAT_LITERAL
%token <sval> IDENTIFIER

/* Operator Precedence: */
%left LESS GREATER EQ
%left PLUS MINUS
%left MULTIPLY DIVIDE

%%
/* --- THE GRAMMAR RULES --- */

program:
    statements { std::cout << "\n[SUCCESS] Syntax Analysis: Parsing completed without errors!\n"; }
    ;

statements:
    statements statement
    | /* Empty: allows for zero or more statements */
    ;

statement:
    declaration SEMICOLON
    | assignment_expr SEMICOLON
    | if_statement
    | while_statement
    | for_statement
    ;

declaration:
    type IDENTIFIER
    ;

type:
    INT | FLOAT | CHAR
    ;

assignment_expr:
    IDENTIFIER ASSIGN expression
    ;

if_statement:
    IF LPAREN expression RPAREN LBRACE statements RBRACE
    | IF LPAREN expression RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE
    ;

while_statement:
    WHILE LPAREN expression RPAREN LBRACE statements RBRACE
    ;

for_statement:
    FOR LPAREN assignment_expr SEMICOLON expression SEMICOLON assignment_expr RPAREN LBRACE statements RBRACE
    ;

expression:
    expression PLUS expression
    | expression MINUS expression
    | expression MULTIPLY expression
    | expression DIVIDE expression
    | expression EQ expression
    | expression LESS expression
    | expression GREATER expression
    | LPAREN expression RPAREN
    | IDENTIFIER
    | INT_LITERAL
    | FLOAT_LITERAL
    ;

%%
/* --- C++ CODE INJECTION --- */

void yyerror(const char* s) {
    std::cerr << "\n[ERROR] Syntax Error at line " << line_num << ": " << s << " near '" << yytext << "'\n";
}