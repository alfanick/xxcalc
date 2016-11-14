#include <iostream>
#include "calculator/tokenizer.hpp"
#include "calculator/parser.hpp"
#include "calculator/evaluator.hpp"

#include "calculator/polynomial_calculator.hpp"

#include <cmath>
using namespace XX;

int main(int argc, char** argv) {
  Calculator::Tokenizer tokenizer;
  Calculator::Parser parser;
  Calculator::PolynomialCalculator calculator(tokenizer, parser);

  std::string line;
  while (std::getline(std::cin, line)) {
    try {
      std::cout << std::string(calculator.process(line)) << std::endl;
    }
    catch (Calculator::ValueError& error) {
      std::cerr << "[VALUE] " << error.what() << std::endl;
    }
    catch (Calculator::EvaluationError& error) {
      std::cerr << "[EVALUATION] " << error.what() << std::endl;
    }
    catch (Calculator::ParsingError& error) {
      std::cerr << "[PARSING] " << error.what() << std::endl;
    }

  }

  return EXIT_SUCCESS;
}
