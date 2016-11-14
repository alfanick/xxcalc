#include "evaluator.hpp"
#include "errors.hpp"

#include <stack>

namespace XX {
namespace Calculator {


void Evaluator::register_function(std::string const& name, size_t arity, std::function<Value(std::vector<Value> const&)> f) {
  functions.emplace(name, Function(arity, f));
}

Value Evaluator::process(TokenList& tokens) {
  std::stack<Value> stack;

  while (!tokens.empty()) {
    auto token = tokens.front();
    tokens.pop_front();

    if (token.type == TokenType::NUMBER) {
      stack.push(std::stod(token.value));
    } else
    if (token.type == TokenType::OPERATOR ||
        token.type == TokenType::IDENTIFIER) {

      auto function = functions.find(token.value);
      if (function == functions.end()) {
        throw UnknownFunctionError(token.value, token.position);
      }

      if (stack.size() < function->second.arity) {
        throw ArgumentMissingError(token.value, token.position);
      } else {
        std::vector<Value> args;
        args.resize(function->second.arity);

        for (int i = function->second.arity-1; i >= 0; i--) {
          args[i] = stack.top();
          stack.pop();
        }
        stack.push(function->second.handle(args));
      }
    }
  }

  if (stack.size() == 1) {
    return stack.top();
  } else {
    throw EvaluationError("Only single expression is allowed", 0);
  }
}

}
}
