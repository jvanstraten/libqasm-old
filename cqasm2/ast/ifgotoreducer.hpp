
#ifndef _LIBCQASM2_AST_IFGOTOREDUCER_H_
#define _LIBCQASM2_AST_IFGOTOREDUCER_H_

#include <cstdlib>
#include <cstdint>
#include <string>
#include <memory>
#include "ast.hpp"

namespace cqasm2 { namespace ast {

    /**
     * This transformation reduces IfGoto gates to equivalent NormalGates.
     */
    class IfGotoReducer : public Transformation {
    public:

        /**
         * Creates an IfGoto reducer.
         */
        IfGotoReducer() = default;

        /**
         * Default destructor.
         */
        virtual ~IfGotoReducer() = default;

        /*
         * The label in IfGoto gates is resolved from the label namespace. A
         * reference to an undefined label is an error here.
         */
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<IfGoto> node) override;

    };

}}

#endif
