%{
#include <iostream>
#include <string>
#include <memory>
#include "../include/ast.h"
#include "../include/symbol_table.h"

extern int yylex();
extern int line_num;
extern char* yytext;
void yyerror(const char* s);

// This global pointer will hold the very top of our completed syntax trees
ASTNodePtr ast_root = nullptr; 
%}

/* Define the types of data our tokens and rules can return */
%union {
    int ival;
    float fval;
    char* sval;
    class ASTNode* node; // Raw pointer for Bison's C-based union
}

%token INT FLOAT CHAR IF ELSE WHILE FOR
%token PLUS MINUS MULTIPLY DIVIDE ASSIGN EQ LESS GREATER
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON

%token <ival> INT_LITERAL
%token <fval> FLOAT_LITERAL
%token <sval> IDENTIFIER

/* Link our grammar rules to return ASTNode pointers */
%type <node> program statements statement declaration assignment_expr 
%type <node> if_statement while_statement for_statement expression
%type <ival> type

%left LESS GREATER EQ
%left PLUS MINUS
%left MULTIPLY DIVIDE

%%

program:
    statements { 
        // When the entire file is parsed, save the root block to our global variable
        ast_root = std::shared_ptr<ASTNode>($1); 
    }
    ;

statements:
    statements statement {
        // Cast the existing block and add the new statement to it
        BlockNode* block = (BlockNode*)$1;
        if ($2) block->addStatement(std::shared_ptr<ASTNode>($2));
        $$ = block;
    }
    | /* Empty */ {
        // Create a new empty block when starting a scope
        $$ = new BlockNode();
    }
    ;

statement:
    declaration SEMICOLON { $$ = $1; }
    | assignment_expr SEMICOLON { $$ = $1; }
    | if_statement { $$ = $1; }
    | while_statement { $$ = $1; }
    | for_statement { $$ = $1; }
    ;

declaration:
    type IDENTIFIER {
        $$ = new DeclarationNode((DataType)$1, $2);
        free($2);
    }
    | type IDENTIFIER ASSIGN expression {
        // NEW RULE: Handles 'int z = 10;'
        $$ = new DeclarationNode((DataType)$1, $2, std::shared_ptr<ASTNode>($4));
        free($2);
    }
    ;

type:
    INT { $$ = TYPE_INT; }
    | FLOAT { $$ = TYPE_FLOAT; }
    | CHAR { $$ = TYPE_CHAR; }
    ;

assignment_expr:
    IDENTIFIER ASSIGN expression {
        $$ = new AssignmentNode($1, std::shared_ptr<ASTNode>($3));
        free($1);
    }
    ;

if_statement:
    IF LPAREN expression RPAREN LBRACE statements RBRACE {
        $$ = new IfNode(std::shared_ptr<ASTNode>($3), std::shared_ptr<ASTNode>($6));
    }
    | IF LPAREN expression RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE {
        $$ = new IfNode(std::shared_ptr<ASTNode>($3), std::shared_ptr<ASTNode>($6), std::shared_ptr<ASTNode>($10));
    }
    ;

while_statement:
    WHILE LPAREN expression RPAREN LBRACE statements RBRACE {
        $$ = new WhileNode(std::shared_ptr<ASTNode>($3), std::shared_ptr<ASTNode>($6));
    }
    ;

for_statement:
    FOR LPAREN assignment_expr SEMICOLON expression SEMICOLON assignment_expr RPAREN LBRACE statements RBRACE {
        $$ = new ForNode(std::shared_ptr<ASTNode>($3), std::shared_ptr<ASTNode>($5), std::shared_ptr<ASTNode>($7), std::shared_ptr<ASTNode>($10));
    }
    ;

expression:
    expression PLUS expression { $$ = new BinaryOpNode("+", std::shared_ptr<ASTNode>($1), std::shared_ptr<ASTNode>($3)); }
    | expression MINUS expression { $$ = new BinaryOpNode("-", std::shared_ptr<ASTNode>($1), std::shared_ptr<ASTNode>($3)); }
    | expression MULTIPLY expression { $$ = new BinaryOpNode("*", std::shared_ptr<ASTNode>($1), std::shared_ptr<ASTNode>($3)); }
    | expression DIVIDE expression { $$ = new BinaryOpNode("/", std::shared_ptr<ASTNode>($1), std::shared_ptr<ASTNode>($3)); }
    | expression EQ expression { $$ = new BinaryOpNode("==", std::shared_ptr<ASTNode>($1), std::shared_ptr<ASTNode>($3)); }
    | expression LESS expression { $$ = new BinaryOpNode("<", std::shared_ptr<ASTNode>($1), std::shared_ptr<ASTNode>($3)); }
    | expression GREATER expression { $$ = new BinaryOpNode(">", std::shared_ptr<ASTNode>($1), std::shared_ptr<ASTNode>($3)); }
    | LPAREN expression RPAREN { $$ = $2; } // Parentheses don't need a node, they just enforce grouping
    | IDENTIFIER { 
        $$ = new VariableNode($1); 
        free($1); 
    }
    | INT_LITERAL { $$ = new NumberNode(std::to_string($1), false); }
    | FLOAT_LITERAL { $$ = new NumberNode(std::to_string($1), true); }
    ;

%%

void yyerror(const char* s) {
    std::cerr << "\n[ERROR] Syntax Error at line " << line_num << ": " << s << " near '" << yytext << "'\n";
}