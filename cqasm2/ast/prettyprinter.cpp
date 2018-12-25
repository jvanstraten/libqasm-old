
#include <sstream>
#include <iostream>

#include "prettyprinter.hpp"

namespace cqasm2 { namespace ast {

    /**
     * Writes spaces to the output stream to get the indentation depth
     * requested through opts.
     */
    void PrettyPrinter::insert_indent() {
        for (int i = 0; i < this->indent_width; i++) {
            this->os << "    ";
        }
    }

    /**
     * Inserts a newline, followed by the correct indentation depth.
     */
    void PrettyPrinter::insert_newline() {
        this->os << std::endl;
        this->insert_indent();
    }

    /**
     * Recurses to the given node iff it is non-null. Otherwise writes
     * $NULL_NODE$ to the output. Dollar signs are illegal in cQASM, so
     * this text is guaranteed to remain an error if parsed again.
     * @param node The node to call apply() on.
     */
    void PrettyPrinter::safe_apply(const std::shared_ptr<Node> &node) {
        if (node) {
            this->apply(node);
        } else {
            this->os << "$NULL_NODE$";
        }
    }

    /**
     * Prints a list of annotations. This does nothing when the vector is
     * empty.
     * @param annots The annotation vector to print.
     * @param brk Whether to break the current line with a \ before
     * printing the first annotation. Lines are always broken when multiple
     * annotations are specified.
     */
    void PrettyPrinter::pprint_annots(const std::vector<std::shared_ptr<AnnotationData>> &annots, bool brk) {
        PrettyPrinter pp = *this;
        pp.indent_width += 2;
        for (auto it = annots.begin(); it != annots.end(); it++) {
            if (brk) {
                pp.os << " \\";
                pp.insert_newline();
            } else {
                pp.os << " ";
            }
            pp.os << "@";
            pp.safe_apply(*it);
            brk = true;
        }
    }

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
    void PrettyPrinter::pprint_gate_type(const std::shared_ptr<GateType> &gt, const std::string &name) {
        int len = 0;
        if (gt == nullptr) {
            for (size_t i = 0; i < gt->conds.size(); i++) {
                this->os << "c-";
                len += 2;
            }
        }
        this->os << name;
        len += name.length();
        len = 8 - len;
        do {
            this->os << " ";
            len--;
        } while (len > 0);
        if (gt == nullptr) {
            for (auto it = gt->conds.begin(); it != gt->conds.end(); it++) {
                this->safe_apply(*it);
                this->os << ", ";
                // FIXME: this is kind of ugly; a gate without operands but with
                // CDASH notation currently needs a comma at the end. This is
                // primarily a parser thing of course, it's reasonably easy to fix
                // here (print the comma in NormalGate::pprint iff there is one or
                // more condition/control).
            }
        }
    }

    /**
     * Prints a gate, including operands and annotations, using the specified
     * gate name. This name may be different from that which is stored in the
     * Gate node when the gate refers to a uniquified macro.
     * @param gate Pointer to the NormalGate node. If null, $NULL$ is printed.
     * @param name Name of the gate to print, which may be different than
     * what is stored inside the Gate.
     */
    void PrettyPrinter::pprint_gate(const std::shared_ptr<NormalGate> &gate, const std::string &name) {
        if (!gate) {
            this->os << "$NULL$";
            return;
        }
        PrettyPrinter pp = *this;
        pp.min_prec = 0;
        pp.pprint_gate_type(gate->typ, name);
        if (gate->src) {
            pp.safe_apply(gate->src);
        }
        if (gate->dest) {
            pp.os << " -> ";
            pp.safe_apply(gate->dest);
        }
        pp.pprint_annots(gate->annots, false);
    }

    /**
     * Creates a pretty-printer with default initial configuration using the
     * specified output stream.
     * @param os The output stream to print to.
     */
    PrettyPrinter::PrettyPrinter(std::ostream &os) : indent_width(0), min_prec(0), os(os) {
    }

    std::shared_ptr<QubitType> PrettyPrinter::apply_to(std::shared_ptr<QubitType> node) {
        this->os << "qubit";
        return node;
    }

    std::shared_ptr<NumericType> PrettyPrinter::apply_to(std::shared_ptr<NumericType> node) {
        PrettyPrinter pp = *this;
        pp.min_prec = __INT_MAX__;
        if (node->has_sign) {
            if (node->ibits) {
                if (node->fbits) {
                    pp.os << "fixed<";
                    pp.safe_apply(node->ibits);
                    pp.os << ",";
                    pp.safe_apply(node->fbits);
                    pp.os << ">";
                } else {
                    pp.os << "int<";
                    pp.safe_apply(node->ibits);
                    pp.os << ">";
                }
            } else {
                pp.os << "$ERROR$";
            }
        } else {
            if (node->ibits) {
                if (node->fbits) {
                    pp.os << "ufixed<";
                    pp.safe_apply(node->ibits);
                    pp.os << ",";
                    pp.safe_apply(node->fbits);
                    pp.os << ">";
                } else {
                    pp.os << "uint<";
                    pp.safe_apply(node->ibits);
                    pp.os << ">";
                }
            } else {
                if (node->fbits) {
                    pp.os << "$ERROR$";
                } else {
                    pp.os << "boolean";
                }
            }
        }
        return node;
    }

    std::shared_ptr<FloatType> PrettyPrinter::apply_to(std::shared_ptr<FloatType> node) {
        this->os << "float";
        return node;
    }

    std::shared_ptr<DoubleType> PrettyPrinter::apply_to(std::shared_ptr<DoubleType> node) {
        this->os << "double";
        return node;
    }

    std::shared_ptr<DecLiteral> PrettyPrinter::apply_to(std::shared_ptr<DecLiteral> node) {
        this->os << node->val;
        return node;
    }

    std::shared_ptr<HexLiteral> PrettyPrinter::apply_to(std::shared_ptr<HexLiteral> node) {
        this->os << node->val;
        return node;
    }

    std::shared_ptr<BinLiteral> PrettyPrinter::apply_to(std::shared_ptr<BinLiteral> node) {
        this->os << node->val;
        return node;
    }

    std::shared_ptr<FloatLiteral> PrettyPrinter::apply_to(std::shared_ptr<FloatLiteral> node) {
        this->os << node->val;
        return node;
    }

    std::shared_ptr<NamedLiteral> PrettyPrinter::apply_to(std::shared_ptr<NamedLiteral> node) {
        this->os << node->name;
        return node;
    }

    std::shared_ptr<ArrayLiteral> PrettyPrinter::apply_to(std::shared_ptr<ArrayLiteral> node) {
        PrettyPrinter pp = *this;
        pp.min_prec = __INT_MAX__;
        pp.os << "{";
        pp.safe_apply(node->data);
        pp.os << "}";
        return node;
    }

    std::shared_ptr<Identifier> PrettyPrinter::apply_to(std::shared_ptr<Identifier> node) {
        this->os << node->name;
        return node;
    }

    std::shared_ptr<ResourceRef> PrettyPrinter::apply_to(std::shared_ptr<ResourceRef> node) {
        if (node->res) {
            this->os << node->res->unique;
        } else {
            this->os << "$NULL_RESOURCE$";
        }
        return node;
    }

    std::shared_ptr<MacroParamRef> PrettyPrinter::apply_to(std::shared_ptr<MacroParamRef> node) {
        if (node->macro) {
            this->os << node->macro->name;
        } else {
            this->os << "$NULL_MACRO$";
        }
        return node;
    }

    std::shared_ptr<MacroIterRef> PrettyPrinter::apply_to(std::shared_ptr<MacroIterRef> node) {
        if (node->macro) {
            this->os << node->macro->iter;
        } else {
            this->os << "$NULL_MACRO$";
        }
        return node;
    }

    std::shared_ptr<LabelRef> PrettyPrinter::apply_to(std::shared_ptr<LabelRef> node) {
        if (node->lbl) {
            this->os << node->lbl->unique;
        } else {
            this->os << "$NULL_LABEL$";
        }
        return node;
    }

    std::shared_ptr<Subscript> PrettyPrinter::apply_to(std::shared_ptr<Subscript> node) {
        PrettyPrinter pp = *this;
        pp.min_prec = 0;
        pp.safe_apply(node->expr);
        pp.os << "." << node->subscript;
        return node;
    }

    std::shared_ptr<Indexation> PrettyPrinter::apply_to(std::shared_ptr<Indexation> node) {
        PrettyPrinter pp = *this;
        pp.min_prec = 0;
        pp.safe_apply(node->expr);
        pp.os << "[";
        pp.min_prec = __INT_MAX__;
        pp.safe_apply(node->indices);
        pp.os << "]";
        return node;
    }

    std::shared_ptr<Operation> PrettyPrinter::apply_to(std::shared_ptr<Operation> node) {
        PrettyPrinter pp = *this;
        if (node->oper.prec >= pp.min_prec) {
            pp.os << "(";
        }
        switch (node->oper.nops) {
            case 1:
                pp.os << node->oper.op1;
                pp.min_prec = node->oper.prec + 1;
                pp.safe_apply(node->op1);
                break;

            case 2:
                pp.min_prec = node->oper.prec + (node->oper.right_assoc ? 0 : 1);
                pp.safe_apply(node->op1);
                pp.os << " " << node->oper.op1 << " ";
                pp.min_prec = node->oper.prec + (node->oper.right_assoc ? 1 : 0);
                pp.safe_apply(node->op2);
                break;

            case 3:
                pp.min_prec = node->oper.prec + (node->oper.right_assoc ? 0 : 1);
                pp.safe_apply(node->op1);
                pp.os << " " << node->oper.op1 << " ";
                pp.min_prec = node->oper.prec;
                pp.safe_apply(node->op2);
                pp.os << " " << node->oper.op2 << " ";
                pp.min_prec = node->oper.prec + (node->oper.right_assoc ? 1 : 0);
                pp.safe_apply(node->op3);
                break;

            default:
                os << "$ERROR$";
        }
        if (node->oper.prec >= pp.min_prec) {
            pp.os << ")";
        }
        return node;
    }

    std::shared_ptr<Function> PrettyPrinter::apply_to(std::shared_ptr<Function> node) {
        PrettyPrinter pp = *this;
        pp.min_prec = __INT_MAX__;
        pp.os << node->name << "(";
        pp.safe_apply(node->ops);
        pp.os << ")";
        return node;
    }

    std::shared_ptr<TypeCast> PrettyPrinter::apply_to(std::shared_ptr<TypeCast> node) {
        PrettyPrinter pp = *this;
        pp.min_prec = 0;
        pp.os << "(";
        pp.safe_apply(node->typ);
        pp.os << ")";
        pp.safe_apply(node->expr);
        return node;
    }

    std::shared_ptr<ShiftCast> PrettyPrinter::apply_to(std::shared_ptr<ShiftCast> node) {
        PrettyPrinter pp = *this;
        pp.min_prec = __INT_MAX__;
        if (node->dir) {
            pp.os << "(<<";
        } else {
            pp.os << "(>>";
        }
        pp.safe_apply(node->shamt);
        pp.os << ")";
        pp.min_prec = 0;
        pp.safe_apply(node->expr);
        return node;
    }

    std::shared_ptr<ErroneousExpression> PrettyPrinter::apply_to(std::shared_ptr<ErroneousExpression> node) {
        this->os << "$ERROR$";
        return node;
    }

    std::shared_ptr<MatrixLiteral1> PrettyPrinter::apply_to(std::shared_ptr<MatrixLiteral1> node) {
        PrettyPrinter pp = *this;
        pp.min_prec = __INT_MAX__;
        pp.os << "[";
        pp.safe_apply(node->data);
        pp.os << "]";
        return node;
    }

    std::shared_ptr<MatrixLiteral2> PrettyPrinter::apply_to(std::shared_ptr<MatrixLiteral2> node) {
        PrettyPrinter pp = *this;
        pp.min_prec = __INT_MAX__;
        pp.indent_width += 1;
        pp.os << "[|";
        pp.insert_newline();
        for (auto it = node->rows.begin(); it != node->rows.end(); it++) {
            pp.safe_apply(*it);
            pp.insert_newline();
        }
        pp.os << "|]";
        return node;
    }

    std::shared_ptr<StringLiteral> PrettyPrinter::apply_to(std::shared_ptr<StringLiteral> node) {
        this->os << "\"";
        for (const char &c : node->data) {
            switch (c) {
                case '\n': this->os << "\\n"; break;
                case '\t': this->os << "\\t"; break;
                case '"':  this->os << "\\\""; break;
                case '\\': this->os << "\\\\"; break;
                default:   this->os << c;
            }
        }
        this->os << "\"";
        return node;
    }

    std::shared_ptr<JsonLiteral> PrettyPrinter::apply_to(std::shared_ptr<JsonLiteral> node) {
        this->os << "{|" << node->data << "|}";
        return node;
    }

    std::shared_ptr<ExpressionList> PrettyPrinter::apply_to(std::shared_ptr<ExpressionList> node) {
        for (auto it = node->exprs.begin(); it != node->exprs.end();) {
            this->safe_apply(*it);
            it++;
            if (it != node->exprs.end()) {
                this->os << ", ";
            }
        }
        return node;
    }

    std::shared_ptr<OperandList> PrettyPrinter::apply_to(std::shared_ptr<OperandList> node) {
        for (auto it = node->opers.begin(); it != node->opers.end();) {
            this->safe_apply(*it);
            it++;
            if (it != node->opers.end()) {
                this->os << ", ";
            }
        }
        return node;
    }

    std::shared_ptr<IdentifierList> PrettyPrinter::apply_to(std::shared_ptr<IdentifierList> node) {
        for (auto it = node->ids.begin(); it != node->ids.end();) {
            this->os << *it;
            it++;
            if (it != node->ids.end()) {
                this->os << ", ";
            }
        }
        return node;
    }

    std::shared_ptr<IndexEntry> PrettyPrinter::apply_to(std::shared_ptr<IndexEntry> node) {
        PrettyPrinter pp = *this;
        pp.min_prec = 0;
        pp.safe_apply(node->first);
        if (node->last) {
            pp.os << ":";
            pp.safe_apply(node->last);
        }
        return node;
    }

    std::shared_ptr<IndexList> PrettyPrinter::apply_to(std::shared_ptr<IndexList> node) {
        for (auto it = node->ents.begin(); it != node->ents.end();) {
            this->safe_apply(*it);
            it++;
            if (it != node->ents.end()) {
                this->os << ",";
            }
        }
        return node;
    }

    std::shared_ptr<StringBuilder> PrettyPrinter::apply_to(std::shared_ptr<StringBuilder> node) {
        this->os << "$STR_BUILDER$";
        return node;
    }

    std::shared_ptr<AnnotationData> PrettyPrinter::apply_to(std::shared_ptr<AnnotationData> node) {
        this->os << node->target << " " << node->name;
        if (node->ops) {
            PrettyPrinter pp = *this;
            pp.min_prec = 0;
            pp.os << ": ";
            pp.safe_apply(node->ops);
        }
        return node;
    }

    std::shared_ptr<GateType> PrettyPrinter::apply_to(std::shared_ptr<GateType> node) {
        this->pprint_gate_type(node, node->name);
        return node;
    }

    std::shared_ptr<NormalGate> PrettyPrinter::apply_to(std::shared_ptr<NormalGate> node) {
        if (node->typ) {
            this->pprint_gate(node, node->typ->name);
        } else {
            this->pprint_gate(node, "$NULL$");
        }
        return node;
    }

    std::shared_ptr<IfGoto> PrettyPrinter::apply_to(std::shared_ptr<IfGoto> node) {
        if (node->cond) {
            PrettyPrinter pp = *this;
            pp.min_prec = __INT_MAX__;
            pp.os << "if      ";
            pp.safe_apply(node->cond);
            pp.os << " goto ";
        } else {
            this->os << "goto    ";
        }
        this->safe_apply(node->lbl);
        return node;
    }

    std::shared_ptr<MacroCall> PrettyPrinter::apply_to(std::shared_ptr<MacroCall> node) {
        if (!node->macro || !node->macro->unique.length()) {
            this->os << "$NONAME$";
        } else {
            this->pprint_gate(node->gate, node->macro->unique);
        }
        return node;
    }

    std::shared_ptr<Pragma> PrettyPrinter::apply_to(std::shared_ptr<Pragma> node) {
        this->insert_indent();
        this->os << "pragma ";
        this->safe_apply(node->data);
        this->pprint_annots(node->annots, false);
        this->os << std::endl;
        return node;
    }

    std::shared_ptr<QubitRegister1> PrettyPrinter::apply_to(std::shared_ptr<QubitRegister1> node) {
        this->insert_indent();
        this->os << "qubits ";
        this->safe_apply(node->nqubits);
        this->pprint_annots(node->annots, false);
        this->os << std::endl;
        return node;
    }

    std::shared_ptr<ScalarResource> PrettyPrinter::apply_to(std::shared_ptr<ScalarResource> node) {
        PrettyPrinter pp = *this;
        pp.min_prec = __INT_MAX__;
        pp.insert_indent();
        pp.safe_apply(node->typ);
        pp.os << " ";
        pp.os << node->name;
        if (node->init) {
            pp.os << " = ";
            pp.safe_apply(node->init);
        }
        pp.pprint_annots(node->annots, false);
        pp.os << std::endl;
        return node;
    }

    std::shared_ptr<ArrayResource> PrettyPrinter::apply_to(std::shared_ptr<ArrayResource> node) {
        PrettyPrinter pp = *this;
        pp.min_prec = __INT_MAX__;
        pp.insert_indent();
        pp.safe_apply(node->typ);
        pp.os << " ";
        pp.os << node->name;
        pp.os << "[";
        pp.safe_apply(node->size);
        pp.os << "]";
        if (node->init) {
            pp.os << " = ";
            pp.safe_apply(node->init);
        }
        pp.pprint_annots(node->annots, false);
        pp.os << std::endl;
        return node;
    }

    std::shared_ptr<LetStatement> PrettyPrinter::apply_to(std::shared_ptr<LetStatement> node) {
        PrettyPrinter pp = *this;
        pp.min_prec = __INT_MAX__;
        pp.insert_indent();
        pp.os << "let     ";
        pp.os << node->name;
        pp.os << " = ";
        pp.safe_apply(node->init);
        pp.pprint_annots(node->annots, false);
        pp.os << std::endl;
        return node;
    }

    std::shared_ptr<Mapping> PrettyPrinter::apply_to(std::shared_ptr<Mapping> node) {
        PrettyPrinter pp = *this;
        pp.min_prec = __INT_MAX__;
        pp.insert_indent();
        pp.os << "map     " << node->name << " -> ";
        pp.safe_apply(node->expr);
        pp.pprint_annots(node->annots, false);
        pp.os << std::endl;
        return node;
    }

    std::shared_ptr<Assignment> PrettyPrinter::apply_to(std::shared_ptr<Assignment> node) {
        PrettyPrinter pp = *this;
        pp.min_prec = __INT_MAX__;
        pp.insert_indent();
        pp.os << "set     ";
        pp.safe_apply(node->lvalue);
        pp.os << " = ";
        pp.safe_apply(node->rvalue);
        pp.pprint_annots(node->annots, false);
        pp.os << std::endl;
        return node;
    }

    std::shared_ptr<MacroDef> PrettyPrinter::apply_to(std::shared_ptr<MacroDef> node) {
        this->insert_indent();
        this->os << "def ";
        if (node->unique.length()) {
            this->os << node->unique;
        } else {
            this->os << node->name;
        }
        this->os << " {" << std::endl;
        this->safe_apply(node->blk);
        this->insert_indent();
        this->os << "}";
        this->pprint_annots(node->annots, false);
        this->os << std::endl;
        return node;
    }

    std::shared_ptr<MacroFor> PrettyPrinter::apply_to(std::shared_ptr<MacroFor> node) {
        this->insert_indent();
        this->os << "for " << node->iter << " = [";
        this->safe_apply(node->indices);
        this->os << "] {" << std::endl;
        this->safe_apply(node->blk);
        this->insert_indent();
        this->os << "}";
        this->pprint_annots(node->annots, false);
        this->os << std::endl;
        return node;
    }

    std::shared_ptr<MacroIfElse> PrettyPrinter::apply_to(std::shared_ptr<MacroIfElse> node) {
        PrettyPrinter pp = *this;
        pp.min_prec = __INT_MAX__;
        pp.insert_indent();
        pp.os << "if (";
        pp.safe_apply(node->cond);
        pp.os << ") {" << std::endl;
        pp.safe_apply(node->blk_true);
        if (node->blk_false) {
            pp.insert_indent();
            pp.os << "} else {" << std::endl;
            pp.safe_apply(node->blk_false);
        }
        pp.insert_indent();
        pp.os << "}";
        pp.pprint_annots(node->annots, false);
        pp.os << std::endl;
        return node;
    }

    std::shared_ptr<Include> PrettyPrinter::apply_to(std::shared_ptr<Include> node) {
        this->insert_indent();
        this->os << "include ";
        this->safe_apply(node->fname);
        this->pprint_annots(node->annots, false);
        this->os << std::endl;
        return node;
    }

    std::shared_ptr<Subcircuit> PrettyPrinter::apply_to(std::shared_ptr<Subcircuit> node) {
        PrettyPrinter pp = *this;
        pp.indent_width -= 1;
        pp.min_prec = __INT_MAX__;
        pp.insert_indent();
        pp.os << "." << node->name;
        if (node->iter) {
            pp.os << "(";
            pp.safe_apply(node->iter);
            pp.os << ")";
        }
        pp.indent_width += 1;
        pp.pprint_annots(node->annots, false);
        pp.os << std::endl;
        return node;
    }

    std::shared_ptr<Label> PrettyPrinter::apply_to(std::shared_ptr<Label> node) {
        PrettyPrinter pp = *this;
        pp.indent_width -= 1;
        pp.min_prec = __INT_MAX__;
        pp.insert_indent();
        if (node->unique.length()) {
            pp.os << node->unique;
        } else {
            pp.os << node->name;
        }
        pp.os << ":";
        pp.indent_width += 1;
        pp.pprint_annots(node->annots, false);
        pp.os << std::endl;
        return node;
    }

    std::shared_ptr<Bundle> PrettyPrinter::apply_to(std::shared_ptr<Bundle> node) {
        this->insert_indent();
        if (node->gates.size() == 1 && !node->annots.size()) {
            this->safe_apply(node->gates.at(0));
        } else {
            this->os << "{";
            this->os << std::endl;
            PrettyPrinter pp = *this;
            pp.indent_width += 1;
            for (auto it = node->gates.begin(); it != node->gates.end(); it++) {
                pp.insert_indent();
                pp.safe_apply(*it);
                pp.os << std::endl;
            }
            this->insert_indent();
            this->os << "}";
        }
        this->pprint_annots(node->annots, false);
        this->os << std::endl;
        return node;
    }

    std::shared_ptr<ErroneousStatement> PrettyPrinter::apply_to(std::shared_ptr<ErroneousStatement> node) {
        this->os << "$ERROR$";
        return node;
    }

    std::shared_ptr<Block> PrettyPrinter::apply_to(std::shared_ptr<Block> node) {
        PrettyPrinter pp = *this;
        pp.indent_width += 1;
        for (auto it = node->stmts.begin(); it != node->stmts.end(); it++) {
            pp.safe_apply(*it);
        }
        return node;
    }

    std::shared_ptr<Program> PrettyPrinter::apply_to(std::shared_ptr<Program> node) {
        this->os << node->version << std::endl;
        this->os << std::endl;
        this->safe_apply(node->blk);
        return node;
    }

}}
