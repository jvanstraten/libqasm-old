
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
        std::cout << "OH MAN WE MADE A NODE\n";
        this->source = "<internal>";
        this->line   = yylloc.first_line;
        this->column = yylloc.first_column;
    }

    /**
     * Copy line source, line number, and column information from the given
     * node.
     */
    void Node::setSource(const Node &similar) {
        this->source = similar.source;
        this->line   = similar.line;
        this->column = similar.column;
    }

    /**
     * Set source, line number, and column directly.
     */
    void Node::setSource(std::string source, int line, int column) {
        this->source = source;
        this->line   = line;
        this->column = column;
    }

    /**
     * Adds an expansion to the list.
     */
    void Node::addExpansion(std::string expansion) {
        this->expansions.push_back(expansion);
    }

    /**
     * Returns the source of this node. Expansions are listed first, each
     * with a newline, followed by the filename, line number, and column.
     */
    std::string Node::getSource() const {
        std::ostringstream os;
        for (auto it = this->expansions.begin(); it != this->expansions.end(); it++) {
            os << "Inside macro " << *it << ":" << std::endl;
        }
        os << this->source << ":" << this->line << ":" << this->column;
        return os.str();
    }

}}
