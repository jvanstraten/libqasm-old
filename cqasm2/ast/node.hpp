
#ifndef _LIBCQASM2_AST_NODE_H_
#define _LIBCQASM2_AST_NODE_H_

#include <cstdlib>
#include <cstdint>
#include <string>
#include <memory>
#include <list>

namespace cqasm2 { namespace ast {

    /**
     * Options that can be passed to pprint() to change the way it behaves.
     */
    typedef struct {

        /**
         * Number of spaces to indent by.
         */
        int indent_width;

        /**
         * Minimum precedence level that warrents placing parentheses around an
         * expression before printing it.
         */
        int min_prec;

    } pprint_opts_t;

    /**
     * Base class for AST nodes. Includes position information.
     */
    class Node {
    public:

        /**
         * Source filename.
         */
        std::string source;

        /**
         * Starting line number within the source file.
         */
        int first_line;

        /**
         * Starting column within the source file.
         */
        int first_column;

        /**
         * Final line number within the source file.
         */
        int last_line;

        /**
         * Final column within the source file.
         */
        int last_column;

        /**
         * List of macro expansions that this node was generated from.
         */
        std::list<std::string> expansions;

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
         * @param similar Node to copy data from.
         */
        void set_source(const Node &similar);

        /**
         * Adds an expansion to the list. These are printed, in order, in error
         * messages relating to nodes.
         * @param expansion Line to print.
         */
        void push_expansion(std::string expansion);

        /**
         * @return The source of this node. Expansions are listed first, each
         * with a newline, followed by the filename, line number, and column.
         */
        std::string fmt_source() const;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         * @return Debug representation of this node.
         */
        virtual operator std::string() const = 0;

        /**
         * Pretty-print to the given output stream with the given indentation
         * level.
         * @param os Stream to output to.
         * @param opts Structure containing printing options.
         */
        virtual void pprint(std::ostream &os, const pprint_opts_t &opts) const = 0;

    };

}}

#endif
