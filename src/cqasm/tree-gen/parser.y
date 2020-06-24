%define api.pure full
%locations

%code requires {
    #include <memory>
    #include <cstdio>
    #include <cstdint>
    #include <cstring>
    #include <iostream>
    #include "tree-gen.hpp"
    typedef void* yyscan_t;
}

%code {
    int yylex(YYSTYPE* yylvalp, YYLTYPE* yyllocp, yyscan_t scanner);
    void yyerror(YYLTYPE* yyllocp, yyscan_t scanner, Specification &specification, const char* msg);
}

%code top {
    #define TRY try {
    #define CATCH } catch (std::exception &e) { yyerror(&yyloc, scanner, specification, e.what()); }

    #include <algorithm>
    #include <cctype>
    #include <locale>

    // trim from start (in place)
    static inline void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
            return !std::isspace(ch);
        }));
    }

    // trim from end (in place)
    static inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    // trim from both ends (in place)
    static inline std::string trim(const std::string &s) {
        auto copy = std::string(s);
        ltrim(copy);
        rtrim(copy);
        return copy;
    }

}

%param { yyscan_t scanner }
%parse-param { Specification &specification }

/* YYSTYPE union */
%union {
    char           	*str;
    std::string     *xstr;
    NodeBuilder     *nbld;
};

/* Typenames for nonterminals */
%type <xstr> Documentation Identifier String
%type <nbld> Node

/* Tokens */
%token <str> DOCSTRING
%token <str> INCLUDE SRC_INCLUDE
%token SOURCE HEADER TREE_NS SOURCE_LOC
%token NAMESPACE NAMESPACE_SEP
%token ERROR
%token MAYBE ONE ANY MANY EXT
%token <str> IDENT
%token <str> STRING
%token '{' '}' '<' '>' ':' ';'
%token BAD_CHARACTER

/* Misc. Yacc directives */
%error-verbose
%start Root

%%

Documentation   :                                                               { TRY $$ = new std::string(); CATCH }
                | Documentation DOCSTRING                                       { TRY $$ = $1; if (!$$->empty()) *$$ += " "; *$$ += trim(std::string($2 + 1)); std::free($2); CATCH }
                ;

Identifier      : IDENT                                                         { TRY $$ = new std::string($1); std::free($1); CATCH }
                | Identifier NAMESPACE_SEP IDENT                                { TRY $$ = $1; *$$ += "::"; *$$ += $3; std::free($3); CATCH }
                ;

String          : STRING                                                        { TRY $1[std::strlen($1) - 1] = 0; $$ = new std::string($1 + 1); std::free($1); CATCH }
                ;

Node            : Documentation IDENT '{'                                       { TRY auto nb = std::make_shared<NodeBuilder>(std::string($2), *$1); specification.add_node(nb); $$ = nb.get(); delete $1; std::free($2); CATCH }
                | Node ERROR ';'                                                { TRY $$ = $1->mark_error(); CATCH }
                | Node Documentation IDENT ':' MAYBE '<' Identifier '>' ';'     { TRY $$ = $1->with_child(Maybe, *$7, std::string($3), *$2); delete $2; std::free($3); delete $7; CATCH }
                | Node Documentation IDENT ':' ONE '<' Identifier '>' ';'       { TRY $$ = $1->with_child(One, *$7, std::string($3), *$2); delete $2; std::free($3); delete $7; CATCH }
                | Node Documentation IDENT ':' ANY '<' Identifier '>' ';'       { TRY $$ = $1->with_child(Any, *$7, std::string($3), *$2); delete $2; std::free($3); delete $7; CATCH }
                | Node Documentation IDENT ':' MANY '<' Identifier '>' ';'      { TRY $$ = $1->with_child(Many, *$7, std::string($3), *$2); delete $2; std::free($3); delete $7; CATCH }
                | Node Documentation IDENT ':' EXT MAYBE '<' Identifier '>' ';' { TRY $$ = $1->with_prim(*$8, std::string($3), *$2, Maybe); delete $2; std::free($3); delete $8; CATCH }
                | Node Documentation IDENT ':' EXT ONE '<' Identifier '>' ';'   { TRY $$ = $1->with_prim(*$8, std::string($3), *$2, One); delete $2; std::free($3); delete $8; CATCH }
                | Node Documentation IDENT ':' EXT ANY '<' Identifier '>' ';'   { TRY $$ = $1->with_prim(*$8, std::string($3), *$2, Any); delete $2; std::free($3); delete $8; CATCH }
                | Node Documentation IDENT ':' EXT MANY '<' Identifier '>' ';'  { TRY $$ = $1->with_prim(*$8, std::string($3), *$2, Many); delete $2; std::free($3); delete $8; CATCH }
                | Node Documentation IDENT ':' Identifier ';'                   { TRY $$ = $1->with_prim(*$5, std::string($3), *$2, Prim); delete $2; std::free($3); delete $5; CATCH }
                | Node Node '}'                                                 { TRY $2->derive_from($1->node); CATCH }
                ;

Root            :                                                               {}
                | Root Documentation SOURCE String                              { TRY specification.set_source(*$4); delete $2; delete $4; CATCH }
                | Root Documentation HEADER String                              { TRY specification.set_header(*$4); delete $2; delete $4; CATCH }
                | Root Documentation TREE_NS Identifier                         { TRY specification.set_tree_namespace(*$4); delete $2; delete $4; CATCH }
                | Root Documentation SOURCE_LOC Identifier                      { TRY specification.set_source_location(*$4); delete $2; delete $4; CATCH }
                | Root Documentation INCLUDE                                    { TRY specification.add_include(std::string($3)); delete $2; std::free($3); CATCH }
                | Root Documentation SRC_INCLUDE                                { TRY specification.add_src_include(std::string($3 + 4)); delete $2; std::free($3); CATCH }
                | Root Documentation NAMESPACE IDENT                            { TRY specification.add_namespace(std::string($4)); delete $2; std::free($4); CATCH }
                | Root Node '}'                                                 {}
                ;

%%

void yyerror(YYLTYPE* yyllocp, yyscan_t unused, Specification &specification, const char* msg) {
    (void)unused;
    (void)specification;
    std::cerr << "Parse error at " << ":"
              << yyllocp->first_line << ":" << yyllocp->first_column << ".."
              << yyllocp->last_line << ":" << yyllocp->last_column
              << ": " << msg;
}
