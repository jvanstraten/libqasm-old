
#ifndef _LIBCQASM2_AST_NODE_H_
#define _LIBCQASM2_AST_NODE_H_

#include <cstdlib>
#include <cstdint>
#include <string>
#include <memory>
#include <list>
//#include "cqasm2/semantic/namespace.hpp"

namespace cqasm2 { namespace ast {

    class Node {
    protected:

        /**
         * Source filename.
         */
        std::string source;

        /**
         * Line number within the source file.
         */
        int line;

        /**
         * Column within the source file.
         */
        int column;

        /**
         * List of macro expansions that this node was generated from.
         */
        std::list<std::string> expansions;

    public:

        /**
         * Constructor. When constructed during parsing, this will copy over
         * the source filename, line number, and column number from Yacc. When
         * constructed out of a parsing context, this information is set to
         * "<internal>" with line 1 column 1.
         */
        Node();

        /**
         * Default destructor.
         */
        virtual ~Node() = default;

        /**
         * Copy line source, line number, and column information from the given
         * node.
         */
        void setSource(const Node &similar);

        /**
         * Set source, line number, and column directly.
         */
        void setSource(std::string source, int line, int column);

        /**
         * Adds an expansion to the list.
         */
        void addExpansion(std::string expansion);

        /**
         * Returns the source of this node. Expansions are listed first, each
         * with a newline, followed by the filename, line number, and column.
         */
        std::string getSource() const;

        /**
         * Resolves all references in this node tree, and gives all
         * declarations a unique name.
         */
        //virtual void resolve(Namespace &names) = 0;

        /**
         * Performs macro expansion, constant folding, type checking, etc.
         */
        //virtual std::shared_ptr<Node> elaborate() = 0;

        /**
         * Prints this node using cQASM syntax if possible.
         */
        //virtual operator std::string() const = 0;

    };

}}

#endif
