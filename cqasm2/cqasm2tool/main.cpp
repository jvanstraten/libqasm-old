
#include <cqasm2/cqasm2.hpp>
#include "parser.hpp"
#include <stdio.h>

extern FILE *yyin;
extern bool yyrecovered;

int main(int argc, const char **argv) {

    if (argc > 1) {
        auto p = parse(std::string(argv[1]), &std::cerr);
        if (p) {
            std::cout << std::string(*p) << std::endl;
            std::cout << "=================" << std::endl;
            PrettyPrinter(std::cout).apply(p);
        }
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
