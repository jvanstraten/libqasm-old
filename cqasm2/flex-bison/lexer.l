%option yylineno
%{
    #include "cqasm2/ast/node.hpp"
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
    #define TOKEN(TOKNAME) \
        /*printf("Pushing %s token starting at %d:%d\n", #TOKNAME, yylloc.first_line, yylloc.first_column);*/ \
        yylval.tok = strdup(yytext); return TOKNAME
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
(\n|\r\n?)                                          LOCATION_LINE; TOKEN(NEWLINE);

    /* Semicolons function exactly the same way as newlines */
;                                                   TOKEN(NEWLINE);

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
(?i:version)[ \t]+[0-9]+\.[0-9]+                    TOKEN(VERSION);

    /*=======================================================================*/
    /* KEYWORDS                                                              */
    /*=======================================================================*/

    /* Resource type keywords */
(?i:qubits?)                                        TOKEN(TYPE_QUBIT);
(?i:boolean)                                        TOKEN(TYPE_BOOLEAN);
(?i:int)                                            TOKEN(TYPE_INT);
(?i:uint)                                           TOKEN(TYPE_UINT);
(?i:fixed)                                          TOKEN(TYPE_FIXED);
(?i:ufixed)                                         TOKEN(TYPE_UFIXED);
(?i:float)                                          TOKEN(TYPE_FLOAT);
(?i:double)                                         TOKEN(TYPE_DOUBLE);

    /* High-level keywords */
(?i:def)                                            TOKEN(DEF);
(?i:if)                                             TOKEN(IF);
(?i:goto)                                           TOKEN(GOTO);
(?i:else)                                           TOKEN(ELSE);
(?i:for)                                            TOKEN(FOR);
(?i:include)                                        TOKEN(INCLUDE);

    /* Misc. keywords */
(?i:map)                                            TOKEN(MAP);
(?i:pragma)                                         TOKEN(PRAGMA);
(?i:gate)                                           TOKEN(GATE);

    /* Unused reserved words */
(?i:complex)                                        TOKEN(BAD_RESERVED);
(?i:string)                                         TOKEN(BAD_RESERVED);
(?i:vector)                                         TOKEN(BAD_RESERVED);
(?i:matrix)                                         TOKEN(BAD_RESERVED);
(?i:struct)                                         TOKEN(BAD_RESERVED);
(?i:type)                                           TOKEN(BAD_RESERVED);
(?i:const)                                          TOKEN(BAD_RESERVED);
(?i:extern)                                         TOKEN(BAD_RESERVED);
(?i:volatile)                                       TOKEN(BAD_RESERVED);
(?i:weak)                                           TOKEN(BAD_RESERVED);

    /* This one is ugly... but needs to be here for 1.0 compat. If only 1.0
    used "c_" insted of "c-"... */
[cC]-/[a-zA-Z_]                                     TOKEN(CDASH);


    /*=======================================================================*/
    /* LITERALS                                                              */
    /*=======================================================================*/

    /* Integer literals */
[0-9]+                                              TOKEN(LIT_INT_DEC);
0[xX][0-9a-fA-F]+                                   TOKEN(LIT_INT_HEX);
0[bB][01]+                                          TOKEN(LIT_INT_BIN);
[0-9]+[uU]                                          TOKEN(LIT_UINT_DEC);
0[xX][0-9a-fA-F]+[uU]                               TOKEN(LIT_UINT_HEX);
0[bB][01]+[uU]                                      TOKEN(LIT_UINT_BIN);

    /* Float literals */
[0-9]*\.[0-9]+([eE][-+]?[0-9]+)?[fF]                TOKEN(LIT_FLOAT);
[0-9]*\.[0-9]+([eE][-+]?[0-9]+)?                    TOKEN(LIT_DOUBLE);

    /* Fixed literals */
0[xX][0-9a-fA-F]*\.[0-9a-fA-F]+                     TOKEN(LIT_FIXED_HEX);
0[bB][01]*\.[01]+                                   TOKEN(LIT_FIXED_BIN);
0[xX][0-9a-fA-F]*\.[0-9a-fA-F]+[uU]                 TOKEN(LIT_UFIXED_HEX);
0[bB][01]*\.[01]+[uU]                               TOKEN(LIT_UFIXED_BIN);

    /* The same with letters following, to detect errors */
[0-9]+[uU]?[a-zA-Z_]                                TOKEN(BAD_NUMBER);
0[xX][0-9a-fA-F]+[uU]?[a-zA-Z_]                     TOKEN(BAD_NUMBER);
0[bB][01]+[uU]?[a-zA-Z_]                            TOKEN(BAD_NUMBER);
[0-9]*\.[0-9]+([eE][-+]?[0-9]+)?[fF]?[a-zA-Z_]      TOKEN(BAD_NUMBER);
0[xX][0-9a-fA-F]*\.[0-9a-fA-F]+[uU]?[a-zA-Z_]       TOKEN(BAD_NUMBER);
0[bB][01]*\.[01]+[uU]?[a-zA-Z_]                     TOKEN(BAD_NUMBER);

    /* Boolean literal keywords */
(?i:false)                                          TOKEN(LIT_BOOLEAN);
(?i:true)                                           TOKEN(LIT_BOOLEAN);

    /* Mathematical constant keywords */
(?i:pi)                                             TOKEN(LIT_PI);
(?i:eu)                                             TOKEN(LIT_EU);
(?i:im)                                             TOKEN(LIT_IM);

    /* String literals */
["]                                                 BEGIN(STRING); TOKEN(STRING_OPEN);
}
<STRING>{
["]                                                 BEGIN(INITIAL); TOKEN(STRING_CLOSE);
[\\][tn'"\\]                                        TOKEN(APPEND_ESCAPE);
[\\](\n|\r\n?)                                      LOCATION_LINE; DELETE;
(\n|\r\n?)                                          LOCATION_LINE; TOKEN(APPEND);
.                                                   TOKEN(APPEND);
}
<INITIAL>{

    /* JSON literals */
"{|"                                                BEGIN(JSON); TOKEN(JSON_OPEN);
}
<JSON>{
"|}"                                                BEGIN(INITIAL); TOKEN(JSON_CLOSE);
["]                                                 BEGIN(JSONSTRING); TOKEN(APPEND);
(\n|\r\n?)                                          LOCATION_LINE; TOKEN(APPEND);
.                                                   TOKEN(APPEND);
}
<JSONSTRING>{
["]                                                 BEGIN(JSON); TOKEN(APPEND);
[\\].                                               TOKEN(APPEND);
(\n|\r\n?)                                          LOCATION_LINE; TOKEN(APPEND);
.                                                   TOKEN(APPEND);
}
<INITIAL>{

    /* Identifiers */
[a-zA-Z_][a-zA-Z0-9_]*                              TOKEN(IDENTIFIER);


    /*=======================================================================*/
    /* OPERATORS                                                             */
    /*=======================================================================*/

[()[\]{}<>=!~|&^+\-*/%?:,.@]                        OPERATOR;
"[|"                                                TOKEN(MATRIX_OPEN);
"|]"                                                TOKEN(MATRIX_CLOSE);
"=="                                                TOKEN(CMP_EQ);
"!="                                                TOKEN(CMP_NE);
"<="                                                TOKEN(CMP_LE);
">="                                                TOKEN(CMP_GE);
"||"                                                TOKEN(LOGICAL_OR);
"&&"                                                TOKEN(LOGICAL_AND);
"^^"                                                TOKEN(LOGICAL_XOR);
"<<"                                                TOKEN(SHIFT_LEFT);
">>"                                                TOKEN(SHIFT_RIGHT);
">>>"                                               TOKEN(LOG_SHIFT_RIGHT);
"//"                                                TOKEN(DIV_INT);
"**"                                                TOKEN(POWER);
"->"                                                TOKEN(ASSIGN);

    /*=======================================================================*/
    /* DEFAULT                                                               */
    /*=======================================================================*/

    /* Catch any other character as a single-character lexeme */
.                                                   TOKEN(BAD_CHARACTER);


}


%%

int yywrap(void) {
    return 1;
}

