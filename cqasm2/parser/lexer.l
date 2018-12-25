%option yylineno
%option nounput

%{
    #include "cqasm2/ast/ast.hpp"
    #include "parser.hpp"
    #define YY_USER_ACTION yylloc.last_column += yyleng;
    #define LOCATION_RESET \
        yylloc.first_column = 1; yylloc.first_line = 1; \
        yylloc.last_column = 1; yylloc.last_line = 1;
    #define LOCATION_LINE \
        yylloc.last_column = 1; yylloc.last_line += 1;
    #define LOCATION_STEP \
        yylloc.first_column = yylloc.last_column; \
        yylloc.first_line = yylloc.last_line;
    #define WITH_STR(TOKNAME) \
        /*printf("Pushing %s token starting at %d:%d\n", #TOKNAME, yylloc.first_line, yylloc.first_column);*/ \
        yylval.str = strdup(yytext); return TOKNAME
    #define WITHOUT_STR(TOKNAME) \
        /*printf("Pushing %s token starting at %d:%d\n", #TOKNAME, yylloc.first_line, yylloc.first_column);*/ \
        return TOKNAME
    #define OPERATOR \
        /*printf("Pushing %s token starting at %d:%d\n", yytext, yylloc.first_line, yylloc.first_column);*/ \
        return yytext[0]
    #define DELETE LOCATION_STEP
%}

%x MULTILINE_COMMENT
%x STRING
%x JSON
%x JSONSTRING
%%

%{
    LOCATION_STEP;
%}

<INITIAL>{

    /*=======================================================================*/
    /* WHITESPACE MANAGEMENT                                                 */
    /*=======================================================================*/

    /* Newline and single-line comment */
(\n|\r\n?)                                          LOCATION_LINE; WITHOUT_STR(NEWLINE);

    /* Semicolons function exactly the same way as newlines */
;                                                   WITHOUT_STR(NEWLINE);

    /* And then there's the opposite, the escaped newline */
\\(\n|\r\n?)                                        LOCATION_LINE; DELETE;

    /* Gobble up comments */
    /* TODO: error on EOF within comment mode */
#.*                                                 DELETE;
"/*"                                                BEGIN(MULTILINE_COMMENT); DELETE;
}
<MULTILINE_COMMENT>{
"*/"                                                BEGIN(INITIAL); DELETE;
(\n|\r\n?)                                          LOCATION_LINE; DELETE;
.                                                   DELETE;
}
<INITIAL>{


    /* Gobble up whitespace */
[ \t]+                                              DELETE;


    /*=======================================================================*/
    /* VERSION STATEMENT                                                     */
    /*=======================================================================*/

    /* This is treated separately so there is A) no need to clutter up the
    reserved word space and B) so we don't need the float token to scan the
    version number */
(?i:version)[ \t]+[0-9]+\.[0-9]+                    WITH_STR(VERSION);

    /*=======================================================================*/
    /* KEYWORDS                                                              */
    /*=======================================================================*/

    /* Resource type keywords */
(?i:qubits?)                                        WITHOUT_STR(TYPE_QUBIT);
(?i:boolean)                                        WITHOUT_STR(TYPE_BOOLEAN);
(?i:int)                                            WITHOUT_STR(TYPE_INT);
(?i:uint)                                           WITHOUT_STR(TYPE_UINT);
(?i:fixed)                                          WITHOUT_STR(TYPE_FIXED);
(?i:ufixed)                                         WITHOUT_STR(TYPE_UFIXED);
(?i:float)                                          WITHOUT_STR(TYPE_FLOAT);
(?i:double)                                         WITHOUT_STR(TYPE_DOUBLE);

    /* High-level keywords */
(?i:map)                                            WITHOUT_STR(MAP);
(?i:let)                                            WITHOUT_STR(LET);
(?i:set)                                            WITHOUT_STR(SET);
(?i:def)                                            WITHOUT_STR(DEF);
(?i:if)                                             WITHOUT_STR(IF);
(?i:else)                                           WITHOUT_STR(ELSE);
(?i:for)                                            WITHOUT_STR(FOR);
(?i:goto)                                           WITHOUT_STR(GOTO);
(?i:include)                                        WITHOUT_STR(INCLUDE);

    /* Pragma statement  */
(?i:pragma)                                         WITHOUT_STR(PRAGMA);

    /* Unused reserved words */
(?i:complex)                                        WITHOUT_STR(BAD_RESERVED);
(?i:string)                                         WITHOUT_STR(BAD_RESERVED);
(?i:vector)                                         WITHOUT_STR(BAD_RESERVED);
(?i:matrix)                                         WITHOUT_STR(BAD_RESERVED);
(?i:struct)                                         WITHOUT_STR(BAD_RESERVED);
(?i:type)                                           WITHOUT_STR(BAD_RESERVED);
(?i:const)                                          WITHOUT_STR(BAD_RESERVED);
(?i:extern)                                         WITHOUT_STR(BAD_RESERVED);
(?i:volatile)                                       WITHOUT_STR(BAD_RESERVED);
(?i:weak)                                           WITHOUT_STR(BAD_RESERVED);
(?i:gate)                                           WITHOUT_STR(BAD_RESERVED);

    /* This one is ugly... but needs to be here for 1.0 compat. If only 1.0
    used "c_" insted of "c-"... */
[cC]-/[a-zA-Z_]                                     WITHOUT_STR(CDASH);


    /*=======================================================================*/
    /* LITERALS                                                              */
    /*=======================================================================*/

    /* Integer literals */
[0-9]+                                              WITH_STR(LIT_INT_DEC);
0[xX][0-9a-fA-F]+                                   WITH_STR(LIT_INT_HEX);
0[bB][01]+                                          WITH_STR(LIT_INT_BIN);
[0-9]+[uU]                                          WITH_STR(LIT_UINT_DEC);
0[xX][0-9a-fA-F]+[uU]                               WITH_STR(LIT_UINT_HEX);
0[bB][01]+[uU]                                      WITH_STR(LIT_UINT_BIN);

    /* Float literals */
[0-9]*\.[0-9]+([eE][-+]?[0-9]+)?[fF]                WITH_STR(LIT_FLOAT);
[0-9]*\.[0-9]+([eE][-+]?[0-9]+)?                    WITH_STR(LIT_DOUBLE);

    /* Fixed literals */
0[xX][0-9a-fA-F]*_*\._*[0-9a-fA-F]*                 WITH_STR(LIT_FIXED_HEX);
0[bB][01]*_*\._*[01]*                               WITH_STR(LIT_FIXED_BIN);
0[xX][0-9a-fA-F]*_*\._*[0-9a-fA-F]*[uU]             WITH_STR(LIT_UFIXED_HEX);
0[bB][01]*_*\._*[01]*[uU]                           WITH_STR(LIT_UFIXED_BIN);

    /* The same with letters following, to detect errors */
[0-9]+[uU]?[a-zA-Z_]                                WITHOUT_STR(BAD_NUMBER);
0[xX][0-9a-fA-F]+[uU]?[a-zA-Z_]                     WITHOUT_STR(BAD_NUMBER);
0[bB][01]+[uU]?[a-zA-Z_]                            WITHOUT_STR(BAD_NUMBER);
[0-9]*\.[0-9]+([eE][-+]?[0-9]+)?[fF]?[a-zA-Z_]      WITHOUT_STR(BAD_NUMBER);
0[xX][0-9a-fA-F]*_*\._*[0-9a-fA-F]*[uU]?[a-zA-Z_]   WITHOUT_STR(BAD_NUMBER);
0[bB][01]*_*\._*[01]*[uU]?[a-zA-Z_]                 WITHOUT_STR(BAD_NUMBER);

    /* Named constant keywords */
(?i:true|false|pi|eu|im)                            WITH_STR(LIT_NAMED);

    /* String literals */
["]                                                 BEGIN(STRING); WITHOUT_STR(STRING_OPEN);
}
<STRING>{
["]                                                 BEGIN(INITIAL); WITHOUT_STR(STRING_CLOSE);
[\\][tn'"\\]                                        WITH_STR(STRBUILD_ESCAPE);
[\\](\n|\r\n?)                                      LOCATION_LINE; DELETE;
(\n|\r\n?)                                          LOCATION_LINE; WITH_STR(STRBUILD_APPEND);
.                                                   WITH_STR(STRBUILD_APPEND);
}
<INITIAL>{

    /* JSON literals */
"{|"                                                BEGIN(JSON); WITHOUT_STR(JSON_OPEN);
}
<JSON>{
"|}"                                                BEGIN(INITIAL); WITHOUT_STR(JSON_CLOSE);
["]                                                 BEGIN(JSONSTRING); WITH_STR(STRBUILD_APPEND);
(\n|\r\n?)                                          LOCATION_LINE; WITH_STR(STRBUILD_APPEND);
.                                                   WITH_STR(STRBUILD_APPEND);
}
<JSONSTRING>{
["]                                                 BEGIN(JSON); WITH_STR(STRBUILD_APPEND);
[\\].                                               WITH_STR(STRBUILD_APPEND);
(\n|\r\n?)                                          LOCATION_LINE; WITH_STR(STRBUILD_APPEND);
.                                                   WITH_STR(STRBUILD_APPEND);
}
<INITIAL>{

    /* Identifiers */
[a-zA-Z_][a-zA-Z0-9_]*                              WITH_STR(IDENTIFIER);


    /*=======================================================================*/
    /* OPERATORS                                                             */
    /*=======================================================================*/

[()[\]{}<>=!~|&^+\-*/%?:,.@]                        OPERATOR;
"[|"                                                WITHOUT_STR(MATRIX_OPEN);
"|]"                                                WITHOUT_STR(MATRIX_CLOSE);
"=="                                                WITHOUT_STR(CMP_EQ);
"!="                                                WITHOUT_STR(CMP_NE);
"<="                                                WITHOUT_STR(CMP_LE);
">="                                                WITHOUT_STR(CMP_GE);
"||"                                                WITHOUT_STR(LOGICAL_OR);
"^^"                                                WITHOUT_STR(LOGICAL_XOR);
"&&"                                                WITHOUT_STR(LOGICAL_AND);
"<<"                                                WITHOUT_STR(SHIFT_LEFT);
">>"                                                WITHOUT_STR(SHIFT_RIGHT);
"//"                                                WITHOUT_STR(DIV_INT);
"**"                                                WITHOUT_STR(POWER);
"->"                                                WITHOUT_STR(ASSIGN);

    /*=======================================================================*/
    /* DEFAULT                                                               */
    /*=======================================================================*/

    /* Catch any other character as a single-character lexeme */
.                                                   WITHOUT_STR(BAD_CHARACTER);


}


%%

int yywrap(void) {
    return 1;
}

