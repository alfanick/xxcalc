#include "polynomial_calculator.hpp"

#pragma once

namespace XX {
namespace Calculator {

/**
 * Linear solver extends polynomial calculator with basic
 * solving capabilities. This solver can solve simple
 * linear equations and perform every other operation
 * available in polynomial calculator.
 *
 * A new operator '=' is defined - it solves a single linear
 * equation (a polynomial with degree equal 1) for symbol x.
 *
 * If solver cannot solve given equation, an appropriate
 * exception is thrown.
 */
class LinearSolver : public PolynomialCalculator {
  public:

  /**
   * Creates instance of linear solver. Solver support the
   * same operators and functions as PolynomialCalculator,
   * however it registers a '=' operator (with lowest
   * possible precedence, so it always divides expression
   * into halves).
   *
   * @param tokenizer Tokenizer to use
   * @param parser Parser to use
   */
  LinearSolver(Tokenizer& tokenizer, Parser& parser);

  private:

  /**
   * Solving operator. This solver can solve equations with
   * polynomials of at most degree 1. The principle of its
   * work is moving symbols to LHS and while moving numerical
   * constants to RHS. This is done just by simple subtraction
   * of appropriate coefficients, then coefficients are scaled
   * to be unit value at RHS. The value of "x" is read as LHS
   * constant. The algorithm running time is constant O(1).
   *
   * It should be noted that this is not a symbolic solver, as
   * operands are evaluated values of functions (if any).
   *
   * @throw NonLinearEquation When any of operands is not a
   *        linear expression (has a degree large than 1)
   * @throw NoSymbolFound When both operands are a constant
   *        expression (no symbol to solve)
   * @throw ExpressionIsTautology When there are infinite
   *        solutions (like in "x=x")
   * @throw NonSolvableExpression When there are no valid
   *        solutions (like in "x=x+1")
   * @param args Two operands
   * @return Value of the symbol
   */
  static Value solve_operator(std::vector<Value> const& args);
};

}
}
