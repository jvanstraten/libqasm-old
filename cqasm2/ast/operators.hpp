
#ifndef _LIBCQASM2_AST_OPERATORS_H_
#define _LIBCQASM2_AST_OPERATORS_H_

#include <cstdlib>
#include <cstdint>
#include <string>

namespace cqasm2 { namespace ast {

    /**
     * Operator description.
     */
    struct Operator {

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

        /**
         * Checks equality between two operators.
         */
        bool operator==(const Operator &x) const;

    };

    /**
     * Operators.
     */
    namespace operators {
        extern const Operator NEG;
        extern const Operator NOT;
        extern const Operator INV;
        extern const Operator POW;
        extern const Operator MUL;
        extern const Operator DIV;
        extern const Operator IDIV;
        extern const Operator MOD;
        extern const Operator ADD;
        extern const Operator SUB;
        extern const Operator SHL;
        extern const Operator SHR;
        extern const Operator CGT;
        extern const Operator CLT;
        extern const Operator CGE;
        extern const Operator CLE;
        extern const Operator CEQ;
        extern const Operator CNE;
        extern const Operator AND;
        extern const Operator XOR;
        extern const Operator OR;
        extern const Operator LAND;
        extern const Operator LXOR;
        extern const Operator LOR;
        extern const Operator SLCT;
    }

}}

#endif
