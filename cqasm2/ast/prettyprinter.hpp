
#ifndef _LIBCQASM2_AST_PRETTYPRINTER_H_
#define _LIBCQASM2_AST_PRETTYPRINTER_H_

#include <cstdlib>
#include <cstdint>
#include <string>
#include <memory>
#include <ios>
#include "ast.hpp"

namespace cqasm2 { namespace ast {

    /**
     * Pretty-print transformation class. Does not modify or change allocation
     * of the AST, but instead prints a cQASM version of the code to the
     * specified output stream.
     */
    class PrettyPrinter : public Transformation {
    protected:

        /**
         * Number of spaces to indent by.
         */
        int indent_width;

        /**
         * Minimum precedence level that warrents placing parentheses around an
         * expression before printing it.
         */
        int min_prec;

        /**
         * Output stream to print to.
         */
        std::ostream &os;

        /**
         * Writes spaces to the output stream to get the indentation depth
         * requested through opts.
         */
        void insert_indent();

        /**
         * Inserts a newline, followed by the correct indentation depth.
         */
        void insert_newline();

        /**
         * Recurses to the given node iff it is non-null. Otherwise writes
         * $NULL_NODE$ to the output. Dollar signs are illegal in cQASM, so
         * this text is guaranteed to remain an error if parsed again.
         * @param node The node to call apply() on.
         */
        void safe_apply(const std::shared_ptr<Node> &node);

        /**
         * Prints a list of annotations. This does nothing when the vector is
         * empty.
         * @param annots The annotation vector to print.
         * @param brk Whether to break the current line with a \ before
         * printing the first annotation. Lines are always broken when multiple
         * annotations are specified.
         */
        void pprint_annots(const std::vector<std::shared_ptr<AnnotationData>> &annots, bool brk);

        /**
         * Prints a "gate type" (its name prefixed with the right number of c-
         * strings and suffixed with the conditional expressions) using the
         * specified name. This name may be different from that which is stored
         * in the GateType node when the gate refers to a uniquified macro.
         * @param gt Pointer to the GateType node. If null, the gate name is
         * printed without conditional arguments.
         * @param name Name of the gate to print, which may be different than
         * what is stored inside the GateType.
         */
        void pprint_gate_type(const std::shared_ptr<GateType> &gt, const std::string &name);

        /**
         * Prints a gate, including operands and annotations, using the specified
         * gate name. This name may be different from that which is stored in the
         * Gate node when the gate refers to a uniquified macro.
         * @param gate Pointer to the NormalGate node. If null, $NULL$ is printed.
         * @param name Name of the gate to print, which may be different than
         * what is stored inside the Gate.
         */
        void pprint_gate(const std::shared_ptr<NormalGate> &gate, const std::string &name);

    public:

        /**
         * Creates a pretty-printer with default initial configuration using the
         * specified output stream.
         * @param os The output stream to print to.
         */
        PrettyPrinter(std::ostream &os);

        /**
         * Default destructor.
         */
        virtual ~PrettyPrinter() = default;

        /*
         * Overrides for transormation application.
         */
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<QubitType> node)           override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<NumericType> node)         override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<FloatType> node)           override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<DoubleType> node)          override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<DecLiteral> node)          override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<HexLiteral> node)          override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<BinLiteral> node)          override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<FloatLiteral> node)        override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<NamedLiteral> node)        override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<ArrayLiteral> node)        override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<Identifier> node)          override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<ResourceRef> node)         override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<MacroParamRef> node)       override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<MacroIterRef> node)        override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<LabelRef> node)            override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<Subscript> node)           override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<Indexation> node)          override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<Operation> node)           override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<Function> node)            override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<TypeCast> node)            override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<ShiftCast> node)           override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<ErroneousExpression> node) override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<MatrixLiteral1> node)      override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<MatrixLiteral2> node)      override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<StringLiteral> node)       override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<JsonLiteral> node)         override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<ExpressionList> node)      override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<OperandList> node)         override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<IdentifierList> node)      override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<IndexEntry> node)          override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<IndexList> node)           override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<StringBuilder> node)       override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<AnnotationData> node)      override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<GateType> node)            override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<NormalGate> node)          override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<IfGoto> node)              override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<MacroCall> node)           override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<Pragma> node)              override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<QubitRegister1> node)      override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<ScalarResource> node)      override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<ArrayResource> node)       override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<LetStatement> node)        override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<Mapping> node)             override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<Assignment> node)          override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<MacroDef> node)            override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<MacroFor> node)            override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<MacroIfElse> node)         override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<Include> node)             override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<Subcircuit> node)          override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<Label> node)               override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<Bundle> node)              override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<ErroneousStatement> node)  override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<Block> node)               override;
        virtual std::shared_ptr<Node> apply_to(std::shared_ptr<Program> node)             override;

    };

}}

#endif
