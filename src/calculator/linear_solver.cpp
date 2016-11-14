#include "linear_solver.hpp"
#include "errors.hpp"

namespace XX {
namespace Calculator {

LinearSolver::LinearSolver(Tokenizer& tokenizer, Parser& parser) :
  PolynomialCalculator(tokenizer, parser) {
  parser.register_operator("=", std::numeric_limits<int>::min(), -1);
  register_function("=", 2, [](std::vector<Value> const& args) {
    size_t left_degree = args[0].degree();
    size_t right_degree = args[1].degree();

    Value left = args[0];
    Value right = args[1];

    if (left_degree > 1 || right_degree > 1) {
      throw NonLinearEquation();
    } else
    if (left_degree == 0 && right_degree == 0) {
      throw NoSymbolFound();
    }

    left[1] -= right[1]; // move x to left
    right[1] -= right[1];

    right[0] -= left[0]; // move numbers to right
    left[0] -= left[0];

    right[0] /= left[1];
    left[1] /= left[1];

    return right;
  });
}

}
}
