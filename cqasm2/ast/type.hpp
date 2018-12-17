
#ifndef _LIBCQASM2_AST_TYPE_H_
#define _LIBCQASM2_AST_TYPE_H_

#include <cstdlib>
#include <cstdint>
#include <string>
#include <memory>
#include <list>
#include "node.hpp"

namespace cqasm2 { namespace ast {

    // Forward declarations.
    class Expression;
    class Node;

    /**
     * Abstract superclass for all Type AST nodes.
     */
    class Type : public Node {
    public:

        /**
         * Default destructor.
         */
        virtual ~Type() = default;

        /**
         * Returns whether an array of this type can be indexed dynamically.
         */
        virtual bool canIndexDynamically() const;

        /**
         * Returns whether this type can be subscripted by the given subscript.
         */
        virtual bool canSubscript(const std::string subscript) const;

        /**
         * Returns the type of the given subscript.
         */
        virtual std::shared_ptr<Type> getSubscriptType(const std::string subscript) const;

        /**
         * Returns whether this is a regular classical datatype.
         */
        virtual bool isClassical() const;

        /**
         * Returns whether this is/can be a measurement register.
         */
        virtual bool isMeasurement() const;

        /**
         * Returns whether this is/can be a quantum datatype.
         */
        virtual bool isQuantum() const;

    };

    /**
     * Qubit type, represents a single qubit and an accompanying boolean
     * measurement register.
     */
    class QubitType : public Type {
    protected:

        /**
         * Indicates that this type has been disambiguated to a qubit using
         * subscript notation.
         */
        bool disambiguatedToQubit;

        /**
         * Indicates that this type has been disambiguated to a measurement
         * register using subscript notation.
         */
        bool disambiguatedToMeasurement;

    public:

        /**
         * Constructor.
         */
        QubitType();

        /**
         * Default destructor.
         */
        virtual ~QubitType() = default;

        /**
         * Returns that a qubit cannot be dynamically indexed.
         */
        virtual bool canIndexDynamically() const override;

        /**
         * Returns that this type can be disambiguated.
         */
        virtual bool canSubscript(const std::string subscript) const override;

        /**
         * Returns the type of the given subscript.
         */
        virtual std::shared_ptr<Type> getSubscriptType(const std::string subscript) const override;

        /**
         * Returns that this isn't a classical type.
         */
        virtual bool isClassical() const override;

        /**
         * Returns whether this can be used as a measurement register.
         */
        virtual bool isMeasurement() const override;

        /**
         * Returns whether this can be used as a qubit.
         */
        virtual bool isQuantum() const override;

        /**
         * Resolves all references in this node tree, and gives all
         * declarations a unique name.
         */
        virtual void resolve(Namespace &names) override;

        /**
         * Performs macro expansion, constant folding, type checking, etc.
         */
        virtual std::shared_ptr<Node> elaborate() override;

        /**
         * Return the name of this type in cQASM syntax (if there is such a
         * representation).
         */
        virtual operator std::string() const override;

    };

    /**
     * Integer type, either signed or unsigned, using expressions for sizing.
     */
    class FixedType : public Type {
    protected:

        /**
         * Indicates whether this type is signed or unsigned.
         */
        bool sign;

        /**
         * Indicates whether this type is fixed or integral.
         */
        bool fixed;

        /**
         * The expression representing the number of integer bits.
         */
        std::shared_ptr<Expression> intBits;

        /**
         * The expression representing the number of fractional bits.
         */
        std::shared_ptr<Expression> fracBits;

    public:

        /**
         * Constructor for int/uint.
         */
        FixedType(bool sign, std::shared_ptr<Expression> bitcount);

        /**
         * Constructor for fixed/ufixed.
         */
        FixedType(bool sign, std::shared_ptr<Expression> intBits, std::shared_ptr<Expression> fracBits);

        /**
         * Default destructor.
         */
        virtual ~FixedType() = default;

        /**
         * Return the number of integer bits as an expression.
         */
        std::shared_ptr<Expression> getIntBitsExp() const;

        /**
         * Return the number of fractional bits as an expression. This returns
         * null if this is an int or uint type.
         */
        std::shared_ptr<Expression> getFracBitsExp() const;

        /**
         * Return whether this integer type is fixed-point.
         */
        bool isFixed() const;

        /**
         * Return whether this integer type is signed.
         */
        bool isSigned() const;

        /**
         * Resolves all references in this node tree, and gives all
         * declarations a unique name.
         */
        virtual void resolve(Namespace &names) override;

        /**
         * Performs macro expansion, constant folding, type checking, etc.
         */
        virtual std::shared_ptr<Node> elaborate() override;

        /**
         * Return the name of this type in cQASM syntax (if there is such a
         * representation).
         */
        virtual operator std::string() const override;

    };


}}

#endif
