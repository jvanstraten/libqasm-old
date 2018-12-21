#ifndef _LIBCQASM2_AST_GEN_H_
#define _LIBCQASM2_AST_GEN_H_

#include <cstdlib>
#include <cstdint>
#include <string>
#include <sstream>
#include <vector>

#include "node.hpp"

using namespace cqasm2;

namespace cqasm2 { namespace ast {








    /*
     * Forward declarations for all classes defined here.
     */
    class Type;                          // Abstract, use derived classes.
    class TypeLiteral;                   // Abstract, use derived classes.
    class QubitType;
    class NumericType;
    class FloatType;
    class DoubleType;
    class Operand;                       // Abstract, use derived classes.
    class Expression;                    // Abstract, use derived classes.
    class NumericLiteral;                // Abstract, use derived classes.
    class DecLiteral;
    class HexLiteral;
    class BinLiteral;
    class FloatLiteral;
    class NamedLiteral;
    class Identifier;
    class ResourceRef;
    class MacroParamRef;
    class MacroIterRef;
    class LabelRef;
    class Subscript;
    class Indexation;
    class Operation;
    class TypeCast;
    class ShiftCast;
    class ErroneousExpression;
    class MatrixLiteral;                 // Abstract, use derived classes.
    class MatrixLiteral1;
    class MatrixLiteral2;
    class StringLiteral;
    class JsonLiteral;
    class ExpressionList;
    class OperandList;
    class IdentifierList;
    class IndexEntry;
    class IndexList;
    class StringBuilder;
    class AnnotationData;
    class GateType;
    class Gate;                          // Abstract, use derived classes.
    class UnresolvedGate;                // Abstract, use derived classes.
    class NormalGate;
    class IfGoto;
    class MacroCall;
    class Statement;                     // Abstract, use derived classes.
    class Pragma;
    class Resource;                      // Abstract, use derived classes.
    class QubitRegister1;
    class ScalarResource;
    class ArrayResource;
    class LetStatement;
    class Mapping;
    class Assignment;
    class MacroDef;
    class MacroFor;
    class MacroIfElse;
    class Include;
    class Subcircuit;
    class Label;
    class Bundle;
    class ErroneousStatement;
    class Block;
    class Program;

    /**
     * Represents any resource type token, such as "uint<64>".
     */
    class Type : public Node {
    public:

        /**
         * Constructor for Type.
         */
        Type();

        /**
         * Default destructor for Type.
         */
        virtual ~Type() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;

    };

    /**
     * Represents a type AST node, before constant folding to figure out the
     * actual type used by the typesystem.
     */
    class TypeLiteral : public Type {
    public:

        /**
         * Constructor for TypeLiteral.
         */
        TypeLiteral();

        /**
         * Default destructor for TypeLiteral.
         */
        virtual ~TypeLiteral() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;

    };

    /**
     * Represents a qubit type token, i.e. "qubit".
     */
    class QubitType : public TypeLiteral {
    public:

        /**
         * Constructor for QubitType.
         */
        QubitType();

        /**
         * Default destructor for QubitType.
         */
        virtual ~QubitType() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a numeric type token, such as "uint<64>".
     */
    class NumericType : public TypeLiteral {
    public:

        /**
         * Whether this is a signed or unsigned type.
         */
        bool has_sign;

        /**
         * Number of bits before the decimal separator.
         */
        std::shared_ptr<Expression> ibits;

        /**
         * Number of bits after the decimal separator.
         */
        std::shared_ptr<Expression> fbits;

        /**
         * Constructor for NumericType.
         * @param has_sign Whether this is a signed or unsigned type.
         */
        NumericType(bool has_sign);

        /**
         * Constructor for NumericType.
         * @param has_sign Whether this is a signed or unsigned type.
         * @param ibits Number of bits before the decimal separator.
         */
        NumericType(bool has_sign, std::shared_ptr<Expression> ibits);

        /**
         * Constructor for NumericType, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param has_sign Whether this is a signed or unsigned type.
         * @param ibits Number of bits before the decimal separator.
         */
        NumericType(bool has_sign, Expression * ibits);

        /**
         * Constructor for NumericType.
         * @param has_sign Whether this is a signed or unsigned type.
         * @param ibits Number of bits before the decimal separator.
         * @param fbits Number of bits after the decimal separator.
         */
        NumericType(bool has_sign, std::shared_ptr<Expression> ibits, std::shared_ptr<Expression> fbits);

        /**
         * Constructor for NumericType, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param has_sign Whether this is a signed or unsigned type.
         * @param ibits Number of bits before the decimal separator.
         * @param fbits Number of bits after the decimal separator.
         */
        NumericType(bool has_sign, Expression * ibits, Expression * fbits);

        /**
         * Default destructor for NumericType.
         */
        virtual ~NumericType() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a float type token, i.e. "float".
     */
    class FloatType : public TypeLiteral {
    public:

        /**
         * Constructor for FloatType.
         */
        FloatType();

        /**
         * Default destructor for FloatType.
         */
        virtual ~FloatType() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a double type token, i.e. "double".
     */
    class DoubleType : public TypeLiteral {
    public:

        /**
         * Constructor for DoubleType.
         */
        DoubleType();

        /**
         * Default destructor for DoubleType.
         */
        virtual ~DoubleType() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents any operand.
     */
    class Operand : public Node {
    public:

        /**
         * Constructor for Operand.
         */
        Operand();

        /**
         * Default destructor for Operand.
         */
        virtual ~Operand() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;

    };

    /**
     * Represents any expression.
     */
    class Expression : public Operand {
    public:

        /**
         * Constructor for Expression.
         */
        Expression();

        /**
         * Default destructor for Expression.
         */
        virtual ~Expression() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;

    };

    /**
     * Represents unparsed numeric literals.
     */
    class NumericLiteral : public Expression {
    public:

        /**
         * Constructor for NumericLiteral.
         */
        NumericLiteral();

        /**
         * Default destructor for NumericLiteral.
         */
        virtual ~NumericLiteral() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;

    };

    /**
     * Represents a number in decimal integer notation.
     */
    class DecLiteral : public NumericLiteral {
    public:

        /**
         * The value as it appears in the source file.
         */
        std::string val;

        /**
         * Constructor for DecLiteral.
         * @param val The value as it appears in the source file.
         */
        DecLiteral(std::string val);

        /**
         * Constructor for DecLiteral, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param val The value as it appears in the source file. The passed
         * pointer will be free()d by this constructor!
         */
        DecLiteral(char * val);

        /**
         * Default destructor for DecLiteral.
         */
        virtual ~DecLiteral() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a number in hexadecimal notation.
     */
    class HexLiteral : public NumericLiteral {
    public:

        /**
         * The value as it appears in the source file.
         */
        std::string val;

        /**
         * Constructor for HexLiteral.
         * @param val The value as it appears in the source file.
         */
        HexLiteral(std::string val);

        /**
         * Constructor for HexLiteral, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param val The value as it appears in the source file. The passed
         * pointer will be free()d by this constructor!
         */
        HexLiteral(char * val);

        /**
         * Default destructor for HexLiteral.
         */
        virtual ~HexLiteral() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a number in binary notation.
     */
    class BinLiteral : public NumericLiteral {
    public:

        /**
         * The value as it appears in the source file.
         */
        std::string val;

        /**
         * Constructor for BinLiteral.
         * @param val The value as it appears in the source file.
         */
        BinLiteral(std::string val);

        /**
         * Constructor for BinLiteral, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param val The value as it appears in the source file. The passed
         * pointer will be free()d by this constructor!
         */
        BinLiteral(char * val);

        /**
         * Default destructor for BinLiteral.
         */
        virtual ~BinLiteral() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a number in decimal float notation.
     */
    class FloatLiteral : public NumericLiteral {
    public:

        /**
         * The value as it appears in the source file.
         */
        std::string val;

        /**
         * Constructor for FloatLiteral.
         * @param val The value as it appears in the source file.
         */
        FloatLiteral(std::string val);

        /**
         * Constructor for FloatLiteral, intended to be used from YACC only.
         * This version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param val The value as it appears in the source file. The passed
         * pointer will be free()d by this constructor!
         */
        FloatLiteral(char * val);

        /**
         * Default destructor for FloatLiteral.
         */
        virtual ~FloatLiteral() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a number by means of a named constant.
     */
    class NamedLiteral : public NumericLiteral {
    public:

        /**
         * The name of the constant.
         */
        std::string name;

        /**
         * Constructor for NamedLiteral.
         * @param name The name of the constant.
         */
        NamedLiteral(std::string name);

        /**
         * Constructor for NamedLiteral, intended to be used from YACC only.
         * This version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param name The name of the constant. The passed pointer will be
         * free()d by this constructor!
         */
        NamedLiteral(char * name);

        /**
         * Default destructor for NamedLiteral.
         */
        virtual ~NamedLiteral() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents any identifier in an expression. This can ultimately resolve
     * to almost anything (like a label) or not be resolved at all, at the
     * discretion of the context which the expression is used in.
     */
    class Identifier : public Expression {
    public:

        /**
         * Identifier.
         */
        std::string name;

        /**
         * Constructor for Identifier.
         * @param name Identifier.
         */
        Identifier(std::string name);

        /**
         * Constructor for Identifier, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param name Identifier. The passed pointer will be free()d by this
         * constructor!
         */
        Identifier(char * name);

        /**
         * Default destructor for Identifier.
         */
        virtual ~Identifier() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents an identifier that was resolved to a resource. When printing,
     * the uniquified name of the resource is used.
     */
    class ResourceRef : public Expression {
    public:

        /**
         * Resolved resource.
         */
        std::shared_ptr<Resource> res;

        /**
         * Original identifier that was used for this reference, if any.
         */
        std::shared_ptr<Identifier> id;

        /**
         * Constructor for ResourceRef.
         * @param res Resolved resource.
         */
        ResourceRef(std::shared_ptr<Resource> res);

        /**
         * Constructor for ResourceRef, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param res Resolved resource.
         */
        ResourceRef(Resource * res);

        /**
         * Constructor for ResourceRef.
         * @param res Resolved resource.
         * @param id Original identifier that was used for this reference, if
         * any.
         */
        ResourceRef(std::shared_ptr<Resource> res, std::shared_ptr<Identifier> id);

        /**
         * Constructor for ResourceRef, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param res Resolved resource.
         * @param id Original identifier that was used for this reference, if
         * any.
         */
        ResourceRef(Resource * res, Identifier * id);

        /**
         * Default destructor for ResourceRef.
         */
        virtual ~ResourceRef() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents an identifier that was resolved to one of the parameters of a
     * macro def statement.
     */
    class MacroParamRef : public Expression {
    public:

        /**
         * Macro definition this parameter was resolved to.
         */
        std::shared_ptr<MacroDef> macro;

        /**
         * Parameter index.
         */
        int param;

        /**
         * Original identifier that was used for this reference, if any.
         */
        std::shared_ptr<Identifier> id;

        /**
         * Constructor for MacroParamRef.
         * @param macro Macro definition this parameter was resolved to.
         * @param param Parameter index.
         */
        MacroParamRef(std::shared_ptr<MacroDef> macro, int param);

        /**
         * Constructor for MacroParamRef, intended to be used from YACC only.
         * This version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param macro Macro definition this parameter was resolved to.
         * @param param Parameter index.
         */
        MacroParamRef(MacroDef * macro, int param);

        /**
         * Constructor for MacroParamRef.
         * @param macro Macro definition this parameter was resolved to.
         * @param param Parameter index.
         * @param id Original identifier that was used for this reference, if
         * any.
         */
        MacroParamRef(std::shared_ptr<MacroDef> macro, int param, std::shared_ptr<Identifier> id);

        /**
         * Constructor for MacroParamRef, intended to be used from YACC only.
         * This version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param macro Macro definition this parameter was resolved to.
         * @param param Parameter index.
         * @param id Original identifier that was used for this reference, if
         * any.
         */
        MacroParamRef(MacroDef * macro, int param, Identifier * id);

        /**
         * Default destructor for MacroParamRef.
         */
        virtual ~MacroParamRef() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents an identifier that was resolved to the loop variable of a
     * macro for statement.
     */
    class MacroIterRef : public Expression {
    public:

        /**
         * For loop that this parameter was resolved to.
         */
        std::shared_ptr<MacroFor> macro;

        /**
         * Original identifier that was used for this reference, if any.
         */
        std::shared_ptr<Identifier> id;

        /**
         * Constructor for MacroIterRef.
         * @param macro For loop that this parameter was resolved to.
         */
        MacroIterRef(std::shared_ptr<MacroFor> macro);

        /**
         * Constructor for MacroIterRef, intended to be used from YACC only.
         * This version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param macro For loop that this parameter was resolved to.
         */
        MacroIterRef(MacroFor * macro);

        /**
         * Constructor for MacroIterRef.
         * @param macro For loop that this parameter was resolved to.
         * @param id Original identifier that was used for this reference, if
         * any.
         */
        MacroIterRef(std::shared_ptr<MacroFor> macro, std::shared_ptr<Identifier> id);

        /**
         * Constructor for MacroIterRef, intended to be used from YACC only.
         * This version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param macro For loop that this parameter was resolved to.
         * @param id Original identifier that was used for this reference, if
         * any.
         */
        MacroIterRef(MacroFor * macro, Identifier * id);

        /**
         * Default destructor for MacroIterRef.
         */
        virtual ~MacroIterRef() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents an identifier that was resolved to a label.
     */
    class LabelRef : public Expression {
    public:

        /**
         * Label that this identifier was resolved to.
         */
        std::shared_ptr<Label> lbl;

        /**
         * Original identifier that was used for this reference, if any.
         */
        std::shared_ptr<Identifier> id;

        /**
         * Constructor for LabelRef.
         * @param lbl Label that this identifier was resolved to.
         */
        LabelRef(std::shared_ptr<Label> lbl);

        /**
         * Constructor for LabelRef, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param lbl Label that this identifier was resolved to.
         */
        LabelRef(Label * lbl);

        /**
         * Constructor for LabelRef.
         * @param lbl Label that this identifier was resolved to.
         * @param id Original identifier that was used for this reference, if
         * any.
         */
        LabelRef(std::shared_ptr<Label> lbl, std::shared_ptr<Identifier> id);

        /**
         * Constructor for LabelRef, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param lbl Label that this identifier was resolved to.
         * @param id Original identifier that was used for this reference, if
         * any.
         */
        LabelRef(Label * lbl, Identifier * id);

        /**
         * Default destructor for LabelRef.
         */
        virtual ~LabelRef() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a disambiguation subscript. This is used for qubits, to select
     * between the qubit itself and its measurement register in contexts where
     * both are allowed.
     */
    class Subscript : public Expression {
    public:

        /**
         * Expression to disambiguate with subscript notation.
         */
        std::shared_ptr<Expression> expr;

        /**
         * The subscript text.
         */
        std::string subscript;

        /**
         * Constructor for Subscript.
         * @param expr Expression to disambiguate with subscript notation.
         * @param subscript The subscript text.
         */
        Subscript(std::shared_ptr<Expression> expr, std::string subscript);

        /**
         * Constructor for Subscript, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param expr Expression to disambiguate with subscript notation.
         * @param subscript The subscript text. The passed pointer will be
         * free()d by this constructor!
         */
        Subscript(Expression * expr, char * subscript);

        /**
         * Default destructor for Subscript.
         */
        virtual ~Subscript() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents an index notation, i.e. of the form "a[b]".
     */
    class Indexation : public Expression {
    public:

        /**
         * Expression to index.
         */
        std::shared_ptr<Expression> expr;

        /**
         * List of index entries to index by.
         */
        std::shared_ptr<IndexList> indices;

        /**
         * Constructor for Indexation.
         * @param expr Expression to index.
         * @param indices List of index entries to index by.
         */
        Indexation(std::shared_ptr<Expression> expr, std::shared_ptr<IndexList> indices);

        /**
         * Constructor for Indexation, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param expr Expression to index.
         * @param indices List of index entries to index by.
         */
        Indexation(Expression * expr, IndexList * indices);

        /**
         * Default destructor for Indexation.
         */
        virtual ~Indexation() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents any kind of operation.
     */
    class Operation : public Expression {
    public:

        /**
         * Whether this is a function call (true) or an inline operator (false).
         */
        bool fun;

        /**
         * Operator or function name.
         */
        std::string oper;

        /**
         * Operand list.
         */
        std::shared_ptr<ExpressionList> ops;

        /**
         * Constructor for Operation.
         * @param fun Whether this is a function call (true) or an inline
         * operator (false).
         * @param oper Operator or function name.
         * @param ops Operand list.
         */
        Operation(bool fun, std::string oper, std::shared_ptr<ExpressionList> ops);

        /**
         * Constructor for Operation, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param fun Whether this is a function call (true) or an inline
         * operator (false).
         * @param oper Operator or function name. The passed pointer will be
         * free()d by this constructor!
         * @param ops Operand list.
         */
        Operation(bool fun, char * oper, ExpressionList * ops);

        /**
         * Default destructor for Operation.
         */
        virtual ~Operation() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a typecast expression.
     */
    class TypeCast : public Expression {
    public:

        /**
         * Type to cast to.
         */
        std::shared_ptr<Type> typ;

        /**
         * Expression to typecast.
         */
        std::shared_ptr<Expression> expr;

        /**
         * Constructor for TypeCast.
         * @param typ Type to cast to.
         * @param expr Expression to typecast.
         */
        TypeCast(std::shared_ptr<Type> typ, std::shared_ptr<Expression> expr);

        /**
         * Constructor for TypeCast, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param typ Type to cast to.
         * @param expr Expression to typecast.
         */
        TypeCast(Type * typ, Expression * expr);

        /**
         * Default destructor for TypeCast.
         */
        virtual ~TypeCast() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a shift-decimal-point typecast expression.
     */
    class ShiftCast : public Expression {
    public:

        /**
         * False: (&gt;&gt;a)b = shift decimal point left; true: (&lt;&lt;a)b =
         * shift decimal point right.
         */
        bool dir;

        /**
         * Shift amount.
         */
        std::shared_ptr<Expression> shamt;

        /**
         * Expression to typecast.
         */
        std::shared_ptr<Expression> expr;

        /**
         * Constructor for ShiftCast.
         * @param dir False: (&gt;&gt;a)b = shift decimal point left; true:
         * (&lt;&lt;a)b = shift decimal point right.
         * @param shamt Shift amount.
         * @param expr Expression to typecast.
         */
        ShiftCast(bool dir, std::shared_ptr<Expression> shamt, std::shared_ptr<Expression> expr);

        /**
         * Constructor for ShiftCast, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param dir False: (&gt;&gt;a)b = shift decimal point left; true:
         * (&lt;&lt;a)b = shift decimal point right.
         * @param shamt Shift amount.
         * @param expr Expression to typecast.
         */
        ShiftCast(bool dir, Expression * shamt, Expression * expr);

        /**
         * Default destructor for ShiftCast.
         */
        virtual ~ShiftCast() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents an erroneous expression. This is inserted by the parser to
     * recover from errors.
     */
    class ErroneousExpression : public Expression {
    public:

        /**
         * Constructor for ErroneousExpression.
         */
        ErroneousExpression();

        /**
         * Default destructor for ErroneousExpression.
         */
        virtual ~ErroneousExpression() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a matrix literal.
     */
    class MatrixLiteral : public Operand {
    public:

        /**
         * Constructor for MatrixLiteral.
         */
        MatrixLiteral();

        /**
         * Default destructor for MatrixLiteral.
         */
        virtual ~MatrixLiteral() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;

    };

    /**
     * cQASM 1.0 notation for a matrix.
     */
    class MatrixLiteral1 : public MatrixLiteral {
    public:

        /**
         * Represents the data contained in this matrix. Each expression is
         * alternatingly the real resp. imaginary part of the matrix. The matrix
         * must furthermore be square.
         */
        std::shared_ptr<ExpressionList> data;

        /**
         * Constructor for MatrixLiteral1.
         * @param data Represents the data contained in this matrix. Each
         * expression is alternatingly the real resp. imaginary part of the
         * matrix. The matrix must furthermore be square.
         */
        MatrixLiteral1(std::shared_ptr<ExpressionList> data);

        /**
         * Constructor for MatrixLiteral1, intended to be used from YACC only.
         * This version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param data Represents the data contained in this matrix. Each
         * expression is alternatingly the real resp. imaginary part of the
         * matrix. The matrix must furthermore be square.
         */
        MatrixLiteral1(ExpressionList * data);

        /**
         * Default destructor for MatrixLiteral1.
         */
        virtual ~MatrixLiteral1() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * cQASM 2.0 notation for a matrix.
     */
    class MatrixLiteral2 : public MatrixLiteral {
    public:

        /**
         * List of rows. The index into the enclosed ExpressionList is the
         * column index, and each individual Expression is cast to a complex
         * number.
         */
        std::vector<std::shared_ptr<ExpressionList>> rows;

        /**
         * Constructor for MatrixLiteral2.
         */
        MatrixLiteral2();

        /**
         * Default destructor for MatrixLiteral2.
         */
        virtual ~MatrixLiteral2() = default;

        /**
         * Appends to row vector. Returns reference to this to allow chaining.
         * @param row Value to push.
         * @return this, to allow chaining.
         */
        MatrixLiteral2 *push_row(std::shared_ptr<ExpressionList> row);

        /**
         * Appends to row vector. Returns reference to this to allow chaining.
         * @param row Value to push.
         * @return this, to allow chaining.
         */
        MatrixLiteral2 *push_row(ExpressionList * row);

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a string literal.
     */
    class StringLiteral : public Operand {
    public:

        /**
         * Contained string literal.
         */
        std::string data;

        /**
         * Constructor for StringLiteral.
         * @param data Contained string literal.
         */
        StringLiteral(std::string data);

        /**
         * Constructor for StringLiteral, intended to be used from YACC only.
         * This version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param data Contained string literal. The passed pointer will be
         * free()d by this constructor!
         */
        StringLiteral(char * data);

        /**
         * Default destructor for StringLiteral.
         */
        virtual ~StringLiteral() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a JSON literal.
     */
    class JsonLiteral : public Operand {
    public:

        /**
         * Contained JSON string.
         */
        std::string data;

        /**
         * Constructor for JsonLiteral.
         * @param data Contained JSON string.
         */
        JsonLiteral(std::string data);

        /**
         * Constructor for JsonLiteral, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param data Contained JSON string. The passed pointer will be free()d
         * by this constructor!
         */
        JsonLiteral(char * data);

        /**
         * Default destructor for JsonLiteral.
         */
        virtual ~JsonLiteral() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a list of expressions.
     */
    class ExpressionList : public Node {
    public:

        /**
         * Vector of all the expressions in this list.
         */
        std::vector<std::shared_ptr<Expression>> exprs;

        /**
         * Constructor for ExpressionList.
         */
        ExpressionList();

        /**
         * Default destructor for ExpressionList.
         */
        virtual ~ExpressionList() = default;

        /**
         * Appends to expr vector. Returns reference to this to allow chaining.
         * @param expr Value to push.
         * @return this, to allow chaining.
         */
        ExpressionList *push_expr(std::shared_ptr<Expression> expr);

        /**
         * Appends to expr vector. Returns reference to this to allow chaining.
         * @param expr Value to push.
         * @return this, to allow chaining.
         */
        ExpressionList *push_expr(Expression * expr);

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a list of operands.
     */
    class OperandList : public Node {
    public:

        /**
         * Vector of all the operands in this list.
         */
        std::vector<std::shared_ptr<Operand>> opers;

        /**
         * Constructor for OperandList.
         */
        OperandList();

        /**
         * Default destructor for OperandList.
         */
        virtual ~OperandList() = default;

        /**
         * Appends to oper vector. Returns reference to this to allow chaining.
         * @param oper Value to push.
         * @return this, to allow chaining.
         */
        OperandList *push_oper(std::shared_ptr<Operand> oper);

        /**
         * Appends to oper vector. Returns reference to this to allow chaining.
         * @param oper Value to push.
         * @return this, to allow chaining.
         */
        OperandList *push_oper(Operand * oper);

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a list of identifiers.
     */
    class IdentifierList : public Node {
    public:

        /**
         * Vector of all the identifier strings in this list.
         */
        std::vector<std::string> ids;

        /**
         * Constructor for IdentifierList.
         */
        IdentifierList();

        /**
         * Default destructor for IdentifierList.
         */
        virtual ~IdentifierList() = default;

        /**
         * Appends to id vector. Returns reference to this to allow chaining.
         * @param id Value to push.
         * @return this, to allow chaining.
         */
        IdentifierList *push_id(std::string id);

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents either a singular entry or a range within a list of indices.
     */
    class IndexEntry : public Node {
    public:

        /**
         * First index in this range, or the selected index if singular.
         */
        std::shared_ptr<Expression> first;

        /**
         * Last index in this range, or null if singular.
         */
        std::shared_ptr<Expression> last;

        /**
         * Constructor for IndexEntry.
         * @param first First index in this range, or the selected index if
         * singular.
         */
        IndexEntry(std::shared_ptr<Expression> first);

        /**
         * Constructor for IndexEntry, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param first First index in this range, or the selected index if
         * singular.
         */
        IndexEntry(Expression * first);

        /**
         * Constructor for IndexEntry.
         * @param first First index in this range, or the selected index if
         * singular.
         * @param last Last index in this range, or null if singular.
         */
        IndexEntry(std::shared_ptr<Expression> first, std::shared_ptr<Expression> last);

        /**
         * Constructor for IndexEntry, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param first First index in this range, or the selected index if
         * singular.
         * @param last Last index in this range, or null if singular.
         */
        IndexEntry(Expression * first, Expression * last);

        /**
         * Default destructor for IndexEntry.
         */
        virtual ~IndexEntry() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a list of indices.
     */
    class IndexList : public Node {
    public:

        /**
         * Vector of all the expressions in this list.
         */
        std::vector<std::shared_ptr<IndexEntry>> ents;

        /**
         * Constructor for IndexList.
         */
        IndexList();

        /**
         * Default destructor for IndexList.
         */
        virtual ~IndexList() = default;

        /**
         * Appends to ent vector. Returns reference to this to allow chaining.
         * @param ent Value to push.
         * @return this, to allow chaining.
         */
        IndexList *push_ent(std::shared_ptr<IndexEntry> ent);

        /**
         * Appends to ent vector. Returns reference to this to allow chaining.
         * @param ent Value to push.
         * @return this, to allow chaining.
         */
        IndexList *push_ent(IndexEntry * ent);

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents the contents of a string or JSON literal as its being parsed.
     */
    class StringBuilder : public Node {
    public:

        /**
         * String builder to append to.
         */
        std::ostringstream os;

        /**
         * Constructor for StringBuilder.
         */
        StringBuilder();

        /**
         * Default destructor for StringBuilder.
         */
        virtual ~StringBuilder() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents the data contained in an annotation or pragma.
     */
    class AnnotationData : public Node {
    public:

        /**
         * Name of the target that this pragma/annotation is intended for.
         */
        std::string target;

        /**
         * Name of the pragma/annotation.
         */
        std::string name;

        /**
         * List of operands, if specified, that are attached to the
         * annotation/pragma.
         */
        std::shared_ptr<OperandList> ops;

        /**
         * Constructor for AnnotationData.
         * @param target Name of the target that this pragma/annotation is
         * intended for.
         * @param name Name of the pragma/annotation.
         */
        AnnotationData(std::string target, std::string name);

        /**
         * Constructor for AnnotationData, intended to be used from YACC only.
         * This version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param target Name of the target that this pragma/annotation is
         * intended for. The passed pointer will be free()d by this constructor!
         * @param name Name of the pragma/annotation. The passed pointer will be
         * free()d by this constructor!
         */
        AnnotationData(char * target, char * name);

        /**
         * Constructor for AnnotationData.
         * @param target Name of the target that this pragma/annotation is
         * intended for.
         * @param name Name of the pragma/annotation.
         * @param ops List of operands, if specified, that are attached to the
         * annotation/pragma.
         */
        AnnotationData(std::string target, std::string name, std::shared_ptr<OperandList> ops);

        /**
         * Constructor for AnnotationData, intended to be used from YACC only.
         * This version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param target Name of the target that this pragma/annotation is
         * intended for. The passed pointer will be free()d by this constructor!
         * @param name Name of the pragma/annotation. The passed pointer will be
         * free()d by this constructor!
         * @param ops List of operands, if specified, that are attached to the
         * annotation/pragma.
         */
        AnnotationData(char * target, char * name, OperandList * ops);

        /**
         * Default destructor for AnnotationData.
         */
        virtual ~AnnotationData() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents the name of a gate and its conditions for execution.
     */
    class GateType : public Node {
    public:

        /**
         * Name of the gate.
         */
        std::string name;

        /**
         * Conditional expressions.
         */
        std::vector<std::shared_ptr<Expression>> conds;

        /**
         * Constructor for GateType.
         * @param name Name of the gate.
         */
        GateType(std::string name);

        /**
         * Constructor for GateType, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param name Name of the gate. The passed pointer will be free()d by
         * this constructor!
         */
        GateType(char * name);

        /**
         * Default destructor for GateType.
         */
        virtual ~GateType() = default;

        /**
         * Appends to cond vector. Returns reference to this to allow chaining.
         * @param cond Value to push.
         * @return this, to allow chaining.
         */
        GateType *push_cond(std::shared_ptr<Expression> cond);

        /**
         * Appends to cond vector. Returns reference to this to allow chaining.
         * @param cond Value to push.
         * @return this, to allow chaining.
         */
        GateType *push_cond(Expression * cond);

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a gate that may or may not have been resolved.
     */
    class Gate : public Node {
    public:

        /**
         * Constructor for Gate.
         */
        Gate();

        /**
         * Default destructor for Gate.
         */
        virtual ~Gate() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;

    };

    /**
     * Represents an unresolved gate, including annotations.
     */
    class UnresolvedGate : public Gate {
    public:

        /**
         * Gate annotations.
         */
        std::vector<std::shared_ptr<AnnotationData>> annots;

        /**
         * Constructor for UnresolvedGate.
         */
        UnresolvedGate();

        /**
         * Default destructor for UnresolvedGate.
         */
        virtual ~UnresolvedGate() = default;

        /**
         * Appends to annot vector. Returns reference to this to allow chaining.
         * @param annot Value to push.
         * @return this, to allow chaining.
         */
        UnresolvedGate *push_annot(std::shared_ptr<AnnotationData> annot);

        /**
         * Appends to annot vector. Returns reference to this to allow chaining.
         * @param annot Value to push.
         * @return this, to allow chaining.
         */
        UnresolvedGate *push_annot(AnnotationData * annot);

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;

    };

    /**
     * Represents a normal gate, expressed as an instruction.
     */
    class NormalGate : public UnresolvedGate {
    public:

        /**
         * Contains the name of the gate and its conditional expressions.
         */
        std::shared_ptr<GateType> typ;

        /**
         * Source operand list.
         */
        std::shared_ptr<OperandList> src;

        /**
         * Destination operand list.
         */
        std::shared_ptr<OperandList> dest;

        /**
         * Constructor for NormalGate.
         * @param typ Contains the name of the gate and its conditional
         * expressions.
         */
        NormalGate(std::shared_ptr<GateType> typ);

        /**
         * Constructor for NormalGate, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param typ Contains the name of the gate and its conditional
         * expressions.
         */
        NormalGate(GateType * typ);

        /**
         * Constructor for NormalGate.
         * @param typ Contains the name of the gate and its conditional
         * expressions.
         * @param src Source operand list.
         */
        NormalGate(std::shared_ptr<GateType> typ, std::shared_ptr<OperandList> src);

        /**
         * Constructor for NormalGate, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param typ Contains the name of the gate and its conditional
         * expressions.
         * @param src Source operand list.
         */
        NormalGate(GateType * typ, OperandList * src);

        /**
         * Constructor for NormalGate.
         * @param typ Contains the name of the gate and its conditional
         * expressions.
         * @param src Source operand list.
         * @param dest Destination operand list.
         */
        NormalGate(std::shared_ptr<GateType> typ, std::shared_ptr<OperandList> src, std::shared_ptr<OperandList> dest);

        /**
         * Constructor for NormalGate, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param typ Contains the name of the gate and its conditional
         * expressions.
         * @param src Source operand list.
         * @param dest Destination operand list.
         */
        NormalGate(GateType * typ, OperandList * src, OperandList * dest);

        /**
         * Default destructor for NormalGate.
         */
        virtual ~NormalGate() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents the syntactic sugar if-goto notation for branches.
     */
    class IfGoto : public UnresolvedGate {
    public:

        /**
         * Name of the label to jump to.
         */
        std::string lbl;

        /**
         * The condition for jumping, or null for always.
         */
        std::shared_ptr<Expression> expr;

        /**
         * Constructor for IfGoto.
         * @param lbl Name of the label to jump to.
         */
        IfGoto(std::string lbl);

        /**
         * Constructor for IfGoto, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param lbl Name of the label to jump to. The passed pointer will be
         * free()d by this constructor!
         */
        IfGoto(char * lbl);

        /**
         * Constructor for IfGoto.
         * @param lbl Name of the label to jump to.
         * @param expr The condition for jumping, or null for always.
         */
        IfGoto(std::string lbl, std::shared_ptr<Expression> expr);

        /**
         * Constructor for IfGoto, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param lbl Name of the label to jump to. The passed pointer will be
         * free()d by this constructor!
         * @param expr The condition for jumping, or null for always.
         */
        IfGoto(char * lbl, Expression * expr);

        /**
         * Default destructor for IfGoto.
         */
        virtual ~IfGoto() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a call to a macro.
     */
    class MacroCall : public Gate {
    public:

        /**
         * Gate description from the source code.
         */
        std::shared_ptr<NormalGate> gate;

        /**
         * Macro that this gate was resolved to.
         */
        std::shared_ptr<MacroDef> macro;

        /**
         * Constructor for MacroCall.
         * @param gate Gate description from the source code.
         * @param macro Macro that this gate was resolved to.
         */
        MacroCall(std::shared_ptr<NormalGate> gate, std::shared_ptr<MacroDef> macro);

        /**
         * Constructor for MacroCall, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param gate Gate description from the source code.
         * @param macro Macro that this gate was resolved to.
         */
        MacroCall(NormalGate * gate, MacroDef * macro);

        /**
         * Default destructor for MacroCall.
         */
        virtual ~MacroCall() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a statement.
     */
    class Statement : public Node {
    public:

        /**
         * Statement annotations.
         */
        std::vector<std::shared_ptr<AnnotationData>> annots;

        /**
         * Constructor for Statement.
         */
        Statement();

        /**
         * Default destructor for Statement.
         */
        virtual ~Statement() = default;

        /**
         * Appends to annot vector. Returns reference to this to allow chaining.
         * @param annot Value to push.
         * @return this, to allow chaining.
         */
        Statement *push_annot(std::shared_ptr<AnnotationData> annot);

        /**
         * Appends to annot vector. Returns reference to this to allow chaining.
         * @param annot Value to push.
         * @return this, to allow chaining.
         */
        Statement *push_annot(AnnotationData * annot);

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;

    };

    /**
     * Represents a pragma statement.
     */
    class Pragma : public Statement {
    public:

        /**
         * Data belonging to this pragma statement.
         */
        std::shared_ptr<AnnotationData> data;

        /**
         * Constructor for Pragma.
         * @param data Data belonging to this pragma statement.
         */
        Pragma(std::shared_ptr<AnnotationData> data);

        /**
         * Constructor for Pragma, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param data Data belonging to this pragma statement.
         */
        Pragma(AnnotationData * data);

        /**
         * Default destructor for Pragma.
         */
        virtual ~Pragma() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a statement that declares a resource.
     */
    class Resource : public Statement {
    public:

        /**
         * Uniquified name.
         */
        std::string unique;

        /**
         * Constructor for Resource.
         */
        Resource();

        /**
         * Default destructor for Resource.
         */
        virtual ~Resource() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;

    };

    /**
     * Represents cQASM 1.0's qubit resource notation.
     */
    class QubitRegister1 : public Resource {
    public:

        /**
         * Number of qubits.
         */
        std::shared_ptr<NumericLiteral> nqubits;

        /**
         * Constructor for QubitRegister1.
         * @param nqubits Number of qubits.
         */
        QubitRegister1(std::shared_ptr<NumericLiteral> nqubits);

        /**
         * Constructor for QubitRegister1, intended to be used from YACC only.
         * This version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param nqubits Number of qubits.
         */
        QubitRegister1(NumericLiteral * nqubits);

        /**
         * Default destructor for QubitRegister1.
         */
        virtual ~QubitRegister1() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a scalar resource declaration.
     */
    class ScalarResource : public Resource {
    public:

        /**
         * Type of the resource.
         */
        std::shared_ptr<Type> typ;

        /**
         * Name of the resource.
         */
        std::string name;

        /**
         * Initializer expression.
         */
        std::shared_ptr<Expression> init;

        /**
         * Constructor for ScalarResource.
         * @param typ Type of the resource.
         * @param name Name of the resource.
         */
        ScalarResource(std::shared_ptr<Type> typ, std::string name);

        /**
         * Constructor for ScalarResource, intended to be used from YACC only.
         * This version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param typ Type of the resource.
         * @param name Name of the resource. The passed pointer will be free()d
         * by this constructor!
         */
        ScalarResource(Type * typ, char * name);

        /**
         * Constructor for ScalarResource.
         * @param typ Type of the resource.
         * @param name Name of the resource.
         * @param init Initializer expression.
         */
        ScalarResource(std::shared_ptr<Type> typ, std::string name, std::shared_ptr<Expression> init);

        /**
         * Constructor for ScalarResource, intended to be used from YACC only.
         * This version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param typ Type of the resource.
         * @param name Name of the resource. The passed pointer will be free()d
         * by this constructor!
         * @param init Initializer expression.
         */
        ScalarResource(Type * typ, char * name, Expression * init);

        /**
         * Default destructor for ScalarResource.
         */
        virtual ~ScalarResource() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents an array resource declaration.
     */
    class ArrayResource : public Resource {
    public:

        /**
         * Type of the array entries.
         */
        std::shared_ptr<Type> typ;

        /**
         * Name of the resource.
         */
        std::string name;

        /**
         * Array size expression.
         */
        std::shared_ptr<Expression> size;

        /**
         * Initializer expression.
         */
        std::shared_ptr<Expression> init;

        /**
         * Constructor for ArrayResource.
         * @param typ Type of the array entries.
         * @param name Name of the resource.
         * @param size Array size expression.
         */
        ArrayResource(std::shared_ptr<Type> typ, std::string name, std::shared_ptr<Expression> size);

        /**
         * Constructor for ArrayResource, intended to be used from YACC only.
         * This version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param typ Type of the array entries.
         * @param name Name of the resource. The passed pointer will be free()d
         * by this constructor!
         * @param size Array size expression.
         */
        ArrayResource(Type * typ, char * name, Expression * size);

        /**
         * Constructor for ArrayResource.
         * @param typ Type of the array entries.
         * @param name Name of the resource.
         * @param size Array size expression.
         * @param init Initializer expression.
         */
        ArrayResource(std::shared_ptr<Type> typ, std::string name, std::shared_ptr<Expression> size, std::shared_ptr<Expression> init);

        /**
         * Constructor for ArrayResource, intended to be used from YACC only.
         * This version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param typ Type of the array entries.
         * @param name Name of the resource. The passed pointer will be free()d
         * by this constructor!
         * @param size Array size expression.
         * @param init Initializer expression.
         */
        ArrayResource(Type * typ, char * name, Expression * size, Expression * init);

        /**
         * Default destructor for ArrayResource.
         */
        virtual ~ArrayResource() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a "let" statement.
     */
    class LetStatement : public Resource {
    public:

        /**
         * Name of the resource.
         */
        std::string name;

        /**
         * Initializer expression.
         */
        std::shared_ptr<Expression> init;

        /**
         * Constructor for LetStatement.
         * @param name Name of the resource.
         * @param init Initializer expression.
         */
        LetStatement(std::string name, std::shared_ptr<Expression> init);

        /**
         * Constructor for LetStatement, intended to be used from YACC only.
         * This version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param name Name of the resource. The passed pointer will be free()d
         * by this constructor!
         * @param init Initializer expression.
         */
        LetStatement(char * name, Expression * init);

        /**
         * Default destructor for LetStatement.
         */
        virtual ~LetStatement() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a statement that declares a mapping.
     */
    class Mapping : public Statement {
    public:

        /**
         * Name of the mapping.
         */
        std::string name;

        /**
         * Expression to map to.
         */
        std::shared_ptr<Expression> expr;

        /**
         * Constructor for Mapping.
         * @param name Name of the mapping.
         */
        Mapping(std::string name);

        /**
         * Constructor for Mapping, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param name Name of the mapping. The passed pointer will be free()d
         * by this constructor!
         */
        Mapping(char * name);

        /**
         * Constructor for Mapping.
         * @param name Name of the mapping.
         * @param expr Expression to map to.
         */
        Mapping(std::string name, std::shared_ptr<Expression> expr);

        /**
         * Constructor for Mapping, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param name Name of the mapping. The passed pointer will be free()d
         * by this constructor!
         * @param expr Expression to map to.
         */
        Mapping(char * name, Expression * expr);

        /**
         * Default destructor for Mapping.
         */
        virtual ~Mapping() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents an assignment statement.
     */
    class Assignment : public Statement {
    public:

        /**
         * Expression to be assigned.
         */
        std::shared_ptr<Expression> lvalue;

        /**
         * What to assign the lvalue to.
         */
        std::shared_ptr<Expression> rvalue;

        /**
         * Constructor for Assignment.
         * @param lvalue Expression to be assigned.
         * @param rvalue What to assign the lvalue to.
         */
        Assignment(std::shared_ptr<Expression> lvalue, std::shared_ptr<Expression> rvalue);

        /**
         * Constructor for Assignment, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param lvalue Expression to be assigned.
         * @param rvalue What to assign the lvalue to.
         */
        Assignment(Expression * lvalue, Expression * rvalue);

        /**
         * Default destructor for Assignment.
         */
        virtual ~Assignment() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a macro subroutine.
     */
    class MacroDef : public Statement {
    public:

        /**
         * Name of the macro subroutine.
         */
        std::string name;

        /**
         * Contents of the macro.
         */
        std::shared_ptr<Block> blk;

        /**
         * Source parameter names.
         */
        std::shared_ptr<IdentifierList> src;

        /**
         * Destination parameter names.
         */
        std::shared_ptr<IdentifierList> dest;

        /**
         * Constructor for MacroDef.
         * @param name Name of the macro subroutine.
         * @param blk Contents of the macro.
         */
        MacroDef(std::string name, std::shared_ptr<Block> blk);

        /**
         * Constructor for MacroDef, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param name Name of the macro subroutine. The passed pointer will be
         * free()d by this constructor!
         * @param blk Contents of the macro.
         */
        MacroDef(char * name, Block * blk);

        /**
         * Constructor for MacroDef.
         * @param name Name of the macro subroutine.
         * @param blk Contents of the macro.
         * @param src Source parameter names.
         */
        MacroDef(std::string name, std::shared_ptr<Block> blk, std::shared_ptr<IdentifierList> src);

        /**
         * Constructor for MacroDef, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param name Name of the macro subroutine. The passed pointer will be
         * free()d by this constructor!
         * @param blk Contents of the macro.
         * @param src Source parameter names.
         */
        MacroDef(char * name, Block * blk, IdentifierList * src);

        /**
         * Constructor for MacroDef.
         * @param name Name of the macro subroutine.
         * @param blk Contents of the macro.
         * @param src Source parameter names.
         * @param dest Destination parameter names.
         */
        MacroDef(std::string name, std::shared_ptr<Block> blk, std::shared_ptr<IdentifierList> src, std::shared_ptr<IdentifierList> dest);

        /**
         * Constructor for MacroDef, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param name Name of the macro subroutine. The passed pointer will be
         * free()d by this constructor!
         * @param blk Contents of the macro.
         * @param src Source parameter names.
         * @param dest Destination parameter names.
         */
        MacroDef(char * name, Block * blk, IdentifierList * src, IdentifierList * dest);

        /**
         * Default destructor for MacroDef.
         */
        virtual ~MacroDef() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a macro for loop.
     */
    class MacroFor : public Statement {
    public:

        /**
         * Name of the loop control variable.
         */
        std::string name;

        /**
         * Indices to iterate over.
         */
        std::shared_ptr<IndexList> indices;

        /**
         * Contents of the macro.
         */
        std::shared_ptr<Block> blk;

        /**
         * Constructor for MacroFor.
         * @param name Name of the loop control variable.
         * @param indices Indices to iterate over.
         * @param blk Contents of the macro.
         */
        MacroFor(std::string name, std::shared_ptr<IndexList> indices, std::shared_ptr<Block> blk);

        /**
         * Constructor for MacroFor, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param name Name of the loop control variable. The passed pointer
         * will be free()d by this constructor!
         * @param indices Indices to iterate over.
         * @param blk Contents of the macro.
         */
        MacroFor(char * name, IndexList * indices, Block * blk);

        /**
         * Default destructor for MacroFor.
         */
        virtual ~MacroFor() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a macro if/else.
     */
    class MacroIfElse : public Statement {
    public:

        /**
         * Condition expression.
         */
        std::shared_ptr<Expression> cond;

        /**
         * Block to insert if the expression is true.
         */
        std::shared_ptr<Block> blk_true;

        /**
         * Block to insert if the expression is false.
         */
        std::shared_ptr<Block> blk_false;

        /**
         * Constructor for MacroIfElse.
         * @param cond Condition expression.
         * @param blk_true Block to insert if the expression is true.
         */
        MacroIfElse(std::shared_ptr<Expression> cond, std::shared_ptr<Block> blk_true);

        /**
         * Constructor for MacroIfElse, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param cond Condition expression.
         * @param blk_true Block to insert if the expression is true.
         */
        MacroIfElse(Expression * cond, Block * blk_true);

        /**
         * Constructor for MacroIfElse.
         * @param cond Condition expression.
         * @param blk_true Block to insert if the expression is true.
         * @param blk_false Block to insert if the expression is false.
         */
        MacroIfElse(std::shared_ptr<Expression> cond, std::shared_ptr<Block> blk_true, std::shared_ptr<Block> blk_false);

        /**
         * Constructor for MacroIfElse, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param cond Condition expression.
         * @param blk_true Block to insert if the expression is true.
         * @param blk_false Block to insert if the expression is false.
         */
        MacroIfElse(Expression * cond, Block * blk_true, Block * blk_false);

        /**
         * Default destructor for MacroIfElse.
         */
        virtual ~MacroIfElse() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents an include statement.
     */
    class Include : public Statement {
    public:

        /**
         * Filename.
         */
        std::shared_ptr<StringLiteral> fname;

        /**
         * Constructor for Include.
         * @param fname Filename.
         */
        Include(std::shared_ptr<StringLiteral> fname);

        /**
         * Constructor for Include, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param fname Filename.
         */
        Include(StringLiteral * fname);

        /**
         * Default destructor for Include.
         */
        virtual ~Include() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a subcircuit header.
     */
    class Subcircuit : public Statement {
    public:

        /**
         * Subcircuit name.
         */
        std::string name;

        /**
         * Iteration count, or null if not specified.
         */
        std::shared_ptr<NumericLiteral> iter;

        /**
         * Constructor for Subcircuit.
         * @param name Subcircuit name.
         */
        Subcircuit(std::string name);

        /**
         * Constructor for Subcircuit, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param name Subcircuit name. The passed pointer will be free()d by
         * this constructor!
         */
        Subcircuit(char * name);

        /**
         * Constructor for Subcircuit.
         * @param name Subcircuit name.
         * @param iter Iteration count, or null if not specified.
         */
        Subcircuit(std::string name, std::shared_ptr<NumericLiteral> iter);

        /**
         * Constructor for Subcircuit, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param name Subcircuit name. The passed pointer will be free()d by
         * this constructor!
         * @param iter Iteration count, or null if not specified.
         */
        Subcircuit(char * name, NumericLiteral * iter);

        /**
         * Default destructor for Subcircuit.
         */
        virtual ~Subcircuit() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a label for classical flow control.
     */
    class Label : public Statement {
    public:

        /**
         * Label name.
         */
        std::string name;

        /**
         * Uniquified name.
         */
        std::string unique;

        /**
         * Constructor for Label.
         * @param name Label name.
         */
        Label(std::string name);

        /**
         * Constructor for Label, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param name Label name. The passed pointer will be free()d by this
         * constructor!
         */
        Label(char * name);

        /**
         * Default destructor for Label.
         */
        virtual ~Label() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a collection of gates.
     */
    class Bundle : public Statement {
    public:

        /**
         * List of gates in this bundle.
         */
        std::vector<std::shared_ptr<Gate>> gates;

        /**
         * Constructor for Bundle.
         */
        Bundle();

        /**
         * Default destructor for Bundle.
         */
        virtual ~Bundle() = default;

        /**
         * Appends to gate vector. Returns reference to this to allow chaining.
         * @param gate Value to push.
         * @return this, to allow chaining.
         */
        Bundle *push_gate(std::shared_ptr<Gate> gate);

        /**
         * Appends to gate vector. Returns reference to this to allow chaining.
         * @param gate Value to push.
         * @return this, to allow chaining.
         */
        Bundle *push_gate(Gate * gate);

        /**
         * Appends gates vector by another vector. Returns reference to this to
         * allow chaining.
         * @param gates Vector to push.
         * @return this, to allow chaining.
         */
        Bundle *push_gates(std::vector<std::shared_ptr<Gate>> & gates);

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents an erroneous statement. This is inserted by the parser to
     * recover from errors.
     */
    class ErroneousStatement : public Statement {
    public:

        /**
         * Constructor for ErroneousStatement.
         */
        ErroneousStatement();

        /**
         * Default destructor for ErroneousStatement.
         */
        virtual ~ErroneousStatement() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    /**
     * Represents a collection of statements and name resolution information.
     */
    class Block : public Node {
    public:

        /**
         * Vector of all statements in this list.
         */
        std::vector<std::shared_ptr<Statement>> stmts;

        /**
         * Constructor for Block.
         */
        Block();

        /**
         * Default destructor for Block.
         */
        virtual ~Block() = default;

        /**
         * Appends to stmt vector. Returns reference to this to allow chaining.
         * @param stmt Value to push.
         * @return this, to allow chaining.
         */
        Block *push_stmt(std::shared_ptr<Statement> stmt);

        /**
         * Appends to stmt vector. Returns reference to this to allow chaining.
         * @param stmt Value to push.
         * @return this, to allow chaining.
         */
        Block *push_stmt(Statement * stmt);

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };

    class Program : public Node {
    public:

        /**
         * Version string.
         */
        std::string version;

        /**
         * Block of code representing the program.
         */
        std::shared_ptr<Block> blk;

        /**
         * Constructor for Program.
         * @param version Version string.
         * @param blk Block of code representing the program.
         */
        Program(std::string version, std::shared_ptr<Block> blk);

        /**
         * Constructor for Program, intended to be used from YACC only. This
         * version:
         *  - uses char* for strings and bare pointers instead of
         * std::shared_ptr<> encapsulations for inputs;
         *  - calls free() on strings passed to it after constructing
         * std::string instances.
         * @param version Version string. The passed pointer will be free()d by
         * this constructor!
         * @param blk Block of code representing the program.
         */
        Program(char * version, Block * blk);

        /**
         * Default destructor for Program.
         */
        virtual ~Program() = default;

        /**
         * Converts to a "ClassName(...)" string for debugging.
         */
        virtual operator std::string() const override;


    };


}}

#endif

