#include <vector>
#include <string>

#pragma once

namespace XX {
namespace Calculator {

/**
 * The value is a type used internally in the evaluator. The value
 * represents a polynomial. It is unambiguously characterized by
 * its coefficients. A value of degree 1 is a plain double value,
 * while a value of degree 2 is a linear expression.
 *
 * Common operators are implemented, so the value can be easily
 * used in a code.
 */
class Value {
  public:

  /**
   * Creates value (polynomial) with a list of coefficients.
   *
   * @param coefficients List of coefficients
   */
  Value(std::vector<double> coefficients) : coefficients(coefficients) { }

  /**
   * Creates a linear expression of form ax+b.
   *
   * @param b Constant term
   * @param a Linear coefficient
   */
  Value(double b, double a) : Value(std::vector<double>{b, a}){ }

  /**
   * Creates degenerative polynomial with just a constant term.
   *
   * @param b Constant term
   */
  Value(double b) : Value(b, 0.0) { }

  /**
   * Creates zero polynomial
   */
  Value() : Value(0.0) { }

  /**
   * Accesses coefficients of the polynomial. If a coefficient
   * with given index is not existing it is created with a zero
   * value.
   *
   * @param index Coefficient index
   * @return Reference to coefficient
   */
  double& operator[](const unsigned long index);

  /**
   * Accesses coefficients of the polynomial.
   *
   * @param index Coefficient index
   * @return Coefficient value
   */
  double operator[](const unsigned long index) const;

  /**
   * Computes degree of the polynomial. A degree is an index
   * of rightmost non zero coefficient.
   *
   * @return Degree of polynomial
   */
  unsigned long degree() const;

  /**
   * Creates a human readable string representation of
   * the polynomial. The polynomial is represented in
   * the minimal algebraic form, beginning with the
   * largest exponents.
   *
   * @param name Name of variable used in polynomial
   * @return Algebraic form
   */
  std::string repr(std::string const& name) const;

  /**
   * Converts polynomial to singular double value.
   * Makes sense only with polynomial of degree zero
   * (constant polynomials).
   *
   * @throw PolynomialCastError When polynomial is not constant
   * @return Value of constant term
   */
  explicit operator double() const;

  /**
   * Creates an algebraic form of polynomial, using
   * x as name of the variable.
   *
   * @return Algebraic form
   */
  explicit operator std::string() const;

  /**
   * Performs polynomial addition. This is a linear
   * operation implemented using vector addition.
   *
   * @param other Polynomial to add
   * @return Reference to self
   */
  Value& operator+=(Value const& other);

  /**
   * Performs polynomial subtraction. This is a linear
   * operation implemented using vector subtraction.
   *
   * @param other Polynomial to subtract
   * @return Reference to self
   */
  Value& operator-=(Value const& other);

  /**
   * Performs polynomial multiplicaton. This used
   * algorithm is O(n*m). For constant polynomial
   * a direct multiplication is used.
   *
   * @param other Polynomial to multiply
   * @return Reference to self
   */
  Value& operator*=(Value const& other);

  /**
   * Performs polynomial division. A classical
   * long division algorithm is used with O(n^2)
   * complexity. For constant polynomial a direct
   * division is used.
   *
   * @throw PolynomialDivisionError When degree of
   *        divider is large than degree of self.
   * @param other Divider
   * @return Reference to self
   */
  Value& operator/=(Value const& other);

  /**
   * Creates a copy of self and adds another value.
   *
   * @param other Value to add
   * @return Copied result
   */
  const Value operator+(Value const& other) const;

  /**
   * Creates a copy of self and subtracts another value.
   *
   * @param other Value to subtract
   * @return Copied result
   */
  const Value operator-(Value const& other) const;

  /**
   * Creates a copy of self and multiples with another
   * value.
   *
   * @param other Value to multiply
   * @return Copied result
   */
  const Value operator*(Value const& other) const;

  /**
   * Creates a copy of self and divides it by another
   * value.
   *
   * @param other Divider
   * @return Copied result
   */
  const Value operator/(Value const& other) const;

  /**
   * Compares equality of two values. They values
   * are considered equal when they are of the same
   * degree and contain equal coefficients.
   *
   * @param other Value to compare
   * @return True if equal
   */
  bool operator==(Value const &other) const;

  /**
   * Performs inequality test using previously defined
   * equality operator.
   *
   * @param other Value to compare
   * @return True if not equal
   */
  bool operator!=(Value const &other) const;

  private:

  //! Polynomial coefficients
  std::vector<double> coefficients;
};

}
}
