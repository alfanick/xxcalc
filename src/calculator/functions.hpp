#include "value.hpp"

#pragma once

namespace XX {
namespace Calculator {
namespace Functions {

/**
 * Addition operator
 *
 * @param args Two operands
 * @return Added operands
 */
Value addition(std::vector<Value> const& args);

/**
 * Subtraction operator
 *
 * @param args Two operands
 * @return Subtracted operands
 */
Value subtraction(std::vector<Value> const& args);

/**
 * Multiplication operator
 *
 * @param args Two operands
 * @return Multiplied operands
 */
Value multiplication(std::vector<Value> const& args);

/**
 * Division operator
 *
 * @throw PolynomialDivisionError When degree of first operand
 *        is smaller than degree of second operand
 * @param args Two operands
 * @return Divided operands
 */
Value division(std::vector<Value> const& args);

/**
 * Exponentiation operator. The exponent must be a
 * constant polynomial otherwise result will no longer
 * be a polynomial.
 *
 * Different methods of exponentiation are used, depending
 * on type of base polynomial. If base is a constant
 * polynomial a standard math function is used. If base is
 * a polynomial with degree 1 a value with appropriate
 * degree and coefficient is created. Otherwise if base
 * polynomial is complex a multiplation of base is performed
 * as many times as in the exponent.
 *
 * @throws ExponentationError When the exponent is not a
 *         constant polynomial.
 * @param args Two operands (a base and an exponent)
 * @return Result of exponentiation
 */
Value exponentiation(std::vector<Value> const& args);

/**
 * Computes decimal logarithm
 *
 * @param args A single argument
 * @return Logarithmed value
 */
Value log10(std::vector<Value> const&);

/**
 * Computes logarithm of given base
 *
 * @param args Two arguments (value and base)
 * @return Logarithmed value
 */
Value log(std::vector<Value> const&);

}
}
}
