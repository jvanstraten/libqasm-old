#include "ast.gen.hpp"

using namespace cqasm2;

namespace cqasm2 { namespace ast {



    /**
     * Constructor for Type.
     */
    Type::Type() {
    }

    /**
     * Constructor for TypeLiteral.
     */
    TypeLiteral::TypeLiteral() {
    }

    /**
     * Constructor for QubitType.
     */
    QubitType::QubitType() {
    }

    /**
     * Constructor for NumericType.
     * @param has_sign Whether this is a signed or unsigned type.
     */
    NumericType::NumericType(bool has_sign):
        has_sign(has_sign)
    {}

    /**
     * Constructor for NumericType.
     * @param has_sign Whether this is a signed or unsigned type.
     * @param ibits Number of bits before the decimal separator.
     */
    NumericType::NumericType(bool has_sign, std::shared_ptr<Expression> ibits):
        has_sign(has_sign),
        ibits(ibits)
    {}

    /**
     * Constructor for NumericType, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param has_sign Whether this is a signed or unsigned type.
     * @param ibits Number of bits before the decimal separator.
     */
    NumericType::NumericType(bool has_sign, Expression * ibits):
        has_sign(has_sign),
        ibits(std::shared_ptr<Expression>(ibits))
    {
    }

    /**
     * Constructor for NumericType.
     * @param has_sign Whether this is a signed or unsigned type.
     * @param ibits Number of bits before the decimal separator.
     * @param fbits Number of bits after the decimal separator.
     */
    NumericType::NumericType(bool has_sign, std::shared_ptr<Expression> ibits, std::shared_ptr<Expression> fbits):
        has_sign(has_sign),
        ibits(ibits),
        fbits(fbits)
    {}

    /**
     * Constructor for NumericType, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param has_sign Whether this is a signed or unsigned type.
     * @param ibits Number of bits before the decimal separator.
     * @param fbits Number of bits after the decimal separator.
     */
    NumericType::NumericType(bool has_sign, Expression * ibits, Expression * fbits):
        has_sign(has_sign),
        ibits(std::shared_ptr<Expression>(ibits)),
        fbits(std::shared_ptr<Expression>(fbits))
    {
    }

    /**
     * Constructor for FloatType.
     */
    FloatType::FloatType() {
    }

    /**
     * Constructor for DoubleType.
     */
    DoubleType::DoubleType() {
    }

    /**
     * Constructor for Operand.
     */
    Operand::Operand() {
    }

    /**
     * Constructor for Expression.
     */
    Expression::Expression() {
    }

    /**
     * Constructor for NumericLiteral.
     */
    NumericLiteral::NumericLiteral() {
    }

    /**
     * Constructor for DecLiteral.
     * @param val The value as it appears in the source file.
     */
    DecLiteral::DecLiteral(std::string val):
        val(val)
    {}

    /**
     * Constructor for DecLiteral, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param val The value as it appears in the source file. The passed pointer
     * will be free()d by this constructor!
     */
    DecLiteral::DecLiteral(char * val):
        val(std::string(val))
    {
        free(val);
    }

    /**
     * Constructor for HexLiteral.
     * @param val The value as it appears in the source file.
     */
    HexLiteral::HexLiteral(std::string val):
        val(val)
    {}

    /**
     * Constructor for HexLiteral, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param val The value as it appears in the source file. The passed pointer
     * will be free()d by this constructor!
     */
    HexLiteral::HexLiteral(char * val):
        val(std::string(val))
    {
        free(val);
    }

    /**
     * Constructor for BinLiteral.
     * @param val The value as it appears in the source file.
     */
    BinLiteral::BinLiteral(std::string val):
        val(val)
    {}

    /**
     * Constructor for BinLiteral, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param val The value as it appears in the source file. The passed pointer
     * will be free()d by this constructor!
     */
    BinLiteral::BinLiteral(char * val):
        val(std::string(val))
    {
        free(val);
    }

    /**
     * Constructor for FloatLiteral.
     * @param val The value as it appears in the source file.
     */
    FloatLiteral::FloatLiteral(std::string val):
        val(val)
    {}

    /**
     * Constructor for FloatLiteral, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param val The value as it appears in the source file. The passed pointer
     * will be free()d by this constructor!
     */
    FloatLiteral::FloatLiteral(char * val):
        val(std::string(val))
    {
        free(val);
    }

    /**
     * Constructor for NamedLiteral.
     * @param name The name of the constant.
     */
    NamedLiteral::NamedLiteral(std::string name):
        name(name)
    {}

    /**
     * Constructor for NamedLiteral, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param name The name of the constant. The passed pointer will be free()d
     * by this constructor!
     */
    NamedLiteral::NamedLiteral(char * name):
        name(std::string(name))
    {
        free(name);
    }

    /**
     * Constructor for Identifier.
     * @param name Identifier.
     */
    Identifier::Identifier(std::string name):
        name(name)
    {}

    /**
     * Constructor for Identifier, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param name Identifier. The passed pointer will be free()d by this
     * constructor!
     */
    Identifier::Identifier(char * name):
        name(std::string(name))
    {
        free(name);
    }

    /**
     * Constructor for ResourceRef.
     * @param res Resolved resource.
     */
    ResourceRef::ResourceRef(std::shared_ptr<Resource> res):
        res(res)
    {}

    /**
     * Constructor for ResourceRef, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param res Resolved resource.
     */
    ResourceRef::ResourceRef(Resource * res):
        res(std::shared_ptr<Resource>(res))
    {
    }

    /**
     * Constructor for ResourceRef.
     * @param res Resolved resource.
     * @param id Original identifier that was used for this reference, if any.
     */
    ResourceRef::ResourceRef(std::shared_ptr<Resource> res, std::shared_ptr<Identifier> id):
        res(res),
        id(id)
    {}

    /**
     * Constructor for ResourceRef, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param res Resolved resource.
     * @param id Original identifier that was used for this reference, if any.
     */
    ResourceRef::ResourceRef(Resource * res, Identifier * id):
        res(std::shared_ptr<Resource>(res)),
        id(std::shared_ptr<Identifier>(id))
    {
    }

    /**
     * Constructor for MacroParamRef.
     * @param macro Macro definition this parameter was resolved to.
     * @param param Parameter index.
     */
    MacroParamRef::MacroParamRef(std::shared_ptr<MacroDef> macro, int param):
        macro(macro),
        param(param)
    {}

    /**
     * Constructor for MacroParamRef, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param macro Macro definition this parameter was resolved to.
     * @param param Parameter index.
     */
    MacroParamRef::MacroParamRef(MacroDef * macro, int param):
        macro(std::shared_ptr<MacroDef>(macro)),
        param(param)
    {
    }

    /**
     * Constructor for MacroParamRef.
     * @param macro Macro definition this parameter was resolved to.
     * @param param Parameter index.
     * @param id Original identifier that was used for this reference, if any.
     */
    MacroParamRef::MacroParamRef(std::shared_ptr<MacroDef> macro, int param, std::shared_ptr<Identifier> id):
        macro(macro),
        param(param),
        id(id)
    {}

    /**
     * Constructor for MacroParamRef, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param macro Macro definition this parameter was resolved to.
     * @param param Parameter index.
     * @param id Original identifier that was used for this reference, if any.
     */
    MacroParamRef::MacroParamRef(MacroDef * macro, int param, Identifier * id):
        macro(std::shared_ptr<MacroDef>(macro)),
        param(param),
        id(std::shared_ptr<Identifier>(id))
    {
    }

    /**
     * Constructor for MacroIterRef.
     * @param macro For loop that this parameter was resolved to.
     */
    MacroIterRef::MacroIterRef(std::shared_ptr<MacroFor> macro):
        macro(macro)
    {}

    /**
     * Constructor for MacroIterRef, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param macro For loop that this parameter was resolved to.
     */
    MacroIterRef::MacroIterRef(MacroFor * macro):
        macro(std::shared_ptr<MacroFor>(macro))
    {
    }

    /**
     * Constructor for MacroIterRef.
     * @param macro For loop that this parameter was resolved to.
     * @param id Original identifier that was used for this reference, if any.
     */
    MacroIterRef::MacroIterRef(std::shared_ptr<MacroFor> macro, std::shared_ptr<Identifier> id):
        macro(macro),
        id(id)
    {}

    /**
     * Constructor for MacroIterRef, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param macro For loop that this parameter was resolved to.
     * @param id Original identifier that was used for this reference, if any.
     */
    MacroIterRef::MacroIterRef(MacroFor * macro, Identifier * id):
        macro(std::shared_ptr<MacroFor>(macro)),
        id(std::shared_ptr<Identifier>(id))
    {
    }

    /**
     * Constructor for LabelRef.
     * @param lbl Label that this identifier was resolved to.
     */
    LabelRef::LabelRef(std::shared_ptr<Label> lbl):
        lbl(lbl)
    {}

    /**
     * Constructor for LabelRef, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param lbl Label that this identifier was resolved to.
     */
    LabelRef::LabelRef(Label * lbl):
        lbl(std::shared_ptr<Label>(lbl))
    {
    }

    /**
     * Constructor for LabelRef.
     * @param lbl Label that this identifier was resolved to.
     * @param id Original identifier that was used for this reference, if any.
     */
    LabelRef::LabelRef(std::shared_ptr<Label> lbl, std::shared_ptr<Identifier> id):
        lbl(lbl),
        id(id)
    {}

    /**
     * Constructor for LabelRef, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param lbl Label that this identifier was resolved to.
     * @param id Original identifier that was used for this reference, if any.
     */
    LabelRef::LabelRef(Label * lbl, Identifier * id):
        lbl(std::shared_ptr<Label>(lbl)),
        id(std::shared_ptr<Identifier>(id))
    {
    }

    /**
     * Constructor for Subscript.
     * @param expr Expression to disambiguate with subscript notation.
     * @param subscript The subscript text.
     */
    Subscript::Subscript(std::shared_ptr<Expression> expr, std::string subscript):
        expr(expr),
        subscript(subscript)
    {}

    /**
     * Constructor for Subscript, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param expr Expression to disambiguate with subscript notation.
     * @param subscript The subscript text. The passed pointer will be free()d
     * by this constructor!
     */
    Subscript::Subscript(Expression * expr, char * subscript):
        expr(std::shared_ptr<Expression>(expr)),
        subscript(std::string(subscript))
    {
        free(subscript);
    }

    /**
     * Constructor for Indexation.
     * @param expr Expression to index.
     * @param indices List of index entries to index by.
     */
    Indexation::Indexation(std::shared_ptr<Expression> expr, std::shared_ptr<IndexList> indices):
        expr(expr),
        indices(indices)
    {}

    /**
     * Constructor for Indexation, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param expr Expression to index.
     * @param indices List of index entries to index by.
     */
    Indexation::Indexation(Expression * expr, IndexList * indices):
        expr(std::shared_ptr<Expression>(expr)),
        indices(std::shared_ptr<IndexList>(indices))
    {
    }

    /**
     * Constructor for Operation.
     * @param fun Whether this is a function call (true) or an inline operator
     * (false).
     * @param oper Operator or function name.
     * @param ops Operand list.
     */
    Operation::Operation(bool fun, std::string oper, std::shared_ptr<ExpressionList> ops):
        fun(fun),
        oper(oper),
        ops(ops)
    {}

    /**
     * Constructor for Operation, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param fun Whether this is a function call (true) or an inline operator
     * (false).
     * @param oper Operator or function name. The passed pointer will be free()d
     * by this constructor!
     * @param ops Operand list.
     */
    Operation::Operation(bool fun, char * oper, ExpressionList * ops):
        fun(fun),
        oper(std::string(oper)),
        ops(std::shared_ptr<ExpressionList>(ops))
    {
        free(oper);
    }

    /**
     * Constructor for TypeCast.
     * @param typ Type to cast to.
     * @param expr Expression to typecast.
     */
    TypeCast::TypeCast(std::shared_ptr<Type> typ, std::shared_ptr<Expression> expr):
        typ(typ),
        expr(expr)
    {}

    /**
     * Constructor for TypeCast, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param typ Type to cast to.
     * @param expr Expression to typecast.
     */
    TypeCast::TypeCast(Type * typ, Expression * expr):
        typ(std::shared_ptr<Type>(typ)),
        expr(std::shared_ptr<Expression>(expr))
    {
    }

    /**
     * Constructor for ShiftCast.
     * @param dir False: (&gt;&gt;a)b = shift decimal point left; true:
     * (&lt;&lt;a)b = shift decimal point right.
     * @param shamt Shift amount.
     * @param expr Expression to typecast.
     */
    ShiftCast::ShiftCast(bool dir, std::shared_ptr<Expression> shamt, std::shared_ptr<Expression> expr):
        dir(dir),
        shamt(shamt),
        expr(expr)
    {}

    /**
     * Constructor for ShiftCast, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param dir False: (&gt;&gt;a)b = shift decimal point left; true:
     * (&lt;&lt;a)b = shift decimal point right.
     * @param shamt Shift amount.
     * @param expr Expression to typecast.
     */
    ShiftCast::ShiftCast(bool dir, Expression * shamt, Expression * expr):
        dir(dir),
        shamt(std::shared_ptr<Expression>(shamt)),
        expr(std::shared_ptr<Expression>(expr))
    {
    }

    /**
     * Constructor for ErroneousExpression.
     */
    ErroneousExpression::ErroneousExpression() {
    }

    /**
     * Constructor for MatrixLiteral.
     */
    MatrixLiteral::MatrixLiteral() {
    }

    /**
     * Constructor for MatrixLiteral1.
     * @param data Represents the data contained in this matrix. Each expression
     * is alternatingly the real resp. imaginary part of the matrix. The matrix
     * must furthermore be square.
     */
    MatrixLiteral1::MatrixLiteral1(std::shared_ptr<ExpressionList> data):
        data(data)
    {}

    /**
     * Constructor for MatrixLiteral1, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param data Represents the data contained in this matrix. Each expression
     * is alternatingly the real resp. imaginary part of the matrix. The matrix
     * must furthermore be square.
     */
    MatrixLiteral1::MatrixLiteral1(ExpressionList * data):
        data(std::shared_ptr<ExpressionList>(data))
    {
    }

    /**
     * Constructor for MatrixLiteral2.
     */
    MatrixLiteral2::MatrixLiteral2() {
    }

    /**
     * Appends to row vector. Returns reference to this to allow chaining.
     * @param row Value to push.
     * @return this, to allow chaining.
     */
    MatrixLiteral2 *MatrixLiteral2::push_row(std::shared_ptr<ExpressionList> row) {
        this->rows.push_back(row);
        return this;
    }

    /**
     * Appends to row vector. Returns reference to this to allow chaining.
     * @param row Value to push.
     * @return this, to allow chaining.
     */
    MatrixLiteral2 *MatrixLiteral2::push_row(ExpressionList * row) {
        this->rows.push_back(std::shared_ptr<ExpressionList>(row));
        return this;
    }

    /**
     * Constructor for StringLiteral.
     * @param data Contained string literal.
     */
    StringLiteral::StringLiteral(std::string data):
        data(data)
    {}

    /**
     * Constructor for StringLiteral, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param data Contained string literal. The passed pointer will be free()d
     * by this constructor!
     */
    StringLiteral::StringLiteral(char * data):
        data(std::string(data))
    {
        free(data);
    }

    /**
     * Constructor for JsonLiteral.
     * @param data Contained JSON string.
     */
    JsonLiteral::JsonLiteral(std::string data):
        data(data)
    {}

    /**
     * Constructor for JsonLiteral, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param data Contained JSON string. The passed pointer will be free()d by
     * this constructor!
     */
    JsonLiteral::JsonLiteral(char * data):
        data(std::string(data))
    {
        free(data);
    }

    /**
     * Constructor for ExpressionList.
     */
    ExpressionList::ExpressionList() {
    }

    /**
     * Appends to expr vector. Returns reference to this to allow chaining.
     * @param expr Value to push.
     * @return this, to allow chaining.
     */
    ExpressionList *ExpressionList::push_expr(std::shared_ptr<Expression> expr) {
        this->exprs.push_back(expr);
        return this;
    }

    /**
     * Appends to expr vector. Returns reference to this to allow chaining.
     * @param expr Value to push.
     * @return this, to allow chaining.
     */
    ExpressionList *ExpressionList::push_expr(Expression * expr) {
        this->exprs.push_back(std::shared_ptr<Expression>(expr));
        return this;
    }

    /**
     * Constructor for OperandList.
     */
    OperandList::OperandList() {
    }

    /**
     * Appends to oper vector. Returns reference to this to allow chaining.
     * @param oper Value to push.
     * @return this, to allow chaining.
     */
    OperandList *OperandList::push_oper(std::shared_ptr<Operand> oper) {
        this->opers.push_back(oper);
        return this;
    }

    /**
     * Appends to oper vector. Returns reference to this to allow chaining.
     * @param oper Value to push.
     * @return this, to allow chaining.
     */
    OperandList *OperandList::push_oper(Operand * oper) {
        this->opers.push_back(std::shared_ptr<Operand>(oper));
        return this;
    }

    /**
     * Constructor for IdentifierList.
     */
    IdentifierList::IdentifierList() {
    }

    /**
     * Appends to id vector. Returns reference to this to allow chaining.
     * @param id Value to push.
     * @return this, to allow chaining.
     */
    IdentifierList *IdentifierList::push_id(std::string id) {
        this->ids.push_back(id);
        return this;
    }

    /**
     * Constructor for IndexEntry.
     * @param first First index in this range, or the selected index if
     * singular.
     */
    IndexEntry::IndexEntry(std::shared_ptr<Expression> first):
        first(first)
    {}

    /**
     * Constructor for IndexEntry, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param first First index in this range, or the selected index if
     * singular.
     */
    IndexEntry::IndexEntry(Expression * first):
        first(std::shared_ptr<Expression>(first))
    {
    }

    /**
     * Constructor for IndexEntry.
     * @param first First index in this range, or the selected index if
     * singular.
     * @param last Last index in this range, or null if singular.
     */
    IndexEntry::IndexEntry(std::shared_ptr<Expression> first, std::shared_ptr<Expression> last):
        first(first),
        last(last)
    {}

    /**
     * Constructor for IndexEntry, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param first First index in this range, or the selected index if
     * singular.
     * @param last Last index in this range, or null if singular.
     */
    IndexEntry::IndexEntry(Expression * first, Expression * last):
        first(std::shared_ptr<Expression>(first)),
        last(std::shared_ptr<Expression>(last))
    {
    }

    /**
     * Constructor for IndexList.
     */
    IndexList::IndexList() {
    }

    /**
     * Appends to ent vector. Returns reference to this to allow chaining.
     * @param ent Value to push.
     * @return this, to allow chaining.
     */
    IndexList *IndexList::push_ent(std::shared_ptr<IndexEntry> ent) {
        this->ents.push_back(ent);
        return this;
    }

    /**
     * Appends to ent vector. Returns reference to this to allow chaining.
     * @param ent Value to push.
     * @return this, to allow chaining.
     */
    IndexList *IndexList::push_ent(IndexEntry * ent) {
        this->ents.push_back(std::shared_ptr<IndexEntry>(ent));
        return this;
    }

    /**
     * Constructor for StringBuilder.
     */
    StringBuilder::StringBuilder() {
    }

    /**
     * Constructor for AnnotationData.
     * @param target Name of the target that this pragma/annotation is intended
     * for.
     * @param name Name of the pragma/annotation.
     */
    AnnotationData::AnnotationData(std::string target, std::string name):
        target(target),
        name(name)
    {}

    /**
     * Constructor for AnnotationData, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param target Name of the target that this pragma/annotation is intended
     * for. The passed pointer will be free()d by this constructor!
     * @param name Name of the pragma/annotation. The passed pointer will be
     * free()d by this constructor!
     */
    AnnotationData::AnnotationData(char * target, char * name):
        target(std::string(target)),
        name(std::string(name))
    {
        free(target);
        free(name);
    }

    /**
     * Constructor for AnnotationData.
     * @param target Name of the target that this pragma/annotation is intended
     * for.
     * @param name Name of the pragma/annotation.
     * @param ops List of operands, if specified, that are attached to the
     * annotation/pragma.
     */
    AnnotationData::AnnotationData(std::string target, std::string name, std::shared_ptr<OperandList> ops):
        target(target),
        name(name),
        ops(ops)
    {}

    /**
     * Constructor for AnnotationData, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param target Name of the target that this pragma/annotation is intended
     * for. The passed pointer will be free()d by this constructor!
     * @param name Name of the pragma/annotation. The passed pointer will be
     * free()d by this constructor!
     * @param ops List of operands, if specified, that are attached to the
     * annotation/pragma.
     */
    AnnotationData::AnnotationData(char * target, char * name, OperandList * ops):
        target(std::string(target)),
        name(std::string(name)),
        ops(std::shared_ptr<OperandList>(ops))
    {
        free(target);
        free(name);
    }

    /**
     * Constructor for GateType.
     * @param name Name of the gate.
     */
    GateType::GateType(std::string name):
        name(name)
    {}

    /**
     * Constructor for GateType, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param name Name of the gate. The passed pointer will be free()d by this
     * constructor!
     */
    GateType::GateType(char * name):
        name(std::string(name))
    {
        free(name);
    }

    /**
     * Appends to cond vector. Returns reference to this to allow chaining.
     * @param cond Value to push.
     * @return this, to allow chaining.
     */
    GateType *GateType::push_cond(std::shared_ptr<Expression> cond) {
        this->conds.push_back(cond);
        return this;
    }

    /**
     * Appends to cond vector. Returns reference to this to allow chaining.
     * @param cond Value to push.
     * @return this, to allow chaining.
     */
    GateType *GateType::push_cond(Expression * cond) {
        this->conds.push_back(std::shared_ptr<Expression>(cond));
        return this;
    }

    /**
     * Constructor for Gate.
     */
    Gate::Gate() {
    }

    /**
     * Constructor for UnresolvedGate.
     */
    UnresolvedGate::UnresolvedGate() {
    }

    /**
     * Appends to annot vector. Returns reference to this to allow chaining.
     * @param annot Value to push.
     * @return this, to allow chaining.
     */
    UnresolvedGate *UnresolvedGate::push_annot(std::shared_ptr<AnnotationData> annot) {
        this->annots.push_back(annot);
        return this;
    }

    /**
     * Appends to annot vector. Returns reference to this to allow chaining.
     * @param annot Value to push.
     * @return this, to allow chaining.
     */
    UnresolvedGate *UnresolvedGate::push_annot(AnnotationData * annot) {
        this->annots.push_back(std::shared_ptr<AnnotationData>(annot));
        return this;
    }

    /**
     * Constructor for NormalGate.
     * @param typ Contains the name of the gate and its conditional expressions.
     */
    NormalGate::NormalGate(std::shared_ptr<GateType> typ):
        typ(typ)
    {}

    /**
     * Constructor for NormalGate, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param typ Contains the name of the gate and its conditional expressions.
     */
    NormalGate::NormalGate(GateType * typ):
        typ(std::shared_ptr<GateType>(typ))
    {
    }

    /**
     * Constructor for NormalGate.
     * @param typ Contains the name of the gate and its conditional expressions.
     * @param src Source operand list.
     */
    NormalGate::NormalGate(std::shared_ptr<GateType> typ, std::shared_ptr<OperandList> src):
        typ(typ),
        src(src)
    {}

    /**
     * Constructor for NormalGate, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param typ Contains the name of the gate and its conditional expressions.
     * @param src Source operand list.
     */
    NormalGate::NormalGate(GateType * typ, OperandList * src):
        typ(std::shared_ptr<GateType>(typ)),
        src(std::shared_ptr<OperandList>(src))
    {
    }

    /**
     * Constructor for NormalGate.
     * @param typ Contains the name of the gate and its conditional expressions.
     * @param src Source operand list.
     * @param dest Destination operand list.
     */
    NormalGate::NormalGate(std::shared_ptr<GateType> typ, std::shared_ptr<OperandList> src, std::shared_ptr<OperandList> dest):
        typ(typ),
        src(src),
        dest(dest)
    {}

    /**
     * Constructor for NormalGate, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param typ Contains the name of the gate and its conditional expressions.
     * @param src Source operand list.
     * @param dest Destination operand list.
     */
    NormalGate::NormalGate(GateType * typ, OperandList * src, OperandList * dest):
        typ(std::shared_ptr<GateType>(typ)),
        src(std::shared_ptr<OperandList>(src)),
        dest(std::shared_ptr<OperandList>(dest))
    {
    }

    /**
     * Constructor for IfGoto.
     * @param lbl Name of the label to jump to.
     */
    IfGoto::IfGoto(std::string lbl):
        lbl(lbl)
    {}

    /**
     * Constructor for IfGoto, intended to be used from YACC only. This version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param lbl Name of the label to jump to. The passed pointer will be
     * free()d by this constructor!
     */
    IfGoto::IfGoto(char * lbl):
        lbl(std::string(lbl))
    {
        free(lbl);
    }

    /**
     * Constructor for IfGoto.
     * @param lbl Name of the label to jump to.
     * @param expr The condition for jumping, or null for always.
     */
    IfGoto::IfGoto(std::string lbl, std::shared_ptr<Expression> expr):
        lbl(lbl),
        expr(expr)
    {}

    /**
     * Constructor for IfGoto, intended to be used from YACC only. This version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param lbl Name of the label to jump to. The passed pointer will be
     * free()d by this constructor!
     * @param expr The condition for jumping, or null for always.
     */
    IfGoto::IfGoto(char * lbl, Expression * expr):
        lbl(std::string(lbl)),
        expr(std::shared_ptr<Expression>(expr))
    {
        free(lbl);
    }

    /**
     * Constructor for MacroCall.
     * @param gate Gate description from the source code.
     * @param macro Macro that this gate was resolved to.
     */
    MacroCall::MacroCall(std::shared_ptr<NormalGate> gate, std::shared_ptr<MacroDef> macro):
        gate(gate),
        macro(macro)
    {}

    /**
     * Constructor for MacroCall, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param gate Gate description from the source code.
     * @param macro Macro that this gate was resolved to.
     */
    MacroCall::MacroCall(NormalGate * gate, MacroDef * macro):
        gate(std::shared_ptr<NormalGate>(gate)),
        macro(std::shared_ptr<MacroDef>(macro))
    {
    }

    /**
     * Constructor for Statement.
     */
    Statement::Statement() {
    }

    /**
     * Appends to annot vector. Returns reference to this to allow chaining.
     * @param annot Value to push.
     * @return this, to allow chaining.
     */
    Statement *Statement::push_annot(std::shared_ptr<AnnotationData> annot) {
        this->annots.push_back(annot);
        return this;
    }

    /**
     * Appends to annot vector. Returns reference to this to allow chaining.
     * @param annot Value to push.
     * @return this, to allow chaining.
     */
    Statement *Statement::push_annot(AnnotationData * annot) {
        this->annots.push_back(std::shared_ptr<AnnotationData>(annot));
        return this;
    }

    /**
     * Constructor for Pragma.
     * @param data Data belonging to this pragma statement.
     */
    Pragma::Pragma(std::shared_ptr<AnnotationData> data):
        data(data)
    {}

    /**
     * Constructor for Pragma, intended to be used from YACC only. This version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param data Data belonging to this pragma statement.
     */
    Pragma::Pragma(AnnotationData * data):
        data(std::shared_ptr<AnnotationData>(data))
    {
    }

    /**
     * Constructor for Resource.
     */
    Resource::Resource() {
    }

    /**
     * Constructor for QubitRegister1.
     * @param nqubits Number of qubits.
     */
    QubitRegister1::QubitRegister1(std::shared_ptr<NumericLiteral> nqubits):
        nqubits(nqubits)
    {}

    /**
     * Constructor for QubitRegister1, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param nqubits Number of qubits.
     */
    QubitRegister1::QubitRegister1(NumericLiteral * nqubits):
        nqubits(std::shared_ptr<NumericLiteral>(nqubits))
    {
    }

    /**
     * Constructor for ScalarResource.
     * @param typ Type of the resource.
     * @param name Name of the resource.
     */
    ScalarResource::ScalarResource(std::shared_ptr<Type> typ, std::string name):
        typ(typ),
        name(name)
    {}

    /**
     * Constructor for ScalarResource, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param typ Type of the resource.
     * @param name Name of the resource. The passed pointer will be free()d by
     * this constructor!
     */
    ScalarResource::ScalarResource(Type * typ, char * name):
        typ(std::shared_ptr<Type>(typ)),
        name(std::string(name))
    {
        free(name);
    }

    /**
     * Constructor for ScalarResource.
     * @param typ Type of the resource.
     * @param name Name of the resource.
     * @param init Initializer expression.
     */
    ScalarResource::ScalarResource(std::shared_ptr<Type> typ, std::string name, std::shared_ptr<Expression> init):
        typ(typ),
        name(name),
        init(init)
    {}

    /**
     * Constructor for ScalarResource, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param typ Type of the resource.
     * @param name Name of the resource. The passed pointer will be free()d by
     * this constructor!
     * @param init Initializer expression.
     */
    ScalarResource::ScalarResource(Type * typ, char * name, Expression * init):
        typ(std::shared_ptr<Type>(typ)),
        name(std::string(name)),
        init(std::shared_ptr<Expression>(init))
    {
        free(name);
    }

    /**
     * Constructor for ArrayResource.
     * @param typ Type of the array entries.
     * @param name Name of the resource.
     * @param size Array size expression.
     */
    ArrayResource::ArrayResource(std::shared_ptr<Type> typ, std::string name, std::shared_ptr<Expression> size):
        typ(typ),
        name(name),
        size(size)
    {}

    /**
     * Constructor for ArrayResource, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param typ Type of the array entries.
     * @param name Name of the resource. The passed pointer will be free()d by
     * this constructor!
     * @param size Array size expression.
     */
    ArrayResource::ArrayResource(Type * typ, char * name, Expression * size):
        typ(std::shared_ptr<Type>(typ)),
        name(std::string(name)),
        size(std::shared_ptr<Expression>(size))
    {
        free(name);
    }

    /**
     * Constructor for ArrayResource.
     * @param typ Type of the array entries.
     * @param name Name of the resource.
     * @param size Array size expression.
     * @param init Initializer expression.
     */
    ArrayResource::ArrayResource(std::shared_ptr<Type> typ, std::string name, std::shared_ptr<Expression> size, std::shared_ptr<Expression> init):
        typ(typ),
        name(name),
        size(size),
        init(init)
    {}

    /**
     * Constructor for ArrayResource, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param typ Type of the array entries.
     * @param name Name of the resource. The passed pointer will be free()d by
     * this constructor!
     * @param size Array size expression.
     * @param init Initializer expression.
     */
    ArrayResource::ArrayResource(Type * typ, char * name, Expression * size, Expression * init):
        typ(std::shared_ptr<Type>(typ)),
        name(std::string(name)),
        size(std::shared_ptr<Expression>(size)),
        init(std::shared_ptr<Expression>(init))
    {
        free(name);
    }

    /**
     * Constructor for LetStatement.
     * @param name Name of the resource.
     * @param init Initializer expression.
     */
    LetStatement::LetStatement(std::string name, std::shared_ptr<Expression> init):
        name(name),
        init(init)
    {}

    /**
     * Constructor for LetStatement, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param name Name of the resource. The passed pointer will be free()d by
     * this constructor!
     * @param init Initializer expression.
     */
    LetStatement::LetStatement(char * name, Expression * init):
        name(std::string(name)),
        init(std::shared_ptr<Expression>(init))
    {
        free(name);
    }

    /**
     * Constructor for Mapping.
     * @param name Name of the mapping.
     */
    Mapping::Mapping(std::string name):
        name(name)
    {}

    /**
     * Constructor for Mapping, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param name Name of the mapping. The passed pointer will be free()d by
     * this constructor!
     */
    Mapping::Mapping(char * name):
        name(std::string(name))
    {
        free(name);
    }

    /**
     * Constructor for Mapping.
     * @param name Name of the mapping.
     * @param expr Expression to map to.
     */
    Mapping::Mapping(std::string name, std::shared_ptr<Expression> expr):
        name(name),
        expr(expr)
    {}

    /**
     * Constructor for Mapping, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param name Name of the mapping. The passed pointer will be free()d by
     * this constructor!
     * @param expr Expression to map to.
     */
    Mapping::Mapping(char * name, Expression * expr):
        name(std::string(name)),
        expr(std::shared_ptr<Expression>(expr))
    {
        free(name);
    }

    /**
     * Constructor for Assignment.
     * @param lvalue Expression to be assigned.
     * @param rvalue What to assign the lvalue to.
     */
    Assignment::Assignment(std::shared_ptr<Expression> lvalue, std::shared_ptr<Expression> rvalue):
        lvalue(lvalue),
        rvalue(rvalue)
    {}

    /**
     * Constructor for Assignment, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param lvalue Expression to be assigned.
     * @param rvalue What to assign the lvalue to.
     */
    Assignment::Assignment(Expression * lvalue, Expression * rvalue):
        lvalue(std::shared_ptr<Expression>(lvalue)),
        rvalue(std::shared_ptr<Expression>(rvalue))
    {
    }

    /**
     * Constructor for MacroDef.
     * @param name Name of the macro subroutine.
     * @param blk Contents of the macro.
     */
    MacroDef::MacroDef(std::string name, std::shared_ptr<Block> blk):
        name(name),
        blk(blk)
    {}

    /**
     * Constructor for MacroDef, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param name Name of the macro subroutine. The passed pointer will be
     * free()d by this constructor!
     * @param blk Contents of the macro.
     */
    MacroDef::MacroDef(char * name, Block * blk):
        name(std::string(name)),
        blk(std::shared_ptr<Block>(blk))
    {
        free(name);
    }

    /**
     * Constructor for MacroDef.
     * @param name Name of the macro subroutine.
     * @param blk Contents of the macro.
     * @param src Source parameter names.
     */
    MacroDef::MacroDef(std::string name, std::shared_ptr<Block> blk, std::shared_ptr<IdentifierList> src):
        name(name),
        blk(blk),
        src(src)
    {}

    /**
     * Constructor for MacroDef, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param name Name of the macro subroutine. The passed pointer will be
     * free()d by this constructor!
     * @param blk Contents of the macro.
     * @param src Source parameter names.
     */
    MacroDef::MacroDef(char * name, Block * blk, IdentifierList * src):
        name(std::string(name)),
        blk(std::shared_ptr<Block>(blk)),
        src(std::shared_ptr<IdentifierList>(src))
    {
        free(name);
    }

    /**
     * Constructor for MacroDef.
     * @param name Name of the macro subroutine.
     * @param blk Contents of the macro.
     * @param src Source parameter names.
     * @param dest Destination parameter names.
     */
    MacroDef::MacroDef(std::string name, std::shared_ptr<Block> blk, std::shared_ptr<IdentifierList> src, std::shared_ptr<IdentifierList> dest):
        name(name),
        blk(blk),
        src(src),
        dest(dest)
    {}

    /**
     * Constructor for MacroDef, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param name Name of the macro subroutine. The passed pointer will be
     * free()d by this constructor!
     * @param blk Contents of the macro.
     * @param src Source parameter names.
     * @param dest Destination parameter names.
     */
    MacroDef::MacroDef(char * name, Block * blk, IdentifierList * src, IdentifierList * dest):
        name(std::string(name)),
        blk(std::shared_ptr<Block>(blk)),
        src(std::shared_ptr<IdentifierList>(src)),
        dest(std::shared_ptr<IdentifierList>(dest))
    {
        free(name);
    }

    /**
     * Constructor for MacroFor.
     * @param name Name of the loop control variable.
     * @param indices Indices to iterate over.
     * @param blk Contents of the macro.
     */
    MacroFor::MacroFor(std::string name, std::shared_ptr<IndexList> indices, std::shared_ptr<Block> blk):
        name(name),
        indices(indices),
        blk(blk)
    {}

    /**
     * Constructor for MacroFor, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param name Name of the loop control variable. The passed pointer will be
     * free()d by this constructor!
     * @param indices Indices to iterate over.
     * @param blk Contents of the macro.
     */
    MacroFor::MacroFor(char * name, IndexList * indices, Block * blk):
        name(std::string(name)),
        indices(std::shared_ptr<IndexList>(indices)),
        blk(std::shared_ptr<Block>(blk))
    {
        free(name);
    }

    /**
     * Constructor for MacroIfElse.
     * @param cond Condition expression.
     * @param blk_true Block to insert if the expression is true.
     */
    MacroIfElse::MacroIfElse(std::shared_ptr<Expression> cond, std::shared_ptr<Block> blk_true):
        cond(cond),
        blk_true(blk_true)
    {}

    /**
     * Constructor for MacroIfElse, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param cond Condition expression.
     * @param blk_true Block to insert if the expression is true.
     */
    MacroIfElse::MacroIfElse(Expression * cond, Block * blk_true):
        cond(std::shared_ptr<Expression>(cond)),
        blk_true(std::shared_ptr<Block>(blk_true))
    {
    }

    /**
     * Constructor for MacroIfElse.
     * @param cond Condition expression.
     * @param blk_true Block to insert if the expression is true.
     * @param blk_false Block to insert if the expression is false.
     */
    MacroIfElse::MacroIfElse(std::shared_ptr<Expression> cond, std::shared_ptr<Block> blk_true, std::shared_ptr<Block> blk_false):
        cond(cond),
        blk_true(blk_true),
        blk_false(blk_false)
    {}

    /**
     * Constructor for MacroIfElse, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param cond Condition expression.
     * @param blk_true Block to insert if the expression is true.
     * @param blk_false Block to insert if the expression is false.
     */
    MacroIfElse::MacroIfElse(Expression * cond, Block * blk_true, Block * blk_false):
        cond(std::shared_ptr<Expression>(cond)),
        blk_true(std::shared_ptr<Block>(blk_true)),
        blk_false(std::shared_ptr<Block>(blk_false))
    {
    }

    /**
     * Constructor for Include.
     * @param fname Filename.
     */
    Include::Include(std::shared_ptr<StringLiteral> fname):
        fname(fname)
    {}

    /**
     * Constructor for Include, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param fname Filename.
     */
    Include::Include(StringLiteral * fname):
        fname(std::shared_ptr<StringLiteral>(fname))
    {
    }

    /**
     * Constructor for Subcircuit.
     * @param name Subcircuit name.
     */
    Subcircuit::Subcircuit(std::string name):
        name(name)
    {}

    /**
     * Constructor for Subcircuit, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param name Subcircuit name. The passed pointer will be free()d by this
     * constructor!
     */
    Subcircuit::Subcircuit(char * name):
        name(std::string(name))
    {
        free(name);
    }

    /**
     * Constructor for Subcircuit.
     * @param name Subcircuit name.
     * @param iter Iteration count, or null if not specified.
     */
    Subcircuit::Subcircuit(std::string name, std::shared_ptr<NumericLiteral> iter):
        name(name),
        iter(iter)
    {}

    /**
     * Constructor for Subcircuit, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param name Subcircuit name. The passed pointer will be free()d by this
     * constructor!
     * @param iter Iteration count, or null if not specified.
     */
    Subcircuit::Subcircuit(char * name, NumericLiteral * iter):
        name(std::string(name)),
        iter(std::shared_ptr<NumericLiteral>(iter))
    {
        free(name);
    }

    /**
     * Constructor for Label.
     * @param name Label name.
     */
    Label::Label(std::string name):
        name(name)
    {}

    /**
     * Constructor for Label, intended to be used from YACC only. This version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param name Label name. The passed pointer will be free()d by this
     * constructor!
     */
    Label::Label(char * name):
        name(std::string(name))
    {
        free(name);
    }

    /**
     * Constructor for Bundle.
     */
    Bundle::Bundle() {
    }

    /**
     * Appends to gate vector. Returns reference to this to allow chaining.
     * @param gate Value to push.
     * @return this, to allow chaining.
     */
    Bundle *Bundle::push_gate(std::shared_ptr<Gate> gate) {
        this->gates.push_back(gate);
        return this;
    }

    /**
     * Appends to gate vector. Returns reference to this to allow chaining.
     * @param gate Value to push.
     * @return this, to allow chaining.
     */
    Bundle *Bundle::push_gate(Gate * gate) {
        this->gates.push_back(std::shared_ptr<Gate>(gate));
        return this;
    }

    /**
     * Appends gates vector by another vector. Returns reference to this to
     * allow chaining.
     * @param gates Vector to push.
     * @return this, to allow chaining.
     */
    Bundle *Bundle::push_gates(std::vector<std::shared_ptr<Gate>> & gates) {
        this->gates.insert(this->gates.end(), gates.begin(), gates.end());
        return this;
    }

    /**
     * Constructor for ErroneousStatement.
     */
    ErroneousStatement::ErroneousStatement() {
    }

    /**
     * Constructor for Block.
     */
    Block::Block() {
    }

    /**
     * Appends to stmt vector. Returns reference to this to allow chaining.
     * @param stmt Value to push.
     * @return this, to allow chaining.
     */
    Block *Block::push_stmt(std::shared_ptr<Statement> stmt) {
        this->stmts.push_back(stmt);
        return this;
    }

    /**
     * Appends to stmt vector. Returns reference to this to allow chaining.
     * @param stmt Value to push.
     * @return this, to allow chaining.
     */
    Block *Block::push_stmt(Statement * stmt) {
        this->stmts.push_back(std::shared_ptr<Statement>(stmt));
        return this;
    }

    /**
     * Constructor for Program.
     * @param version Version string.
     * @param blk Block of code representing the program.
     */
    Program::Program(std::string version, std::shared_ptr<Block> blk):
        version(version),
        blk(blk)
    {}

    /**
     * Constructor for Program, intended to be used from YACC only. This
     * version:
     *  - uses char* for strings and bare pointers instead of std::shared_ptr<>
     * encapsulations for inputs;
     *  - calls free() on strings passed to it after constructing std::string
     * instances.
     * @param version Version string. The passed pointer will be free()d by this
     * constructor!
     * @param blk Block of code representing the program.
     */
    Program::Program(char * version, Block * blk):
        version(std::string(version)),
        blk(std::shared_ptr<Block>(blk))
    {
        free(version);
    }

}}

