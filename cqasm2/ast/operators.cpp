
#include "operators.hpp"

namespace cqasm2 { namespace ast {

    namespace operators {

        /**
         * Operator description table. NOTE: must be kept in sync with parser.y
         * for corrent printing!
         */
        //                        nops    prec    right_assoc op1     op2     gate
        const Operator NEG    = { 1,      1,      true,       "-",    "",     "neg"   };
        const Operator NOT    = { 1,      1,      true,       "!",    "",     "not"   };
        const Operator INV    = { 1,      1,      true,       "~",    "",     "inv"   };
        const Operator POW    = { 2,      2,      true,       "**",   "",     "pow"   };
        const Operator MUL    = { 2,      3,      false,      "*",    "",     "mul"   };
        const Operator DIV    = { 2,      3,      false,      "/",    "",     "div"   };
        const Operator IDIV   = { 2,      3,      false,      "//",   "",     "idiv"  };
        const Operator MOD    = { 2,      3,      false,      "%",    "",     "mod"   };
        const Operator ADD    = { 2,      4,      false,      "+",    "",     "add"   };
        const Operator SUB    = { 2,      4,      false,      "-",    "",     "sub"   };
        const Operator SHL    = { 2,      5,      false,      "<<",   "",     "shl"   };
        const Operator SHR    = { 2,      5,      false,      ">>",   "",     "shr"   };
        const Operator CGT    = { 2,      6,      false,      ">",    "",     "cgt"   };
        const Operator CLT    = { 2,      6,      false,      "<",    "",     "clt"   };
        const Operator CGE    = { 2,      6,      false,      ">=",   "",     "cge"   };
        const Operator CLE    = { 2,      6,      false,      "<=",   "",     "cle"   };
        const Operator CEQ    = { 2,      7,      false,      "==",   "",     "ceq"   };
        const Operator CNE    = { 2,      7,      false,      "!=",   "",     "cne"   };
        const Operator AND    = { 2,      8,      false,      "&",    "",     "and"   };
        const Operator XOR    = { 2,      9,      false,      "^",    "",     "xor"   };
        const Operator OR     = { 2,      10,     false,      "|",    "",     "or"    };
        const Operator LAND   = { 2,      11,     false,      "&&",   "",     "land"  };
        const Operator LXOR   = { 2,      12,     false,      "^^",   "",     "lxor"  };
        const Operator LOR    = { 2,      13,     false,      "||",   "",     "lor"   };
        const Operator SLCT   = { 3,      14,     true,       "?",    ":",    "slct"  };
    }

    /**
     * Returns the gate name.
     * @return The gate name.
     */
    Operator::operator std::string() const {
        return std::string(this->gate);
    }

    /**
     * Checks equality between two operators.
     */
    bool Operator::operator==(const Operator &x) const {
        return this->gate == x.gate;
    }

}}
