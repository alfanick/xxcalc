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

  double ans;

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

  evaluator.register_function("pi", 0, [](std::vector<double> const& args) {
    return M_PI;
  });

  evaluator.register_function("ans", 0, [&ans](std::vector<double> const& args) {
    return ans;
  });

  std::string line;
  while (std::getline(std::cin, line)) {
    Calculator::TokenList tokens = tokenizer.process(line);
#ifdef DEBUG
    std::cerr << "Tokenized: " << std::endl;
    std::cerr << tokens << std::endl;
#endif

    try {
      tokens = parser.process(std::move(tokens));

#ifdef DEBUG
    std::cerr << "Parsed: " << std::endl;
    std::cerr << tokens << std::endl;
#endif

      ans = evaluator.process(tokens);

      std::cout << ans << std::endl;
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
