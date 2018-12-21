
#include "operators.hpp"

namespace cqasm2 { namespace ast {

    namespace operators {

        /**
         * Operator description table. NOTE: must be kept in sync with parser.y
         * for corrent printing!
         */
        //                          nops    prec    right_assoc op1     op2     gate
        const operator_t NEG    = { 1,      1,      true,       "-",    "",     "neg"   };
        const operator_t NOT    = { 1,      1,      true,       "!",    "",     "not"   };
        const operator_t INV    = { 1,      1,      true,       "~",    "",     "inv"   };
        const operator_t POW    = { 2,      2,      true,       "**",   "",     "pow"   };
        const operator_t MUL    = { 2,      3,      false,      "*",    "",     "mul"   };
        const operator_t DIV    = { 2,      3,      false,      "/",    "",     "div"   };
        const operator_t IDIV   = { 2,      3,      false,      "//",   "",     "idiv"  };
        const operator_t MOD    = { 2,      3,      false,      "%",    "",     "mod"   };
        const operator_t ADD    = { 2,      4,      false,      "+",    "",     "add"   };
        const operator_t SUB    = { 2,      4,      false,      "-",    "",     "sub"   };
        const operator_t SHL    = { 2,      5,      false,      "<<",   "",     "shl"   };
        const operator_t SHR    = { 2,      5,      false,      ">>",   "",     "shr"   };
        const operator_t CGT    = { 2,      6,      false,      ">",    "",     "cgt"   };
        const operator_t CLT    = { 2,      6,      false,      "<",    "",     "clt"   };
        const operator_t CGE    = { 2,      6,      false,      ">=",   "",     "cge"   };
        const operator_t CLE    = { 2,      6,      false,      "<=",   "",     "cle"   };
        const operator_t CEQ    = { 2,      7,      false,      "==",   "",     "ceq"   };
        const operator_t CNE    = { 2,      7,      false,      "!=",   "",     "cne"   };
        const operator_t AND    = { 2,      8,      false,      "&",    "",     "and"   };
        const operator_t XOR    = { 2,      9,      false,      "^",    "",     "xor"   };
        const operator_t OR     = { 2,      10,     false,      "|",    "",     "or"    };
        const operator_t LAND   = { 2,      11,     false,      "&&",   "",     "land"  };
        const operator_t LXOR   = { 2,      12,     false,      "^^",   "",     "lxor"  };
        const operator_t LOR    = { 2,      13,     false,      "||",   "",     "lor"   };
        const operator_t SLCT   = { 3,      14,     true,       "?",    ":",    "slct"  };
    }

    /**
     * Returns the gate name.
     * @return The gate name.
     */
    operator_t::operator std::string() const {
        return std::string(this->gate);
    }
}}
