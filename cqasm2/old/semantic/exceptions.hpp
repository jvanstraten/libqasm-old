
#ifndef _LIBCQASM2_AST_EXCEPTIONS_H_
#define _LIBCQASM2_AST_EXCEPTIONS_H_

#include <cstdlib>
#include <cstdint>
#include <string>
#include "cqasm2/ast/node.hpp"

namespace cqasm2 { namespace ast {

    /**
     * Represents an error message related to a node, i.e. with
     * file/location information.
     */
    class NodeError : public std::exception {
    protected:

        /**
         * Exception message.
         */
        std::string msg;

    public:

        /**
         * Builds an error message based on the offending node.
         */
        NodeError(const Node &node, const std::string &explanation);

        /**
         * Default destructor.
         */
        virtual ~NodeError() = default;

        /**
         * Returns what this error is all about.
         */
        virtual const char* what() const noexcept;

    };

    /**
     * Represents an internal error. You should not be getting these.
     */
    class InternalError : public std::runtime_error {
    public:

        /**
         * Builds an internal error message.
         */
        InternalError(const std::string &explanation);

        /**
         * Default destructor.
         */
        virtual ~InternalError() = default;

    };

}}

#endif
