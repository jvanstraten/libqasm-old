
#include <sstream>
#include <string>
#include <algorithm>

#include "type.hpp"
#include "cqasm2/semantic/exceptions.hpp"

namespace cqasm2 { namespace ast {

    /**************************************************************************
     * Type                                                                   *
     **************************************************************************/

    /**
     * Returns whether an array of this type can be indexed dynamically.
     */
    bool Type::canIndexDynamically() const {
        return true;
    }

    /**
     * Returns whether this type can be subscripted by the given subscript.
     */
    bool Type::canSubscript(const std::string subscript) const {
        (void)subscript;
        return false;
    }

    /**
     * Returns the type of the given subscript.
     */
    std::shared_ptr<Type> Type::getSubscriptType(const std::string subscript) const {
        throw InternalError("Trying to subscript non-subscriptable type");
    }

    /**
     * Returns whether this is a regular classical datatype.
     */
    bool Type::isClassical() const {
        return true;
    }

    /**
     * Returns whether this is/can be a measurement register.
     */
    bool Type::isMeasurement() const {
        return false;
    }

    /**
     * Returns whether this is/can be a quantum datatype.
     */
    bool Type::isQuantum() const {
        return false;
    }


    /**************************************************************************
     * QubitType                                                              *
     **************************************************************************/

    /**
     * Default constructor.
     */
    QubitType::QubitType() : disambiguatedToQubit(false), disambiguatedToMeasurement(false) {
    }

    /**
     * Returns that a qubit cannot be dynamically indexed.
     */
    bool QubitType::canIndexDynamically() const {
        return false;
    }

    /**
     * Returns that this type can be disambiguated.
     */
    bool QubitType::canSubscript(const std::string subscript) const {
        if (this->disambiguatedToQubit) return false;
        if (this->disambiguatedToMeasurement) return false;
        return subscript == "q" || subscript == "Q" || subscript == "b" || subscript == "B";
    }

    /**
     * Returns the type of the given subscript.
     */
    std::shared_ptr<Type> QubitType::getSubscriptType(const std::string subscript) const {
        if (!this->canSubscript(subscript)) {
            throw InternalError("Illegal subscript for qubit type or already disambiguated");
        }
        auto ret = std::make_shared<QubitType>();
        if (subscript == "q" || subscript == "Q") {
            ret->disambiguatedToQubit = true;
        } else {
            ret->disambiguatedToMeasurement = true;
        }
        return ret;
    }

    /**
     * Returns that this isn't a classical type.
     */
    bool QubitType::isClassical() const {
        return false;
    }

    /**
     * Returns whether this can be used as a measurement register.
     */
    bool QubitType::isMeasurement() const {
        return not this->disambiguatedToQubit;
    }

    /**
     * Returns whether this can be used as a qubit.
     */
    bool QubitType::isQuantum() const {
        return not this->disambiguatedToMeasurement;
    }

    /**
     * Resolves all references in this node tree, and gives all
     * declarations a unique name.
     */
    void QubitType::resolve(Namespace &names) {
        (void)names;
    }

    /**
     * Performs macro expansion, constant folding, type checking, etc.
     */
    std::shared_ptr<Node> QubitType::elaborate() {
        return std::shared_ptr<Node>(this);
    }

    /**
     * Return the name of this type in cQASM syntax (if there is such a
     * representation).
     */
    QubitType::operator std::string() const {
        return "qubit";
    }


    /**************************************************************************
     * FixedType                                                              *
     **************************************************************************/

    /**
     * Constructor for int/uint.
     */
    FixedType::FixedType(bool sign, std::shared_ptr<Expression> bitcount)
        : sign(sign), intBits(bitcount), fracBits() { }

    /**
     * Constructor for fixed/ufixed.
     */
    FixedType::FixedType(bool sign, std::shared_ptr<Expression> intBits, std::shared_ptr<Expression> fracBits)
        : sign(sign), intBits(intBits), fracBits(fracBits) { }

    /**
     * Return the number of integer bits as an expression.
     */
    std::shared_ptr<Expression> FixedType::getIntBitsExp() const {
        return this->intBits;
    }

    /**
     * Return the number of fractional bits as an expression. This returns null
     * if this is an int or uint type.
     */
    std::shared_ptr<Expression> FixedType::getFracBitsExp() const {
        return this->fracBits;
    }

    /**
     * Return the number of bits. This is only legal after elaboration.
     */
    int FixedType::getBits() const {
    }

    /**
     * Return the number of integer bits. This is only legal after elaboration.
     */
    int FixedType::getIntBits() const {
    }

    /**
     * Return the number of fractional bits. This is only legal after elaboration.
     */
    int FixedType::getFracBits() const {
    }

    /**
     * Return whether this integer type is fixed-point.
     */
    bool FixedType::isFixed() const {
    }

    /**
     * Return whether this integer type is signed.
     */
    bool FixedType::isSigned() const {
    }

    /**
     * Resolves all references in this node tree, and gives all
     * declarations a unique name.
     */
    void FixedType::resolve(Namespace &names) {
    }

    /**
     * Performs macro expansion, constant folding, type checking, etc.
     */
    std::shared_ptr<Node> FixedType::elaborate() {
    }

    /**
     * Return the name of this type in cQASM syntax (if there is such a
     * representation).
     */
    FixedType::operator std::string() const {
    }

}}
