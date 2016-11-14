#include "polynomial_calculator.hpp"

#pragma once

namespace XX {
namespace Calculator {

class LinearSolver : public PolynomialCalculator {
  public:

  LinearSolver(Tokenizer& tokenizer, Parser& parser);

};

}
}
