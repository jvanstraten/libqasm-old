
#include <sstream>
#include <string>

#include "ast.hpp"

namespace cqasm2 { namespace ast {

    static void insert_indent(std::ostream &os, const pprint_opts_t &opts) {
        for (int i = 0; i < opts.indent_width; i++) {
            os << "    ";
        }
    }

    static void insert_newline(std::ostream &os, const pprint_opts_t &opts) {
        os << std::endl;
        insert_indent(os, opts);
    }

    static void safe_pprint(const std::shared_ptr<Node> &node, std::ostream &os, const pprint_opts_t &opts) {
        if (node) {
            node->pprint(os, opts);
        } else {
            os << "$NULL_NODE$";
        }
    }

    void QubitType::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        (void)opts;
        os << "qubit";
    }

    void NumericType::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        pprint_opts_t iopts = opts;
        iopts.min_prec = __INT_MAX__;
        if (this->has_sign) {
            if (this->ibits) {
                if (this->fbits) {
                    os << "fixed<";
                    safe_pprint(this->ibits, os, iopts);
                    os << ",";
                    safe_pprint(this->fbits, os, iopts);
                    os << ">";
                } else {
                    os << "int<";
                    safe_pprint(this->ibits, os, iopts);
                    os << ">";
                }
            } else {
                os << "$ERROR$";
            }
        } else {
            if (this->ibits) {
                if (this->fbits) {
                    os << "ufixed<";
                    safe_pprint(this->ibits, os, iopts);
                    os << ",";
                    safe_pprint(this->fbits, os, iopts);
                    os << ">";
                } else {
                    os << "uint<";
                    safe_pprint(this->ibits, os, iopts);
                    os << ">";
                }
            } else {
                if (this->fbits) {
                    os << "$ERROR$";
                } else {
                    os << "boolean";
                }
            }
        }
    }

    void FloatType::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        (void)opts;
        os << "float";
    }

    void DoubleType::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        (void)opts;
        os << "double";
    }

    void DecLiteral::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        (void)opts;
        os << this->val;
    }

    void HexLiteral::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        (void)opts;
        os << this->val;
    }

    void BinLiteral::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        (void)opts;
        os << this->val;
    }

    void FloatLiteral::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        (void)opts;
        os << this->val;
    }

    void NamedLiteral::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        (void)opts;
        os << this->name;
    }

    void ArrayLiteral::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        pprint_opts_t iopts = opts;
        iopts.min_prec = __INT_MAX__;
        os << "{";
        safe_pprint(this->data, os, iopts);
        os << "}";
    }

    void Identifier::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        (void)opts;
        os << this->name;
    }

    void ResourceRef::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        (void)opts;
        if (this->res) {
            os << this->res->unique;
        } else {
            os << "$NULL_RESOURCE$";
        }
    }

    void MacroParamRef::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        (void)opts;
        if (this->macro) {
            os << this->macro->name;
        } else {
            os << "$NULL_MACRO$";
        }
    }

    void MacroIterRef::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        (void)opts;
        if (this->macro) {
            os << this->macro->iter;
        } else {
            os << "$NULL_MACRO$";
        }
    }

    void LabelRef::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        (void)opts;
        if (this->lbl) {
            os << this->lbl->unique;
        } else {
            os << "$NULL_LABEL$";
        }
    }

    void Subscript::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        pprint_opts_t iopts = opts;
        iopts.min_prec = 0;
        safe_pprint(this->expr, os, iopts);
        os << "." << this->subscript;
    }

    void Indexation::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        pprint_opts_t iopts = opts;
        iopts.min_prec = 0;
        safe_pprint(this->expr, os, iopts);
        os << "[";
        iopts.min_prec = __INT_MAX__;
        safe_pprint(this->indices, os, iopts);
        os << "]";
    }

    void Operation::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        pprint_opts_t iopts = opts;
        if (this->oper.prec >= opts.min_prec) {
            os << "(";
        }
        switch (this->oper.nops) {
            case 1:
                os << this->oper.op1;
                iopts.min_prec = this->oper.prec + 1;
                safe_pprint(this->op1, os, iopts);
                break;

            case 2:
                iopts.min_prec = this->oper.prec + (this->oper.right_assoc ? 0 : 1); // TODO check this logic extensively!
                safe_pprint(this->op1, os, iopts);
                os << " " << this->oper.op1 << " ";
                iopts.min_prec = this->oper.prec + (this->oper.right_assoc ? 1 : 0);
                safe_pprint(this->op2, os, iopts);
                break;

            case 3:
                iopts.min_prec = this->oper.prec + (this->oper.right_assoc ? 0 : 1);
                safe_pprint(this->op1, os, iopts);
                os << " " << this->oper.op1 << " ";
                iopts.min_prec = this->oper.prec;
                safe_pprint(this->op2, os, iopts);
                os << " " << this->oper.op2 << " ";
                iopts.min_prec = this->oper.prec + (this->oper.right_assoc ? 1 : 0);
                safe_pprint(this->op3, os, iopts);
                break;

            default:
                os << "$ERROR$";
        }
        if (this->oper.prec >= opts.min_prec) {
            os << ")";
        }
    }

    void Function::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        pprint_opts_t iopts = opts;
        iopts.min_prec = __INT_MAX__;
        os << this->name << "(";
        safe_pprint(this->ops, os, iopts);
        os << ")";
    }

    void TypeCast::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        pprint_opts_t iopts = opts;
        iopts.min_prec = 0;
        os << "(";
        safe_pprint(this->typ, os, iopts);
        os << ")";
        safe_pprint(this->expr, os, iopts);
    }

    void ShiftCast::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        pprint_opts_t iopts = opts;
        iopts.min_prec = __INT_MAX__;
        if (this->dir) {
            os << "(<<";
        } else {
            os << "(>>";
        }
        safe_pprint(this->shamt, os, iopts);
        os << ")";
        iopts.min_prec = 0;
        safe_pprint(this->expr, os, iopts);
    }

    void ErroneousExpression::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        os << "$ERROR$";
    }

    void MatrixLiteral1::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        pprint_opts_t iopts = opts;
        iopts.min_prec = __INT_MAX__;
        os << "[";
        safe_pprint(this->data, os, iopts);
        os << "]";
    }

    void MatrixLiteral2::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        pprint_opts_t iopts = opts;
        iopts.min_prec = __INT_MAX__;
        iopts.indent_width += 1;
        os << "[|";
        insert_newline(os, iopts);
        for (auto it = this->rows.begin(); it != this->rows.end(); it++) {
            safe_pprint(*it, os, iopts);
            insert_newline(os, iopts);
        }
        os << "|]";
    }

    void StringLiteral::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        (void)opts;
        os << "\"";
        for (const char &c : this->data) {
            switch (c) {
                case '\n': os << "\\n"; break;
                case '\t': os << "\\t"; break;
                case '"':  os << "\\\""; break;
                case '\\': os << "\\\\"; break;
                default: os << c;
            }
        }
        os << "\"";
    }

    void JsonLiteral::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        (void)opts;
        os << "{|" << this->data << "|}";
    }

    void ExpressionList::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        for (auto it = this->exprs.begin(); it != this->exprs.end();) {
            safe_pprint(*it, os, opts);
            it++;
            if (it != this->exprs.end()) {
                os << ", ";
            }
        }
    }

    void OperandList::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        for (auto it = this->opers.begin(); it != this->opers.end();) {
            safe_pprint(*it, os, opts);
            it++;
            if (it != this->opers.end()) {
                os << ", ";
            }
        }
    }

    void IdentifierList::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        (void)opts;
        for (auto it = this->ids.begin(); it != this->ids.end();) {
            os << *it;
            it++;
            if (it != this->ids.end()) {
                os << ", ";
            }
        }
    }

    void IndexEntry::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        pprint_opts_t iopts = opts;
        iopts.min_prec = 0;
        safe_pprint(this->first, os, iopts);
        if (this->last) {
            os << ":";
            safe_pprint(this->last, os, iopts);
        }
    }

    void IndexList::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        (void)opts;
        for (auto it = this->ents.begin(); it != this->ents.end();) {
            safe_pprint(*it, os, opts);
            it++;
            if (it != this->ents.end()) {
                os << ",";
            }
        }
    }

    void StringBuilder::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        (void)opts;
        os << "$STR_BUILDER$";
    }

    void AnnotationData::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        os << this->target << " " << this->name;
        if (this->ops) {
            pprint_opts_t iopts = opts;
            iopts.min_prec = 0;
            os << ": ";
            safe_pprint(this->ops, os, iopts);
        }
    }

    static void pprint_gate_type(const GateType *gt, const std::string &name, std::ostream &os, const pprint_opts_t &opts) {
        int len = 0;
        if (gt) {
            for (int i = 0; i < gt->conds.size(); i++) {
                os << "c-";
                len += 2;
            }
        }
        os << name;
        len += name.length();
        len = 8 - len;
        do {
            os << " ";
            len--;
        } while (len > 0);
        if (gt) {
            for (auto it = gt->conds.begin(); it != gt->conds.end(); it++) {
                safe_pprint(*it, os, opts);
                os << ", ";
                // FIXME: this is kind of ugly; a gate without operands but with
                // CDASH notation currently needs a comma at the end. This is
                // primarily a parser thing of course, it's reasonably easy to fix
                // here (print the comma in NormalGate::pprint iff there is one or
                // more condition/control).
            }
        }
    }

    void GateType::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        pprint_gate_type(this, this->name, os, opts);
    }

    static void pprint_annots(const std::vector<std::shared_ptr<AnnotationData>> &annots, bool brk, std::ostream &os, const pprint_opts_t &opts) {
        pprint_opts_t iopts = opts;
        iopts.indent_width += 2;
        for (auto it = annots.begin(); it != annots.end(); it++) {
            if (brk) {
                os << " \\";
                insert_newline(os, iopts);
            } else {
                os << " ";
            }
            os << "@";
            safe_pprint(*it, os, iopts);
            brk = true;
        }
    }

    static void pprint_gate(const NormalGate *gate, const std::string &name, std::ostream &os, const pprint_opts_t &opts) {
        if (!gate) {
            os << "$NULL$";
            return;
        }
        pprint_opts_t iopts = opts;
        iopts.min_prec = 0;
        pprint_gate_type(gate->typ.get(), name, os, iopts);
        if (gate->src) {
            safe_pprint(gate->src, os, iopts);
        }
        if (gate->dest) {
            os << " -> ";
            safe_pprint(gate->dest, os, iopts);
        }
        pprint_annots(gate->annots, false, os, iopts);
    }

    void NormalGate::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        if (this->typ) {
            pprint_gate(this, this->typ->name, os, opts);
        } else {
            pprint_gate(this, "$NULL$", os, opts);
        }
    }

    void IfGoto::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        if (this->cond) {
            pprint_opts_t iopts = opts;
            iopts.min_prec = __INT_MAX__;
            os << "if      ";
            safe_pprint(this->cond, os, iopts);
            os << " goto ";
        } else {
            os << "goto    ";
        }
        safe_pprint(this->lbl, os, opts);
    }

    void MacroCall::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        if (!this->macro || !this->macro->unique.length()) {
            os << "$NONAME$";
        } else {
            pprint_gate(this->gate.get(), this->macro->unique, os, opts);
        }
    }

    void Pragma::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        insert_indent(os, opts);
        os << "pragma ";
        safe_pprint(this->data, os, opts);
        pprint_annots(this->annots, false, os, opts);
        os << std::endl;
    }

    void QubitRegister1::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        insert_indent(os, opts);
        os << "qubits ";
        safe_pprint(this->nqubits, os, opts);
        pprint_annots(this->annots, false, os, opts);
        os << std::endl;
    }

    void ScalarResource::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        pprint_opts_t iopts = opts;
        iopts.min_prec = __INT_MAX__;
        insert_indent(os, opts);
        safe_pprint(this->typ, os, opts);
        os << " ";
        os << this->name;
        if (this->init) {
            os << " = ";
            safe_pprint(this->init, os, iopts);
        }
        pprint_annots(this->annots, false, os, opts);
        os << std::endl;
    }

    void ArrayResource::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        pprint_opts_t iopts = opts;
        iopts.min_prec = __INT_MAX__;
        insert_indent(os, opts);
        safe_pprint(this->typ, os, opts);
        os << " ";
        os << this->name;
        os << "[";
        safe_pprint(this->size, os, iopts);
        os << "]";
        if (this->init) {
            os << " = ";
            safe_pprint(this->init, os, iopts);
        }
        pprint_annots(this->annots, false, os, opts);
        os << std::endl;
    }

    void LetStatement::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        pprint_opts_t iopts = opts;
        iopts.min_prec = __INT_MAX__;
        insert_indent(os, opts);
        os << "let     ";
        os << this->name;
        os << " = ";
        safe_pprint(this->init, os, iopts);
        pprint_annots(this->annots, false, os, opts);
        os << std::endl;
    }

    void Mapping::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        pprint_opts_t iopts = opts;
        iopts.min_prec = __INT_MAX__;
        insert_indent(os, opts);
        os << "map     " << this->name << " -> ";
        safe_pprint(this->expr, os, iopts);
        pprint_annots(this->annots, false, os, opts);
        os << std::endl;
    }

    void Assignment::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        pprint_opts_t iopts = opts;
        iopts.min_prec = __INT_MAX__;
        insert_indent(os, opts);
        os << "set     ";
        safe_pprint(this->lvalue, os, iopts);
        os << " = ";
        safe_pprint(this->rvalue, os, iopts);
        pprint_annots(this->annots, false, os, opts);
        os << std::endl;
    }

    void MacroDef::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        insert_indent(os, opts);
        os << "def ";
        if (this->unique.length()) {
            os << this->unique;
        } else {
            os << this->name;
        }
        os << " {" << std::endl;
        safe_pprint(this->blk, os, opts);
        insert_indent(os, opts);
        os << "}";
        pprint_annots(this->annots, false, os, opts);
        os << std::endl;
    }

    void MacroFor::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        insert_indent(os, opts);
        os << "for " << this->iter << " = [";
        safe_pprint(this->indices, os, opts);
        os << "] {" << std::endl;
        safe_pprint(this->blk, os, opts);
        insert_indent(os, opts);
        os << "}";
        pprint_annots(this->annots, false, os, opts);
        os << std::endl;
    }

    void MacroIfElse::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        pprint_opts_t iopts = opts;
        iopts.min_prec = __INT_MAX__;
        insert_indent(os, opts);
        os << "if (";
        safe_pprint(this->cond, os, iopts);
        os << ") {" << std::endl;
        safe_pprint(this->blk_true, os, opts);
        if (this->blk_false) {
            insert_indent(os, opts);
            os << "} else {" << std::endl;
            safe_pprint(this->blk_false, os, opts);
        }
        insert_indent(os, opts);
        os << "}";
        pprint_annots(this->annots, false, os, opts);
        os << std::endl;
    }

    void Include::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        insert_indent(os, opts);
        os << "include ";
        safe_pprint(this->fname, os, opts);
        pprint_annots(this->annots, false, os, opts);
        os << std::endl;
    }

    void Subcircuit::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        pprint_opts_t iopts = opts;
        iopts.indent_width -= 1;
        iopts.min_prec = __INT_MAX__;
        insert_indent(os, iopts);
        os << "." << this->name;
        if (this->iter) {
            os << "(";
            safe_pprint(this->iter, os, iopts);
            os << ")";
        }
        pprint_annots(this->annots, false, os, opts);
        os << std::endl;
    }

    void Label::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        pprint_opts_t iopts = opts;
        iopts.indent_width -= 1;
        iopts.min_prec = __INT_MAX__;
        insert_indent(os, iopts);
        if (this->unique.length()) {
            os << this->unique;
        } else {
            os << this->name;
        }
        os << ":";
        pprint_annots(this->annots, false, os, opts);
        os << std::endl;
    }

    void Bundle::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        insert_indent(os, opts);
        if (this->gates.size() == 1 && !this->annots.size()) {
            safe_pprint(this->gates.at(0), os, opts);
        } else {
            os << "{";
            os << std::endl;
            pprint_opts_t iopts = opts;
            iopts.indent_width += 1;
            for (auto it = this->gates.begin(); it != this->gates.end(); it++) {
                insert_indent(os, iopts);
                safe_pprint(*it, os, iopts);
                os << std::endl;
            }
            insert_indent(os, opts);
            os << "}";
        }
        pprint_annots(this->annots, false, os, opts);
        os << std::endl;
    }

    void ErroneousStatement::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        os << "$ERROR$";
    }

    void Block::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        pprint_opts_t iopts = opts;
        iopts.indent_width += 1;
        for (auto it = this->stmts.begin(); it != this->stmts.end(); it++) {
            safe_pprint(*it, os, iopts);
        }
    }

    void Program::pprint(std::ostream &os, const pprint_opts_t &opts) const {
        os << this->version << std::endl;
        os << std::endl;
        this->blk->pprint(os, opts);
    }


}}
