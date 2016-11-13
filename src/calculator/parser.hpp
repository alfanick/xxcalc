#include "node.hpp"
#include "tokenizer.hpp"
#include "errors.hpp"

#include <map>
#include <stack>

#pragma once

namespace XX {
namespace Calculator {

class Parser {
  public:

  Parser();

  void register_operator(std::string const& value, int p, int a);
  void register_function(std::string const& value, int a);

  virtual Node::unique process(TokenList tokens) const;

  private:

  Node::unique create_function(Token const& token, std::stack<Node::unique> &stack) const;
  bool lower_precedence(Token const& a, Token const& b) const;

  struct Operator {
    int precedence;
    int associativity;

    Operator(int p, int a) : precedence(p), associativity(a) { }
  };

  struct Function {
    size_t arity;

    Function(size_t a) : arity(a) { }
  };

  std::map<std::string, Operator> operators;
  std::map<std::string, Function> functions;
};

}
}
