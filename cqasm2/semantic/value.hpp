
#ifndef _LIBCQASM2_AST_EXPRESSION_H_
#define _LIBCQASM2_AST_EXPRESSION_H_

#include <cstdlib>
#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include "cqasm2/semantic/namespace.hpp"

namespace cqasm2 { namespace ast {

    class NumericValue {
    };

    class FixedValue : public NumericValue {
    protected:

        /**
         * The LSB is worth 2^exponent.
         */
        int exponent;

        /**
         * Number of valid bits.
         */
        int bits;

        /**
         * Whether or not this value is signed.
         */
        bool hasSign;

        /**
         * Value.
         */
        union {
            uint64_t udata;
            int64_t idata;
        };

    };

    class FloatingValue : public NumericValue {
    protected:

        /**
         * Whether or not this value is a double.
         */
        bool isDouble;

        /**
         * Value.
         */
        union {
            double ddata;
            float fdata;
        };

    };

    class BooleanValue : public NumericValue {
    protected:

        /**
         * Value.
         */
        bool data;

    };

}}

#endif
