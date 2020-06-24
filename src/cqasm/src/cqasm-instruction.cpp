#include "cqasm-instruction.hpp"
#include "cqasm-utils.hpp"

namespace cqasm {
namespace instruction {

/**
 * Creates a new instruction. param_types is a shorthand type specification
 * string as parsed by cqasm::types::from_spec(). If you need more control,
 * you can also manipulate param_types directly.
 *
 * allow_conditional specifies whether the instruction can be made
 * conditional with c- notation. allow_parallel specifies whether it may
 * appear bundled with other instructions. allow_reused_qubits specifies
 * whether it is legal for the instruction to use a qubit more than once in
 * its parameter list.
 */
Instruction::Instruction(
    const std::string &name,
    const std::string &param_types,
    bool allow_conditional,
    bool allow_parallel,
    bool allow_reused_qubits
) :
    name(name),
    param_types(types::from_spec(param_types)),
    allow_conditional(allow_conditional),
    allow_parallel(allow_parallel),
    allow_reused_qubits(allow_reused_qubits)
{}

/**
 * Equality operator.
 */
bool Instruction::operator==(const Instruction& rhs) const {
    return utils::case_insensitive_equals(name, rhs.name)
            && param_types == rhs.param_types
            && allow_conditional == rhs.allow_conditional
            && allow_parallel == rhs.allow_parallel
            && allow_reused_qubits == rhs.allow_reused_qubits;
}

} // namespace instruction
} // namespace cqasm

/**
 * Stream << overload for instructions.
 */
std::ostream& operator<<(std::ostream& os, const ::cqasm::instruction::Instruction& insn) {
    os << insn.name << insn.param_types;
    return os;
}

/**
 * Stream << overload for instruction references.
 */
std::ostream& operator<<(std::ostream& os, const ::cqasm::instruction::InstructionRef& insn) {
    if (insn.empty()) {
        os << "unresolved";
    } else {
        os << *insn;
    }
    return os;
}
