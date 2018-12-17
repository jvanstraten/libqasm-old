%{
    #include <cstdlib>
    #include <string>
    #include "cqasm2/ast/ast.hpp"
    int yyerror(char const *s);
    extern int yylex(void);
    extern int yylineno;
    bool yyrecovered;
    using namespace cqasm2::ast;
%}

/* YYSTYPE union */
%union {
    char *tok;
    cqasm2::ast::Node *node;
};

/* Version header */
%token <tok> VERSION

/* End of line */
%token NEWLINE

/* Resource types */
%token <tok> TYPE_QUBIT TYPE_BOOLEAN TYPE_INT TYPE_UINT
%token <tok> TYPE_FIXED TYPE_UFIXED TYPE_FLOAT TYPE_DOUBLE

/* High-level keywords */
%token DEF IF ELSE FOR INCLUDE GOTO

/* Map statement */
%token MAP

/* Gate declaration */
%token GATE

/* Pragma statement */
%token PRAGMA

/* Numeric literals */
%token <tok> LIT_BOOLEAN
%token <tok> LIT_INT_DEC LIT_INT_HEX LIT_INT_BIN
%token <tok> LIT_UINT_DEC LIT_UINT_HEX LIT_UINT_BIN
%token <tok> LIT_FIXED_HEX LIT_FIXED_BIN
%token <tok> LIT_UFIXED_HEX LIT_UFIXED_BIN
%token <tok> LIT_FLOAT
%token <tok> LIT_DOUBLE
%token <tok> LIT_PI
%token <tok> LIT_EU
%token <tok> LIT_IM

/* Matrix literals */
%token <tok> MATRIX_OPEN MATRIX_CLOSE

/* String and JSON literals */
%token <tok> STRING_OPEN STRING_CLOSE
%token <tok> JSON_OPEN JSON_CLOSE
%token <tok> APPEND APPEND_ESCAPE

/* Identifiers */
%token <tok> IDENTIFIER

/* Conditional execution modifier */
%token <tok> CDASH

/* Multi-character operators */
%token CMP_EQ CMP_NE CMP_LE CMP_GE
%token LOGICAL_OR LOGICAL_XOR LOGICAL_AND
%token SHIFT_LEFT SHIFT_RIGHT LOG_SHIFT_RIGHT
%token DIV_INT POWER
%token ASSIGN

/* Illegal tokens */
%token <tok> BAD_RESERVED BAD_NUMBER BAD_UNTERM_STRING BAD_CHARACTER

/* The > at the end of type parameterization (TYPE_PARAM) must have higher
precedence than shifting an IDENTIFIER, or "int<3> a" would be interpreted
as "int<(3>a)" and would thus always lead to a syntax error. */
%nonassoc IDENTIFIER
%nonassoc TYPE_PARAM

/* Associativity rules for static expressions. The lowest precedence level
comes first. */
%left ',' ':'                                /* SIMD/SGMQ indexation */
%right '?'                                   /* ?: ternary operator */
%left LOGICAL_OR                             /* Boolean or */
%left LOGICAL_XOR                            /* Boolean xor */
%left LOGICAL_AND                            /* Boolean and */
%left '|'                                    /* Bitwise or */
%left '^'                                    /* Bitwise xor */
%left '&'                                    /* Bitwise and */
%left CMP_EQ CMP_NE                          /* Equality */
%left CMP_LE CMP_GE '<' '>'                  /* Comparison */
%left SHIFT_LEFT SHIFT_RIGHT LOG_SHIFT_RIGHT /* Shifts */
%left '+' '-'                                /* Addition/subtraction */
%left '*' '/' '%' DIV_INT                    /* Multiplication/division */
%right POWER                                 /* Python-like power */
%right UPLUS UMINUS '!' '~' TYPECAST         /* Unaries */
%left '(' '[' '.'                            /* Call, indexation, disambig. */

/* In a single-line parallel statement, possibly containing only a single gate,
annotations apply to the gate, not the bundle. Therefore '@' has greater
priority than '|' */
%left '@'
%nonassoc BUNDLE

/* Typenames for nonterminals */
%type <node> Type

/* Misc. Yacc directives */
%error-verbose
%locations
%start Input

%%

Newline         : NEWLINE
                | NEWLINE Newline
                ;

OptNewline      : Newline
                |
                ;

Type            : TYPE_QUBIT                                                    { $$ = new QubitType(); }
                | TYPE_BOOLEAN                                                  { $$ = new QubitType(); }
                | TYPE_INT    '<' Expression '>' %prec TYPE_PARAM               { $$ = new QubitType(); }
                | TYPE_UINT   '<' Expression '>' %prec TYPE_PARAM               { $$ = new QubitType(); }
                | TYPE_FIXED  '<' Expression ',' Expression '>' %prec TYPE_PARAM{ $$ = new QubitType(); }
                | TYPE_UFIXED '<' Expression ',' Expression '>' %prec TYPE_PARAM{ $$ = new QubitType(); }
                | TYPE_FLOAT                                                    { $$ = new QubitType(); }
                | TYPE_DOUBLE                                                   { $$ = new QubitType(); }
                ;

/* All literals that reduce to numericals. */
NumericLiteral  : LIT_BOOLEAN
                | LIT_INT_DEC
                | LIT_INT_HEX
                | LIT_INT_BIN
                | LIT_UINT_DEC
                | LIT_UINT_HEX
                | LIT_UINT_BIN
                | LIT_FIXED_HEX
                | LIT_FIXED_BIN
                | LIT_UFIXED_HEX
                | LIT_UFIXED_BIN
                | LIT_FLOAT
                | LIT_DOUBLE
                | LIT_PI
                | LIT_EU
                | LIT_IM
                ;

/* These expressions are almost fully-featured C. Of course only a subset of
this is semantically legal depending on context, and almost all of these rules
must be statically reduced by desugaring. */
Expression      : NumericLiteral
                | IDENTIFIER
                | IDENTIFIER '.' IDENTIFIER
                | IDENTIFIER '[' IndexList ']' %prec '['
                | IDENTIFIER '[' IndexList ']'  '.' IDENTIFIER %prec '['
                | IDENTIFIER '(' ExpressionList ')' %prec '('
                | IDENTIFIER '(' ')' %prec '('
                | '(' Expression ')'
                | '(' Type ')' Expression %prec TYPECAST
                | '+' Expression %prec UPLUS
                | '-' Expression %prec UMINUS
                | '!' Expression
                | '~' Expression
                | Expression '*' Expression
                | Expression '/' Expression
                | Expression '%' Expression
                | Expression '+' Expression
                | Expression '-' Expression
                | Expression SHIFT_LEFT Expression
                | Expression SHIFT_RIGHT Expression
                | Expression LOG_SHIFT_RIGHT Expression
                | Expression CMP_GE Expression
                | Expression CMP_LE Expression
                | Expression '>' Expression
                | Expression '<' Expression
                | Expression CMP_EQ Expression
                | Expression CMP_NE Expression
                | Expression '&' Expression
                | Expression '^' Expression
                | Expression '|' Expression
                | Expression LOGICAL_AND Expression
                | Expression LOGICAL_XOR Expression
                | Expression LOGICAL_OR Expression
                | Expression '?' Expression ':' Expression %prec '?'
                | error                                                         { yyrecovered = true; }
                ;

/* List of one or more expressions. */
ExpressionList  : ExpressionList ',' Expression
                | Expression %prec ','
                ;

/* Indexation modes. */
IndexEntry      : Expression
                | Expression ':' Expression
                ;

IndexList       : IndexList ',' IndexEntry
                | IndexEntry
                ;

/* Matrix syntax, used to describe custom gates. */
MatrixData      : MatrixData Newline ExpressionList
                | ExpressionList
                ;

Matrix          : MATRIX_OPEN OptNewline MatrixData OptNewline MATRIX_CLOSE
                | '[' ExpressionList ']'
                ;

/* String and JSON syntax. */
StringData      : StringData APPEND
                | StringData APPEND_ESCAPE
                |
                ;

String          : STRING_OPEN StringData STRING_CLOSE
                ;

Json            : JSON_OPEN StringData JSON_CLOSE
                ;

/* Operands in cQASM can be expressions, strings (for print and error), or
matrices (for the U gate). */
Operand         : Expression %prec BUNDLE
                | Matrix
                | String
                ;

/* List of operands. */
OperandList     : OperandList ',' Operand
                | Operand %prec ','
                ;

/* The information caried by an annotation or pragma statement. */
AnnotationData  : IDENTIFIER IDENTIFIER
                | IDENTIFIER IDENTIFIER Json
                ;

/* Pragma statement. */
Pragma          : PRAGMA AnnotationData
                ;

/* Resource declaration statament. */
Resource        : Type IDENTIFIER
                | Type IDENTIFIER '=' Expression
                | Type IDENTIFIER '[' Expression ']' '=' '{' '}'
                | Type IDENTIFIER '[' Expression ']' '=' '{' ExpressionList '}'
                | Type IDENTIFIER '[' ']' '=' '{' ExpressionList '}'
                | TYPE_QUBIT NumericLiteral
                ;

/* Resource mapping statement. */
Mapping         : MAP Expression ',' IDENTIFIER
                | MAP IDENTIFIER ASSIGN Expression
                ;

/* Custom gate declaration. */
GateDecl        : GATE IDENTIFIER Matrix

/* Subcircuit statement. */
Subcircuit      : '.' IDENTIFIER
                | '.' IDENTIFIER '(' NumericLiteral ')'
                ;

/* Label statement. */
Label           : IDENTIFIER ':'
                ;

/* Name for gates, with optional conditional syntax. */
GateRef         : IDENTIFIER
                | CDASH GateRef Expression ','
                ;

/* Gate execution. This includes classical instructions. Note that this is
NOT directly a statement grammatically; they are first made part of a bundle.
*/
Gate            : GateRef
                | GateRef OperandList
                | GateRef OperandList ASSIGN OperandList
                | IF Expression GOTO IDENTIFIER
                ;

/* Gates are not statements but can be annotated, so they need their own
annotation rule. */
AnnotGate       : AnnotGate '@' AnnotationData
                | Gate
                ;

/* Single-line bundling syntax. */
SLParGateList   : SLParGateList '|' AnnotGate
                | AnnotGate %prec '|'
                ;

/* Multi-line bundling syntax. */
CBParGateList   : CBParGateList Newline SLParGateList
                | SLParGateList
                ;

/* Bundle statement. */
Bundle          : SLParGateList
                | '{' OptNewline CBParGateList OptNewline '}'
                ;

Statement       : Pragma
                | Resource
                | Mapping
                | GateDecl
                | Subcircuit
                | Label
            /*  | StaticIfElse */
            /*  | StaticFor */
            /*  | StaticDef */
            /*  | StaticInclude */
                | Bundle
                | error                                                         { yyrecovered = true; }
                ;

AnnotStatement  : AnnotStatement '@' AnnotationData
                | Statement
                ;

Statements      : Statements AnnotStatement Newline
                |
                ;

Input           : VERSION Newline Statements
                ;


%%
int yyerror(char const *s) {
    printf("On %d:%d: %s\n", yylloc.first_line, yylloc.first_column, s);
}
