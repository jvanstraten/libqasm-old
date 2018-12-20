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
    char *str;
    cqasm2::ast::Type *typ;
    cqasm2::ast::NumericLiteral *numlit;
    cqasm2::ast::Expression *expr;
};

/* Typenames for nonterminals */
%type <typ> Type
%type <numlit> NumericLiteral
%type <expr> Expression

/* Version header */
%token <str> VERSION

/* End of line */
%token NEWLINE

/* Resource types */
%token TYPE_QUBIT TYPE_BOOLEAN TYPE_INT TYPE_UINT
%token TYPE_FIXED TYPE_UFIXED TYPE_FLOAT TYPE_DOUBLE

/* High-level keywords */
%token MAP LET SET DEF IF ELSE FOR INCLUDE GOTO

/* Pragma statement */
%token PRAGMA

/* Numeric literals */
%token <str> LIT_INT_DEC LIT_INT_HEX LIT_INT_BIN
%token <str> LIT_UINT_DEC LIT_UINT_HEX LIT_UINT_BIN
%token <str> LIT_FIXED_HEX LIT_FIXED_BIN
%token <str> LIT_UFIXED_HEX LIT_UFIXED_BIN
%token <str> LIT_FLOAT LIT_DOUBLE
%token LIT_TRUE LIT_FALSE LIT_PI LIT_EU LIT_IM

/* Matrix literals */
%token MATRIX_OPEN MATRIX_CLOSE

/* String and JSON literals */
%token STRING_OPEN STRING_CLOSE
%token JSON_OPEN JSON_CLOSE
%token <str> STRBUILD_APPEND STRBUILD_ESCAPE

/* Identifiers */
%token <str> IDENTIFIER

/* Conditional execution modifier */
%token CDASH

/* Multi-character operators */
%token CMP_EQ CMP_NE CMP_LE CMP_GE
%token LOGICAL_OR LOGICAL_XOR LOGICAL_AND
%token SHIFT_LEFT SHIFT_RIGHT
%token DIV_INT POWER
%token ASSIGN

/* Illegal tokens */
%token BAD_RESERVED BAD_NUMBER BAD_CHARACTER

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

/* Misc. Yacc directives */
%error-verbose
%locations
%start Program

%%

/* One or more newlines. */
Newline         : Newline NEWLINE
                | NEWLINE
                ;

/* Zero or more newlines. */
OptNewline      : Newline
                |
                ;

/* Type specifiers. */
Type            : TYPE_QUBIT                                                    { $$ = new QubitType(); }
                | TYPE_BOOLEAN                                                  { $$ = new NumericType(false); }
                | TYPE_INT    '<' Expression '>' %prec TYPE_PARAM               { $$ = new NumericType(true,  std::make_shared<Expression>($3)); }
                | TYPE_UINT   '<' Expression '>' %prec TYPE_PARAM               { $$ = new NumericType(false, std::make_shared<Expression>($3)); }
                | TYPE_FIXED  '<' Expression ',' Expression '>' %prec TYPE_PARAM{ $$ = new NumericType(true,  std::make_shared<Expression>($3), std::make_shared<Expression>($5)); }
                | TYPE_UFIXED '<' Expression ',' Expression '>' %prec TYPE_PARAM{ $$ = new NumericType(false, std::make_shared<Expression>($3), std::make_shared<Expression>($5)); }
                | TYPE_FLOAT                                                    { $$ = new FloatType(); }
                | TYPE_DOUBLE                                                   { $$ = new DoubleType(); }
                ;

/* All literals that reduce to numericals. */
NumericLiteral  : LIT_INT_DEC                                                   { $$ = new DecLiteral($1); free($1); }
                | LIT_UINT_DEC                                                  { $$ = new DecLiteral($1); free($1); }
                | LIT_INT_HEX                                                   { $$ = new HexLiteral($1); free($1); }
                | LIT_UINT_HEX                                                  { $$ = new HexLiteral($1); free($1); }
                | LIT_FIXED_HEX                                                 { $$ = new HexLiteral($1); free($1); }
                | LIT_UFIXED_HEX                                                { $$ = new HexLiteral($1); free($1); }
                | LIT_INT_BIN                                                   { $$ = new BinLiteral($1); free($1); }
                | LIT_UINT_BIN                                                  { $$ = new BinLiteral($1); free($1); }
                | LIT_FIXED_BIN                                                 { $$ = new BinLiteral($1); free($1); }
                | LIT_UFIXED_BIN                                                { $$ = new BinLiteral($1); free($1); }
                | LIT_FLOAT                                                     { $$ = new FloatLiteral($1); free($1); }
                | LIT_DOUBLE                                                    { $$ = new FloatLiteral($1); free($1); }
                | LIT_TRUE                                                      { $$ = new NamedLiteral("true"); }
                | LIT_FALSE                                                     { $$ = new NamedLiteral("false"); }
                | LIT_PI                                                        { $$ = new NamedLiteral("pi"); }
                | LIT_EU                                                        { $$ = new NamedLiteral("eu"); }
                | LIT_IM                                                        { $$ = new NamedLiteral("im"); }
                ;

/* These expressions are almost fully-featured C. Of course only a subset of
this is semantically legal depending on context, and almost all of these rules
must be statically reduced by desugaring. */
Expression      : NumericLiteral                                                { $$ = $1; }
                | IDENTIFIER                                                    { $$ = new Identifier($1); free($1); }
                | Expression '.' IDENTIFIER                                     { $$ = new Subscript($1, $3); free($3); }
                | Expression '[' IndexList ']' %prec '['                        { $$ = $1; /* TODO */ }
                | IDENTIFIER '(' ExpressionList ')' %prec '('                   { $$ = new Operation(true, $1, $3); }
                | '(' Expression ')'                                            { $$ = $2; }
                | '(' Type ')' Expression %prec TYPECAST                        { $$ = new TypeCast($2, $4); }
                | '(' SHIFT_LEFT Expression ')' Expression %prec TYPECAST       { $$ = new ShiftCast(true, $3, $5); }
                | '(' SHIFT_RIGHT Expression ')' Expression %prec TYPECAST      { $$ = new ShiftCast(false, $3, $5); }
                | '+' Expression %prec UPLUS                                    { $$ = new Operation(false, "+", new ExpressionList()->push_expr($2)); }
                | '-' Expression %prec UMINUS                                   { $$ = new Operation(false, "-", new ExpressionList()->push_expr($2)); }
                | '!' Expression                                                { $$ = new Operation(false, "+", new ExpressionList()->push_expr($2)); }
                | '~' Expression                                                { $$ = new Operation(false, "+", new ExpressionList()->push_expr($2)); }
                | Expression '*' Expression
                | Expression '/' Expression
                | Expression '%' Expression
                | Expression '+' Expression
                | Expression '-' Expression
                | Expression SHIFT_LEFT Expression
                | Expression SHIFT_RIGHT Expression
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

MatrixLiteral   : MATRIX_OPEN OptNewline MatrixData OptNewline MATRIX_CLOSE
                | '[' ExpressionList ']'
                ;

/* String builder. This accumulates JSON/String data, mostly
character-by-character. */
StringBuilder   : StringBuilder STRBUILD_APPEND
                | StringBuilder STRBUILD_ESCAPE
                |
                ;

/* String literal. */
StringLiteral   : STRING_OPEN StringBuilder STRING_CLOSE
                ;

/* JSON literal. */
JsonLiteral     : JSON_OPEN StringBuilder JSON_CLOSE
                ;

/* Operands in cQASM can be expressions, strings (for print and error), or
matrices (for the U gate). */
Operand         : Expression %prec BUNDLE
                | MatrixLiteral
                | StringLiteral
                | JsonLiteral
                ;

/* List of operands. */
OperandList     : OperandList ',' Operand
                | Operand %prec ','
                ;

/* List of identifiers. */
IdentifierList  : IdentifierList ',' IDENTIFIER
                | IDENTIFIER
                ;

/* The information caried by an annotation or pragma statement. */
AnnotationData  : IDENTIFIER IDENTIFIER
                | IDENTIFIER IDENTIFIER ':' OperandList
                ;

/* Pragma statement. */
Pragma          : PRAGMA AnnotationData
                ;

/* Resource declaration statament. */
Resource        : Type IDENTIFIER
                | Type IDENTIFIER '=' Expression
                | Type IDENTIFIER '[' Expression ']'
                | Type IDENTIFIER '[' Expression ']' '=' Expression
                | LET IDENTIFIER '=' Expression
                | TYPE_QUBIT NumericLiteral
                ;

/* Resource mapping statement. */
Mapping         : MAP Expression ',' IDENTIFIER
                | MAP IDENTIFIER ASSIGN Expression
                ;

/* Resource assignments. */
Assignment      : SET Expression '=' Expression
                ;

/* Macro subroutine definition. */
MacroDef        : DEF IDENTIFIER '(' ')' Block
                | DEF IDENTIFIER '(' IdentifierList ')' Block
                | DEF IDENTIFIER '(' IdentifierList ASSIGN IdentifierList ')' Block
                ;

/* For loop macro. */
MacroFor        : FOR IDENTIFIER '=' '[' IndexList ']' Block
                ;

/* If/else macro. */
MacroIfElse     : IF '(' Expression ')' Block
                | IF Block ELSE Block
                ;

/* Include statement. */
Include         : INCLUDE StringLiteral
                ;

/* Subcircuit statement. */
Subcircuit      : '.' IDENTIFIER
                | '.' IDENTIFIER '(' NumericLiteral ')'
                ;

/* Label statement. */
Label           : IDENTIFIER ':'
                ;

/* Name for gates, with optional conditional syntax. */
GateType        : IDENTIFIER
                | CDASH GateType Expression ','
                ;

/* Gate execution. This includes classical instructions. Note that this is
NOT directly a statement grammatically; they are first made part of a bundle.
*/
Gate            : GateType
                | GateType OperandList
                | GateType OperandList ASSIGN OperandList
                | IF Expression GOTO IDENTIFIER
                | GOTO IDENTIFIER
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

/* Any of the supported statements. */
Statement       : Pragma
                | Resource
                | Mapping
                | Assignment
                | MacroDef
                | MacroFor
                | MacroIfElse
                | Include
                | Subcircuit
                | Label
                | Bundle
                | error                                                         { yyrecovered = true; }
                ;

/* Statement with annotations attached to it. */
AnnotStatement  : AnnotStatement '@' AnnotationData
                | Statement
                ;

/* List of one or more statements. */
BlockData       : BlockData Newline AnnotStatement
                | AnnotStatement
                ;

/* Block of code; zero or more statements. */
Block           : '{' OptNewline BlockData OptNewline '}'
                | '{' OptNewline '}'
                ;

/* Toplevel. */
Program         : OptNewline VERSION Newline BlockData OptNewline
                | OptNewline VERSION OptNewline
                ;


%%
int yyerror(char const *s) {
    printf("On %d:%d: %s\n", yylloc.first_line, yylloc.first_column, s);
}
