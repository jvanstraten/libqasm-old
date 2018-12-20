
#ifndef _LIBCQASM2_AST_NAMESPACE_H_
#define _LIBCQASM2_AST_NAMESPACE_H_

#include <cstdlib>
#include <cstdint>
#include <string>

namespace cqasm2 { namespace ast {

    /**
     * Represents a cQASM namespace, used during the reference resolution
     * process.
     */
    class Namespace : public std::exception {
    };

}}

#endif
