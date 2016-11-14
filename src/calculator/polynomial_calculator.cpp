#include "polynomial_calculator.hpp"
#include "functions.hpp"

#include <cmath>

namespace XX {
namespace Calculator {

PolynomialCalculator::PolynomialCalculator(Tokenizer& tokenizer, Parser& parser) :
  tokenizer(tokenizer), parser(parser) {

  parser.register_operator("+", 1, -1);
  register_function("+", 2, Functions::addition);

  parser.register_operator("-", 1, -1);
  register_function("-", 2, Functions::subtraction);

  parser.register_operator("*", 5, -1);
  register_function("*", 2, Functions::multiplication);

  parser.register_operator("/", 5, -1);
  register_function("/", 2, Functions::division);

  parser.register_operator("^", 10, 1);
  register_function("^", 2, Functions::exponentiation);

  register_constant("x", Value(0, 1));
  register_constant("pi", Value(M_PI));
  register_constant("e", Value(M_E));

  register_function("log", 2, Functions::log);
  register_function("log10", 1, Functions::log10);

  register_function("ans", 0, [&](std::vector<Value> const& args) {
    return last_value;
  });
}

void PolynomialCalculator::register_function(std::string const& name, size_t arity,
                                             std::function<Value(std::vector<Value> const&)> f) {
  evaluator.register_function(name, arity, f);
}

void PolynomialCalculator::register_constant(std::string const& name, Value value) {
  evaluator.register_constant(name, value);
}

Value PolynomialCalculator::process(std::string const& line) {
  TokenList tokens = tokenizer.process(line);

#ifdef DEBUG
  std::cerr << "Tokenized '" << line << "': " << std::endl;
  std::cerr << tokens << std::endl;
#endif

  tokens = parser.process(std::move(tokens));

#ifdef DEBUG
  std::cerr << "Parsed '" << line << "': " << std::endl;
  std::cerr << tokens << std::endl;
#endif

  last_value = evaluator.process(tokens);

  return last_value;
}

}
}