#include "tokenizer.hpp"
#include "parser.hpp"
#include "evaluator.hpp"

#pragma once

namespace XX {
namespace Calculator {

class PolynomialCalculator {
  public:

  PolynomialCalculator(Tokenizer& tokenizer, Parser& parser);

  Value process(std::string const& line);
  void register_function(std::string const& name, unsigned long arity, std::function<Value(std::vector<Value> const&)> f);
  void register_constant(std::string const& name, Value value);

  Value last_value;

  private:

  Tokenizer& tokenizer;
  Parser& parser;
  Evaluator evaluator;
};

}
}
