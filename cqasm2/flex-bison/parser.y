%{
    #include <cstdlib>
    #include <string>
    #include <iostream>
    #include <string.h>
    #include "cqasm2/ast/ast.hpp"
    void yyerror(char const *s);
    extern int yylex(void);
    extern int yylineno;
    bool yyrecovered;
    using namespace cqasm2::ast;
    pprint_opts_t pprint_opts;
%}

/* YYSTYPE union */
%union {
    char                            *str;
    cqasm2::ast::Type               *typ;
    cqasm2::ast::NumericLiteral     *nlit;
    cqasm2::ast::ArrayLiteral       *alit;
    cqasm2::ast::Expression         *expr;
    cqasm2::ast::ExpressionList     *expl;
    cqasm2::ast::IndexEntry         *idxe;
    cqasm2::ast::IndexList          *idxl;
    cqasm2::ast::MatrixLiteral2     *mat2;
    cqasm2::ast::MatrixLiteral      *mat;
    cqasm2::ast::StringBuilder      *strb;
    cqasm2::ast::StringLiteral      *strl;
    cqasm2::ast::JsonLiteral        *jsl;
    cqasm2::ast::Operand            *oper;
    cqasm2::ast::OperandList        *opl;
    cqasm2::ast::IdentifierList     *idl;
    cqasm2::ast::AnnotationData     *adat;
    cqasm2::ast::GateType           *gtyp;
    cqasm2::ast::UnresolvedGate     *gate;
    cqasm2::ast::Bundle             *bun;
    cqasm2::ast::Statement          *stmt;
    cqasm2::ast::Block              *blk;
    cqasm2::ast::Program            *prgm;
};

/* Typenames for nonterminals */
%type <typ> Type
%type <nlit> NumericLiteral
%type <alit> ArrayLiteral
%type <expr> Expression
%type <expl> ExpressionList
%type <idxe> IndexEntry
%type <idxl> IndexList
%type <mat2> MatrixLiteral2
%type <mat>  MatrixLiteral
%type <strb> StringBuilder
%type <strl> StringLiteral
%type <jsl>  JsonLiteral
%type <oper> Operand
%type <opl>  OperandList
%type <idl>  IdentifierList
%type <adat> AnnotationData
%type <gtyp> GateType
%type <gate> Gate AnnotGate
%type <bun>  SLParGateList CBParGateList
%type <stmt> Statement AnnotStatement
%type <blk>  StatementList Block
%type <prgm> Program

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
%token <str> LIT_NAMED

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
comes first. NOTE: expression precedence must match the values in
operators.[ch]pp for correct pretty-printing! */
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
                | TYPE_INT    '<' Expression '>' %prec TYPE_PARAM               { $$ = new NumericType(true,  $3); }
                | TYPE_UINT   '<' Expression '>' %prec TYPE_PARAM               { $$ = new NumericType(false, $3); }
                | TYPE_FIXED  '<' Expression ',' Expression '>' %prec TYPE_PARAM{ $$ = new NumericType(true,  $3, $5); }
                | TYPE_UFIXED '<' Expression ',' Expression '>' %prec TYPE_PARAM{ $$ = new NumericType(false, $3, $5); }
                | TYPE_FLOAT                                                    { $$ = new FloatType(); }
                | TYPE_DOUBLE                                                   { $$ = new DoubleType(); }
                ;

/* All literals that reduce to numericals. */
NumericLiteral  : LIT_INT_DEC                                                   { $$ = new DecLiteral($1); }
                | LIT_UINT_DEC                                                  { $$ = new DecLiteral($1); }
                | LIT_INT_HEX                                                   { $$ = new HexLiteral($1); }
                | LIT_UINT_HEX                                                  { $$ = new HexLiteral($1); }
                | LIT_FIXED_HEX                                                 { $$ = new HexLiteral($1); }
                | LIT_UFIXED_HEX                                                { $$ = new HexLiteral($1); }
                | LIT_INT_BIN                                                   { $$ = new BinLiteral($1); }
                | LIT_UINT_BIN                                                  { $$ = new BinLiteral($1); }
                | LIT_FIXED_BIN                                                 { $$ = new BinLiteral($1); }
                | LIT_UFIXED_BIN                                                { $$ = new BinLiteral($1); }
                | LIT_FLOAT                                                     { $$ = new FloatLiteral($1); }
                | LIT_DOUBLE                                                    { $$ = new FloatLiteral($1); }
                | LIT_NAMED                                                     { $$ = new NamedLiteral($1); }
                ;

/* Array literal. */
ArrayLiteral    : '{' ExpressionList '}'                                        { $$ = new ArrayLiteral($2); }
                ;

/* These expressions are almost fully-featured C. Of course only a subset of
this is semantically legal depending on context, and almost all of these rules
must be statically reduced by desugaring. */
Expression      : NumericLiteral                                                { $$ = $1; }
                | ArrayLiteral                                                  { $$ = $1; }
                | IDENTIFIER                                                    { $$ = new Identifier($1); }
                | Expression '.' IDENTIFIER                                     { $$ = new Subscript($1, $3); }
                | Expression '[' IndexList ']' %prec '['                        { $$ = new Indexation($1, $3); }
                | IDENTIFIER '(' ExpressionList ')' %prec '('                   { $$ = new Function($1, $3); }
                | '(' Expression ')'                                            { $$ = $2; }
                | '(' Type ')' Expression %prec TYPECAST                        { $$ = new TypeCast($2, $4); }
                | '(' SHIFT_LEFT Expression ')' Expression %prec TYPECAST       { $$ = new ShiftCast(true, $3, $5); }
                | '(' SHIFT_RIGHT Expression ')' Expression %prec TYPECAST      { $$ = new ShiftCast(false, $3, $5); }
                | '+' Expression %prec UPLUS                                    { $$ = $2; }
                | '-' Expression %prec UMINUS                                   { $$ = new Operation(operators::NEG,  $2); }
                | '!' Expression                                                { $$ = new Operation(operators::NOT,  $2); }
                | '~' Expression                                                { $$ = new Operation(operators::INV,  $2); }
                | Expression POWER Expression                                   { $$ = new Operation(operators::POW,  $1, $3); }
                | Expression '*' Expression                                     { $$ = new Operation(operators::MUL,  $1, $3); }
                | Expression '/' Expression                                     { $$ = new Operation(operators::DIV,  $1, $3); }
                | Expression DIV_INT Expression                                 { $$ = new Operation(operators::IDIV, $1, $3); }
                | Expression '%' Expression                                     { $$ = new Operation(operators::MOD,  $1, $3); }
                | Expression '+' Expression                                     { $$ = new Operation(operators::ADD,  $1, $3); }
                | Expression '-' Expression                                     { $$ = new Operation(operators::SUB,  $1, $3); }
                | Expression SHIFT_LEFT Expression                              { $$ = new Operation(operators::SHL,  $1, $3); }
                | Expression SHIFT_RIGHT Expression                             { $$ = new Operation(operators::SHR,  $1, $3); }
                | Expression '>' Expression                                     { $$ = new Operation(operators::CGT,  $1, $3); }
                | Expression '<' Expression                                     { $$ = new Operation(operators::CLT,  $1, $3); }
                | Expression CMP_GE Expression                                  { $$ = new Operation(operators::CGE,  $1, $3); }
                | Expression CMP_LE Expression                                  { $$ = new Operation(operators::CLE,  $1, $3); }
                | Expression CMP_EQ Expression                                  { $$ = new Operation(operators::CEQ,  $1, $3); }
                | Expression CMP_NE Expression                                  { $$ = new Operation(operators::CNE,  $1, $3); }
                | Expression '&' Expression                                     { $$ = new Operation(operators::AND,  $1, $3); }
                | Expression '^' Expression                                     { $$ = new Operation(operators::XOR,  $1, $3); }
                | Expression '|' Expression                                     { $$ = new Operation(operators::OR,   $1, $3); }
                | Expression LOGICAL_AND Expression                             { $$ = new Operation(operators::LAND, $1, $3); }
                | Expression LOGICAL_XOR Expression                             { $$ = new Operation(operators::LXOR, $1, $3); }
                | Expression LOGICAL_OR Expression                              { $$ = new Operation(operators::LOR,  $1, $3); }
                | Expression '?' Expression ':' Expression %prec '?'            { $$ = new Operation(operators::SLCT, $1, $3, $5); }
                | error                                                         { $$ = new ErroneousExpression(); yyrecovered = true; }
                ;

/* List of one or more expressions. */
ExpressionList  : ExpressionList ',' Expression                                 { $$ = $1->push_expr($3); }
                | Expression %prec ','                                          { $$ = (new ExpressionList())->push_expr($1); }
                ;

/* Indexation modes. */
IndexEntry      : Expression                                                    { $$ = new IndexEntry($1); }
                | Expression ':' Expression                                     { $$ = new IndexEntry($1, $3); }
                ;

IndexList       : IndexList ',' IndexEntry                                      { $$ = $1->push_ent($3); }
                | IndexEntry                                                    { $$ = (new IndexList())->push_ent($1); }
                ;

/* Matrix syntax, used to describe custom gates. */
MatrixLiteral2  : MatrixLiteral2 Newline ExpressionList                         { $$ = $1->push_row($3); }
                | ExpressionList                                                { $$ = (new MatrixLiteral2())->push_row($1); }
                ;

MatrixLiteral   : MATRIX_OPEN OptNewline MatrixLiteral2 OptNewline MATRIX_CLOSE { $$ = $3; }
                | '[' ExpressionList ']'                                        { $$ = new MatrixLiteral1($2); }
                ;

/* String builder. This accumulates JSON/String data, mostly
character-by-character. */
StringBuilder   : StringBuilder STRBUILD_APPEND                                 { $$ = $1; $$->os << $2; free($2); }
                | StringBuilder STRBUILD_ESCAPE                                 { $$ = $1; $$->os << $2[1]; free($2); }
                |                                                               { $$ = new StringBuilder(); }
                ;

/* String literal. */
StringLiteral   : STRING_OPEN StringBuilder STRING_CLOSE                        { $$ = new StringLiteral($2->os.str()); delete $2; }
                ;

/* JSON literal. */
JsonLiteral     : JSON_OPEN StringBuilder JSON_CLOSE                            { $$ = new JsonLiteral($2->os.str()); delete $2; }
                ;

/* Operands in cQASM can be expressions, strings (for print and error), or
matrices (for the U gate). */
Operand         : Expression %prec BUNDLE                                       { $$ = $1; }
                | MatrixLiteral                                                 { $$ = $1; }
                | StringLiteral                                                 { $$ = $1; }
                | JsonLiteral                                                   { $$ = $1; }
                | Type                                                          { $$ = $1; }
                ;

/* List of operands. */
OperandList     : OperandList ',' Operand                                       { $$ = $1->push_oper($3); }
                | Operand %prec ','                                             { $$ = (new OperandList())->push_oper($1); }
                ;

/* List of identifiers. */
IdentifierList  : IdentifierList ',' IDENTIFIER                                 { $$ = $1->push_id($3); }
                | IDENTIFIER                                                    { $$ = (new IdentifierList())->push_id($1); }
                ;

/* The information caried by an annotation or pragma statement. */
AnnotationData  : IDENTIFIER IDENTIFIER                                         { $$ = new AnnotationData($1, $2); }
                | IDENTIFIER IDENTIFIER ':' OperandList                         { $$ = new AnnotationData($1, $2, $4); }
                ;

/* Name for gates, with optional conditional syntax. */
GateType        : IDENTIFIER                                                    { $$ = new GateType($1); }
                | CDASH GateType Expression ','                                 { $$ = $2->push_cond($3); }
                ;

/* Gate execution. This includes classical instructions. Note that this is
NOT directly a statement grammatically; they are first made part of a bundle.
*/
Gate            : GateType                                                      { $$ = new NormalGate($1); }
                | GateType OperandList                                          { $$ = new NormalGate($1, $2); }
                | GateType OperandList ASSIGN OperandList                       { $$ = new NormalGate($1, $2, $4); }
                | IF Expression GOTO IDENTIFIER                                 { $$ = new IfGoto(new Identifier($4), $2); }
                | GOTO IDENTIFIER                                               { $$ = new IfGoto(new Identifier($2)); }
                ;

/* Gates are not statements but can be annotated, so they need their own
annotation rule. */
AnnotGate       : AnnotGate '@' AnnotationData                                  { $$ = $1->push_annot($3); }
                | Gate                                                          { $$ = $1; }
                ;

/* Single-line bundling syntax. */
SLParGateList   : SLParGateList '|' AnnotGate                                   { $$ = $1->push_gate($3); }
                | AnnotGate %prec '|'                                           { $$ = (new Bundle())->push_gate($1); }
                ;

/* Multi-line bundling syntax. */
CBParGateList   : CBParGateList Newline SLParGateList                           { $$ = $1->push_gates($3->gates); delete $3; }
                | SLParGateList                                                 { $$ = $1; }
                ;

/* Any of the supported statements. */
Statement       : PRAGMA AnnotationData                                         { $$ = new Pragma($2); }
                | Type IDENTIFIER                                               { $$ = new ScalarResource($1, $2); }
                | Type IDENTIFIER '=' Expression                                { $$ = new ScalarResource($1, $2, $4); }
                | Type IDENTIFIER '[' Expression ']'                            { $$ = new ArrayResource($1, $2, $4); }
                | Type IDENTIFIER '[' Expression ']' '=' Expression             { $$ = new ArrayResource($1, $2, $4, $7); }
                | LET IDENTIFIER '=' Expression                                 { $$ = new LetStatement($2, $4); }
                | TYPE_QUBIT NumericLiteral                                     { $$ = new QubitRegister1($2); }
                | MAP Expression ',' IDENTIFIER                                 { $$ = new Mapping($4, $2); }
                | MAP IDENTIFIER ASSIGN Expression                              { $$ = new Mapping($2, $4); }
                | SET Expression '=' Expression                                 { $$ = new Assignment($2, $4); }
                | DEF IDENTIFIER '(' ')' Block                                  { $$ = new MacroDef($2, $5); }
                | DEF IDENTIFIER '(' IdentifierList ')' Block                   { $$ = new MacroDef($2, $6, $4); }
                | DEF IDENTIFIER '(' IdentifierList ASSIGN IdentifierList ')' Block { $$ = new MacroDef($2, $8, $4, $6); }
                | FOR IDENTIFIER '=' '[' IndexList ']' Block                    { $$ = new MacroFor($2, $5, $7); }
                | IF '(' Expression ')' Block                                   { $$ = new MacroIfElse($3, $5); }
                | IF '(' Expression ')' Block ELSE Block                        { $$ = new MacroIfElse($3, $5, $7); }
                | INCLUDE StringLiteral                                         { $$ = new Include($2); }
                | '.' IDENTIFIER                                                { $$ = new Subcircuit($2); }
                | '.' IDENTIFIER '(' NumericLiteral ')'                         { $$ = new Subcircuit($2, $4); }
                | IDENTIFIER ':'                                                { $$ = new Label($1); }
                | SLParGateList                                                 { $$ = $1; }
                | '{' OptNewline CBParGateList OptNewline '}'                   { $$ = $3; }
                | error                                                         { $$ = new ErroneousStatement(); yyrecovered = true; }
                ;

/* Statement with annotations attached to it. */
AnnotStatement  : AnnotStatement '@' AnnotationData                             { $$ = $1->push_annot($3); }
                | Statement                                                     { $$ = $1; }
                ;

/* List of one or more statements. */
StatementList   : StatementList Newline AnnotStatement                          { $$ = $1->push_stmt($3); }
                | AnnotStatement                                                { $$ = (new Block())->push_stmt($1); }
                ;

/* Block of code; zero or more statements. */
Block           : '{' OptNewline StatementList OptNewline '}'                   { $$ = $3; }
                | '{' OptNewline '}'                                            { $$ = new Block(); }
                ;

/* Toplevel. */
Program         : OptNewline VERSION Newline StatementList OptNewline           { $$ = new Program($2, $4);          std::cout << std::string(*$$) << std::endl << "-------------" << std::endl; PrettyPrinter(std::cout).apply(std::shared_ptr<Program>($$)); std::cout << "-------------" << std::endl; }
                | OptNewline VERSION OptNewline                                 { $$ = new Program($2, new Block()); std::cout << std::string(*$$) << std::endl << "-------------" << std::endl; PrettyPrinter(std::cout).apply(std::shared_ptr<Program>($$)); std::cout << "-------------" << std::endl; }
                ;


%%
void yyerror(char const *s) {
    printf("On %d:%d: %s\n", yylloc.first_line, yylloc.first_column, s);
}
