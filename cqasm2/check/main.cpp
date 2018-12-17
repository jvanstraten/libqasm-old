
#include <cqasm2/cqasm2.hpp>
#include "parser.hpp"
#include <stdio.h>

extern FILE *yyin;
extern bool yyrecovered;

int main(int argc, const char **argv) {

    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror("failed to open input file");
            exit(2);
        }
        yyrecovered = false;
        int ret = yyparse();
        if (ret || yyrecovered) {
            exit(1);
        } else {
            printf("The specified file is valid cQASM 2.0!\n");
        }
        fclose(yyin);
        exit(0);
    } else {
        printf("Usage: %s <cqasm-file>\n", argv[0]);
        printf("Exit codes:\n");
        printf("  0: file is proper cQASM 2.0.\n");
        printf("  1: file has errors.\n");
        printf("  2: file could not be opened.\n");
        exit(2);
    }
    return 0;
}
