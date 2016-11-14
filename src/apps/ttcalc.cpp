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

  Calculator::Value ans;

  parser.register_operator("+", 1, -1);
  evaluator.register_function("+", 2, [](std::vector<Calculator::Value> const& args) {
    return args[0] + args[1];
  });

  parser.register_operator("-", 1, -1);
  evaluator.register_function("-", 2, [](std::vector<Calculator::Value> const& args) {
    return args[0] - args[1];
  });

  parser.register_operator("*", 5, -1);
  evaluator.register_function("*", 2, [](std::vector<Calculator::Value> const& args) {
    return args[0] * args[1];
  });

  parser.register_operator("/", 5, -1);
  evaluator.register_function("/", 2, [](std::vector<Calculator::Value> const& args) {
    return args[0] / args[1];
  });

  evaluator.register_function("x", 0, [](std::vector<Calculator::Value> const& args) {
    return Calculator::Value(0, 1);
  });

  parser.register_operator("^", 10 1);
  evaluator.register_function("^", 2, [](std::vector<Calculator::Value> const& args) {
    size_t base_degree = args[0].degree();
    size_t exponent_degree = args[1].degree();

    if (exponent_degree > 0) {
      throw Calculator::ExponentationError("Unable to perform complex exponentation - only constant polynomials supported");
    } else
    if (base_degree == 0) {
      std::cout << "float exp" << std::endl;
      return Calculator::Value(pow(args[0][0], args[1][0]));
    } else
    if (base_degree == 1 && args[0][0] == 0) {
      std::cout << "easy exp" << std::endl;
      Calculator::Value v;
      v[args[1][0]] = 1.0;
      return v;
    } else {
      std::cout << "complex exp" << std::endl;
      Calculator::Value v = args[0];
      size_t exponent = args[1][0];
      for (size_t i = 1; i < exponent; i++) {
        v *= args[0];
      }
      return v;
    }
  });

  evaluator.register_function("log10", 1, [](std::vector<Calculator::Value> const& args) {
    return log10(double(args[0]));
  });

  evaluator.register_function("pi", 0, [](std::vector<Calculator::Value> const& args) {
    return M_PI;
    // return Calculator::Value(M_PI);
  });

  // evaluator.register_function("ans", 0, [&ans](std::vector<double> const& args) {
    // return ans;
  // });

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

      std::cout << std::string(ans) << std::endl;
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
