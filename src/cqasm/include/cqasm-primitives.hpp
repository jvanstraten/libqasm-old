#pragma once

#include <string>
#include <cstdint>
#include <complex>
#include <vector>

namespace cqasm {
namespace primitives {

/**
 * Generates a default value for the given primitive type. This is specialized
 * for the primitives mapping to builtin types (int, bool, etc, for which the
 * "constructor" doesn't initialize the value at all) such that they actually
 * initialize with a sane default. Used in the default constructors of the
 * generated tree nodes to ensure that there's no garbage in the nodes.
 */
template <class T>
T initialize() { return T(); };

/**
 * String primitive used within the AST and semantic trees.
 */
using Str = std::string;
template <>
Str initialize<Str>();

/**
 * Boolean primitive used within the semantic trees. Defaults to false.
 */
using Bool = bool;
template <>
Bool initialize<Bool>();

/**
 * Axis primitive used within the semantic trees. Defaults to X.
 */
enum class Axis { X, Y, Z };
template <>
Axis initialize<Axis>();

/**
 * Integer primitive used within the AST and semantic trees.
 */
using Int = std::int64_t;
template <>
Int initialize<Int>();

/**
 * Real number primitive used within the AST and semantic trees.
 */
using Real = double;
template <>
Real initialize<Real>();

/**
 * Complex number primitive used within the semantic trees.
 */
using Complex = std::complex<double>;

/**
 * Two-dimensional matrix of some kind of type.
 */
template <typename T>
class Matrix {
private:
    std::vector<T> data;
    size_t nrows;
    size_t ncols;
public:
    /**
     * Creates an empty matrix.
     */
    Matrix()
        : data(ncols), nrows(1), ncols(0)
    {}

    /**
     * Creates a vector.
     */
    Matrix(size_t ncols)
        : data(ncols), nrows(1), ncols(ncols)
    {}

    /**
     * Creates a zero-initialized matrix of the given size.
     */
    Matrix(size_t nrows, size_t ncols)
        : data(nrows*ncols), nrows(nrows), ncols(ncols)
    {}

    /**
     * Creates a column vector with the given data.
     */
    Matrix(const std::vector<T> &data)
        : data(data), nrows(data.size()), ncols(1)
    {}

    /**
     * Creates a matrix with the given data. The number of rows is inferred. If
     * the number of data elements is not divisible by the number of columns, a
     * range error is thrown.
     */
    Matrix(const std::vector<T> &data, size_t ncols)
        : data(data), nrows(data.size() / ncols), ncols(ncols)
    {
        if (data.size() % ncols != 0) {
            throw std::range_error("invalid matrix shape");
        }
    }

    /**
     * Returns the number of rows.
     */
    size_t size_rows() const {
        return nrows;
    }

    /**
     * Returns the number of columns.
     */
    size_t size_cols() const {
        return ncols;
    }

    /**
     * Returns the value at the given position. row and col start at 1. Throws
     * a std::range_error when either or both indices are out of range.
     */
    T at(size_t row, size_t col) const {
        if (row < 1 || row > nrows || col < 1 || col > ncols) {
            throw std::range_error("matrix index out of range");
        }
        return data[(row - 1) * ncols + col - 1];
    }

    /**
     * Returns a mutable reference to the value at the given position. row and
     * col start at 1. Throws a std::range_error when either or both indices
     * are out of range.
     */
    T &at(size_t row, size_t col) {
        if (row < 1 || row > nrows || col < 1 || col > ncols) {
            throw std::range_error("matrix index out of range");
        }
        return data[(row - 1) * ncols + col - 1];
    }

    /**
     * Equality operator for matrices.
     */
    bool operator==(const Matrix<T> &rhs) const {
        return data == rhs.data && nrows == rhs.nrows && ncols == rhs.ncols;
    }

    /**
     * Inequality operator for matrices.
     */
    bool operator!=(const Matrix<T> &rhs) const {
        return !(*this == rhs);
    }
};

/**
 * Matrix of real numbers.
 */
using RMatrix = Matrix<Real>;

/**
 * Matrix of complex numbers.
 */
using CMatrix = Matrix<Complex>;

/**
 * Version number primitive used within the AST and semantic trees.
 */
class Version : public std::vector<Int> {
};

} // namespace primitives
} // namespace cqasm

/**
 * Stream << overload for axis nodes.
 */
std::ostream& operator<<(std::ostream& os, const ::cqasm::primitives::Axis& axis);

/**
 * Stream << overload for matrix nodes.
 */
template <typename T>
std::ostream& operator<<(std::ostream& os, const ::cqasm::primitives::Matrix<T>& mat) {
    os << "[";
    for (size_t row = 1; row <= mat.size_rows(); row++) {
        if (row > 1) {
            os << "; ";
        }
        for (size_t col = 1; col <= mat.size_cols(); col++) {
            if (col > 1) {
                os << ", ";
            }
            os << mat.at(row, col);
        }
    }
    os << "]";
    return os;
}

/**
 * Stream << overload for version nodes.
 */
std::ostream& operator<<(std::ostream& os, const ::cqasm::primitives::Version& object);
