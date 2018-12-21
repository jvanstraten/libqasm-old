
#include <sstream>
#include <iostream>

#include "ast.hpp"
#include "parser.hpp"

namespace cqasm2 { namespace ast {

    /**
     * Constructor. When constructed during parsing, this will copy over
     * the source filename, line number, and column number from Yacc. When
     * constructed out of a parsing context, this information is set to
     * "<internal>" with line 1 column 1.
     */
    Node::Node() {
        // TODO: grab from Yacc globals!
        this->source = "<internal>";
        this->first_line   = yylloc.first_line;
        this->first_column = yylloc.first_column;
        this->last_line    = yylloc.last_line;
        this->last_column  = yylloc.last_column;
    }

    /**
     * Copy line source, line number, and column information from the given
     * node.
     * @param similar Node to copy data from.
     */
    void Node::set_source(const Node &similar) {
        this->source = similar.source;
        this->first_line   = similar.first_line;
        this->first_column = similar.first_column;
        this->last_line    = similar.last_line;
        this->last_column  = similar.last_column;
    }

    /**
     * Adds an expansion to the list. These are printed, in order, in error
     * messages relating to nodes.
     * @param expansion Line to print.
     */
    void Node::push_expansion(std::string expansion) {
        this->expansions.push_back(expansion);
    }

    /**
     * @return The source of this node. Expansions are listed first, each
     * with a newline, followed by the filename, line number, and column.
     */
    std::string Node::fmt_source() const {
        std::ostringstream os;
        for (auto it = this->expansions.begin(); it != this->expansions.end(); it++) {
            os << "Inside macro " << *it << ":" << std::endl;
        }
        os << this->source << ":" << this->first_line << ":" << this->first_column;
        return os.str();
    }

}}
