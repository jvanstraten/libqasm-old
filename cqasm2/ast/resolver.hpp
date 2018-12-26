
#ifndef _LIBCQASM2_AST_RESOLVER_H_
#define _LIBCQASM2_AST_RESOLVER_H_

#include <cstdlib>
#include <cstdint>
#include <string>
#include <memory>
#include <ios>
#include <unordered_map>
#include "ast.hpp"

namespace cqasm2 { namespace ast {

    /**
     * This transformation resolves Identifiers, IfGoto label references, and
     * gates referring to macros. It removed all Mapping nodes from the code,
     * and moves resource declarations and MacroDef nodes to the respective
     * maps in the surrounding scope. Note that this transformation does not
     * resolve all NormalGates and Identifiers; if a gate does not refer to
     * a macro or an identifier does not refer to any known name, they are
     * left alone silently. A later transformation should check whether this
     * is okay.
     */
    class Resolver : public Transformation {
    protected:

        /**
         * The block we're currently traversing.
         */
        std::shared_ptr<Block> block;

        /**
         * Identifier scope. In the first pass over a block, this is populated
         * with LabelRef objects. It is an error during this pass to override
         * an existing reference (duplicate label). During the second pass,
         * resource declarations, mappings, and macro parameters are added.
         * These may override labels, which may result in errors later on.
         */
        std::unordered_map<std::string, std::shared_ptr<Reference>> idToRef;

        /**
         * Gate name scope. This is populated in the first pass over a block.
         * It is an error to override an existing reference (duplicate macro).
         */
        std::unordered_map<std::string, std::shared_ptr<MacroDef>> gateToDef;

        /**
         * Parent resolver when blocks are nested, null if we're in the
         * toplevel block.
         */
        std::shared_ptr<Resolver> parent;

    public:

        /**
         * Creates a pretty-printer with default initial configuration using the
         * specified output stream.
         * @param os The output stream to print to.
         */
        Resolver(std::ostream &os);

        /**
         * Default destructor.
         */
        virtual ~Resolver() = default;

        /*
         * Each block opens a new namespace. Labels, macros, resources, and
         * mappings can refer to the parent block(s), but the current block
         * takes precedence.
         */
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<Block> node)               override;

        /*
         * Label and MacroDef declare names with forward and backward scope; it
         * does not matter where they are defined within the containing block.
         * Duplicate names are therefore illegal. They are, however,
         * independently namespaced. MacroDef and MacroFor furthermore declare
         * parameter names in the forward identifier namespace for their
         * parameters/iteration.
         *
         * MacroDefs are removed from the statement vector and placed in the
         * macro map instead.
         */
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<Label> node)               override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<MacroDef> node)            override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<MacroFor> node)            override;

        /*
         * The following nodes declare resource names within the forward
         * identifier namespace. They are removed from the statement vector
         * and placed in the resource map instead, with a uniquified name if
         * necessary.
         */
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<QubitRegister1> node)      override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<ScalarResource> node)      override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<ArrayResource> node)       override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<LetStatement> node)        override;

        /*
         * Mappings declare names within the forward identifier namespace.
         * Their expression is first resolved, then the resolved expression is
         * placed in the identifier map such that future references to this
         * mapping can be replaced with it as they are encountered. The mapping
         * statement itself is removed from the statement vector.
         */
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<Mapping> node)             override;

        /*
         * Identifiers are resolved from (first) the forward identifier
         * namespace and (secondly) the label namespace. This transformation
         * leaves undefined identifiers alone; they may be legal later on in
         * the form of raw identifier operands.
         */
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<Identifier> node)          override;

        /*
         * The label in IfGoto gates is resolved from the label namespace. A
         * reference to an undefined label is an error here.
         */
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<IfGoto> node)              override;

        /*
         * The names of NormalGates are checked against the macro namespace and
         * resolved if they exist. This pass also matches parameter count and
         * checks that the macro call is not conditional.
         */
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<NormalGate> node)          override;

    };

}}

#endif
