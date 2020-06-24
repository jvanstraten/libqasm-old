#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include "qasm_semantic.hpp"
#include "doctest/doctest.h"

TEST_CASE("Test for the reentrant_invalid.qasm file")
{
    #if YYDEBUG == 1
    extern int yydebug;
    yydebug = 1;
    #endif

    // open a file handle to a particular file:
    FILE *myfile = fopen("reentrant_invalid.qasm", "r");

    CHECK_THROWS_WITH(compiler::QasmSemanticChecker sm(myfile), "Error at <unknown>:6:14..15: index 2 out of range (size 2)");

    rewind(myfile);

    CHECK_THROWS_WITH(compiler::QasmSemanticChecker sm2(myfile), "Error at <unknown>:6:14..15: index 2 out of range (size 2)");
}
