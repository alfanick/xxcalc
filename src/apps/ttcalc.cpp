#include <iostream>
#include "calculator/tokenizer.hpp"
#include "calculator/parser.hpp"
#include "calculator/evaluator.hpp"

#include <cmath>
using namespace XX;

int main(int argc, char** argv) {
  Calculator::Tokenizer tokenizer;
  Calculator::Parser parser;
  Calculator::Evaluator evaluator;

  evaluator.register_function("+", 2, [](std::vector<double> const& args) {
    return args[0] + args[1];
  });

  evaluator.register_function("-", 2, [](std::vector<double> const& args) {
    return args[0] - args[1];
  });

  evaluator.register_function("*", 2, [](std::vector<double> const& args) {
    return args[0] * args[1];
  });

  evaluator.register_function("/", 2, [](std::vector<double> const& args) {
    return args[0] / args[1];
  });

  evaluator.register_function("^", 2, [](std::vector<double> const& args) {
    return pow(args[0], args[1]);
  });

  evaluator.register_function("log10", 1, [](std::vector<double> const& args) {
    return log10(args[0]);
  });

  std::string line;
  while (std::getline(std::cin, line)) {
    Calculator::TokenList tokens = tokenizer.process(line);

    try {
      tokens = parser.process(std::move(tokens));

      std::cout << evaluator.process(tokens) << std::endl;
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
