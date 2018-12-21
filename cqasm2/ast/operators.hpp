
#ifndef _LIBCQASM2_AST_OPERATORS_H_
#define _LIBCQASM2_AST_OPERATORS_H_

#include <cstdlib>
#include <cstdint>
#include <string>

namespace cqasm2 { namespace ast {

    /**
     * Operator description.
     */
    typedef struct {

        /**
         * Number of operands for inline operators.
         */
        int nops;

        /**
         * Precedence level.
         */
        int prec;

        /**
         * Right-associative instead of left-associative.
         */
        bool right_assoc;

        /**
         * Prefix or first infix operator.
         */
        const char *op1;

        /**
         * Second infix operator for ternaries.
         */
        const char *op2;

        /**
         * Name of function and/or accompanying gate.
         */
        const char *gate;

        /**
         * Returns the gate name.
         * @return The gate name.
         */
        operator std::string() const;

    } operator_t;

    /**
     * Operators.
     */
    namespace operators {
        extern const operator_t NEG;
        extern const operator_t NOT;
        extern const operator_t INV;
        extern const operator_t POW;
        extern const operator_t MUL;
        extern const operator_t DIV;
        extern const operator_t IDIV;
        extern const operator_t MOD;
        extern const operator_t ADD;
        extern const operator_t SUB;
        extern const operator_t SHL;
        extern const operator_t SHR;
        extern const operator_t CGT;
        extern const operator_t CLT;
        extern const operator_t CGE;
        extern const operator_t CLE;
        extern const operator_t CEQ;
        extern const operator_t CNE;
        extern const operator_t AND;
        extern const operator_t XOR;
        extern const operator_t OR;
        extern const operator_t LAND;
        extern const operator_t LXOR;
        extern const operator_t LOR;
        extern const operator_t SLCT;
    }

}}

#endif
