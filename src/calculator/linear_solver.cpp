#include "linear_solver.hpp"
#include "errors.hpp"

#include <cmath>
#include <limits>

namespace XX {
namespace Calculator {

LinearSolver::LinearSolver(Tokenizer& tokenizer, Parser& parser) :
  PolynomialCalculator(tokenizer, parser) {
  register_operator("=", std::numeric_limits<int>::min(), -1,
                    std::bind(&LinearSolver::solve_operator, this, std::placeholders::_1));
}

Value LinearSolver::process(std::string const& line) {
  solved = false;
  return PolynomialCalculator::process(line);
}

Value LinearSolver::solve_operator(std::vector<Value> const& args) {
  unsigned long left_degree = args[0].degree();
  unsigned long right_degree = args[1].degree();

  Value left = args[0];
  Value right = args[1];

  if (left_degree > 1 || right_degree > 1) {
    throw NonLinearEquation();
  } else
  if (left_degree == 0 && right_degree == 0) {
    throw NoSymbolFound();
  }

  left[1] -= right[1];
  right[1] -= right[1];

  right[0] -= left[0];
  left[0] -= left[0];

  right[0] /= left[1];
  left[1] /= left[1];

  if (right[0] != right[0]) {
    throw ExpressionIsTautology();
  } else
  if (std::isinf(right[0])) {
    throw NonSolvableExpression();
  }

  solved = true;

  return right;
}

}
}
