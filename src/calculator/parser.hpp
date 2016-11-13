#include "node.hpp"
#include "tokenizer.hpp"
#include "errors.hpp"

#include <map>

#pragma once

namespace XX {
namespace Calculator {

struct Operator {
  int precedence;
  int associativity;

  Operator(int p, int a) : precedence(p), associativity(a) { }
};

struct Function {
  size_t arity;

  Function(size_t a) : arity(a) { }
};

class Parser {
  public:

  Parser();

  void register_operator(std::string const& value, int p, int a);
  void register_function(std::string const& value, int a);

  virtual Node::unique process(TokenList tokens) const;

  private:

  bool lower_precedence(Token const& a, Token const& b) const;

  std::map<std::string, Operator> operators;
  std::map<std::string, Function> functions;
};

}
}
