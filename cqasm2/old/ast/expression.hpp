
#ifndef _LIBCQASM2_AST_EXPRESSION_H_
#define _LIBCQASM2_AST_EXPRESSION_H_

#include <cstdlib>
#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include "cqasm2/semantic/namespace.hpp"

namespace cqasm2 { namespace ast {

    /**
     * Represents any kind of expression.
     */
    class Expression : public Node { // TODO: Operand instead of Node
    protected:

        /**
         * Type of this expression. This is null for non-elaborated
         * expressions.
         */
        std::shared_ptr<Type> type;

    public:

        /**
         * Default destructor.
         */
        virtual ~Expression() = default;

        /**
         * Perform constant folding, annote types, and semantically check this
         * expression. Returns the newly generated Expression.
         */
        virtual std::shared_ptr<Expression> elaborate() const = 0;

        /**
         * Convenience function that returns the type of this expression. This
         * expression must have been elaborated for this to work.
         */
        virtual std::shared_ptr<Type> getType() const;

        /**
         * Convenience function that returns the value of this expression. This
         * works only if this expression is a Literal, otherwise a non-static
         * error is thrown.
         */
        virtual std::shared_ptr<Value> getValue() const;

        /**
         * Convenience function that returns the value of this expression after
         * casting to the specified type. This works only if this expression is
         * a Literal, otherwise a non-static error is thrown.
         */
        virtual std::shared_ptr<Value> getValueAs(std::shared_ptr<Type> castTo) const;

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
         * Prints this node using cQASM syntax if possible.
         */
        virtual operator std::string() const override;

    };

    /**
     * Represents a list of expressions.
     */
    class ExpressionList : public Node {
    public:

        /**
         * List of expressions.
         */
        std::vector<std::shared_ptr<Expression>> list;

        /**
         * Constructs an empty list of expressions.
         */
        ExpressionList() = default;

        /**
         * Default destructor.
         */
        virtual ~ExpressionList() = default;

        /**
         * Add an expression to the list.
         */
        void addExpression(std::shared_ptr<Expression> expression);

        /**
         * Returns the number of expressions.
         */
        int getCount() const;

        /**
         * Returns the i'th expression.
         */
        std::shared_ptr<Expression> getExpression(int index) const;

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
         * Prints this node using cQASM syntax if possible.
         */
        virtual operator std::string() const override;

    };

}}

#endif
