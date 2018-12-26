
#include <sstream>
#include <iostream>

#include "ifgotoreducer.hpp"

namespace cqasm2 { namespace ast {

    /**
     * Reduces an IfGoto node to an equivalent NormalGate.
     */
    std::shared_ptr<Node> IfGotoReducer::apply_to(std::shared_ptr<IfGoto> node) {

        // Default to unconditional jump.
        auto params = std::make_shared<OperandList>();
        std::string name = "jmp";

        // Handle conditional jumps.
        if (node->cond) {
            bool known_binop = false;
            if (typeid(*(node->cond)) == typeid(Operation)) {
                auto op = std::dynamic_pointer_cast<Operation>(node->cond);
                if (op->oper == operators::CGT) { name = "cgt"; known_binop = true; } else
                if (op->oper == operators::CLT) { name = "clt"; known_binop = true; } else
                if (op->oper == operators::CGE) { name = "cge"; known_binop = true; } else
                if (op->oper == operators::CLE) { name = "cle"; known_binop = true; } else
                if (op->oper == operators::CEQ) { name = "ceq"; known_binop = true; } else
                if (op->oper == operators::CNE) { name = "cne"; known_binop = true; }
                if (known_binop) {
                    params->push_oper(op->op1);
                    params->push_oper(op->op2);
                }
            }
            if (!known_binop) {
                name = "cnz";
                params->push_oper(node->cond);
            }
        }

        // Push the label operand last.
        params->push_oper(node->lbl);

        // Create the new gate.
        auto gate = std::make_shared<NormalGate>(std::make_shared<GateType>(name), params);

        // Copy source information from the IfGoto node.
        gate->set_source(*node);
        gate->typ->set_source(*node);
        params->set_source(*node);

        return gate;
    }

}}
