
#include <sstream>

#include "exceptions.hpp"

namespace cqasm2 { namespace ast {

    /**
     * Builds an error message based on the offending node.
     */
    NodeError::NodeError(const Node &node, const std::string &explanation) {
        std::ostringstream os;
        os << node.getSource() << ":" << explanation;
        this->msg = os.str();
    }

    /**
     * Returns what this error is all about.
     */
    const char* NodeError::what() const noexcept {
        return this->msg.c_str();
    }

    /**
     * Builds an internal error message.
     */
    InternalError::InternalError(const std::string &explanation) : std::runtime_error(explanation) {
    }

}}
