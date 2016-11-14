#include "evaluator.hpp"
#include "errors.hpp"

#include <stack>

namespace XX {
namespace Calculator {

Value Evaluator::process(TokenList& tokens) {
  std::stack<Value> stack;

  // process from left to right
  while (!tokens.empty()) {
    auto token = tokens.front();
    tokens.pop_front();

    // put number on a stack
    if (token.type == TokenType::NUMBER) {
      stack.push(std::stod(token.value));
    } else
    // identifier or operator are the same
    if (token.type == TokenType::OPERATOR ||
        token.type == TokenType::IDENTIFIER) {

      // if contant replace it with its value
      auto constant = constants.find(token.value);
      if (constant != constants.end()) {
        stack.push(constant->second);
        continue;
      }

      // find handler
      auto function = functions.find(token.value);
      if (function == functions.end()) {
        throw UnknownSymbolError(token.value, token.position);
      }

      // require arguments from the stack
      if (stack.size() < function->second.arity) {
        throw ArgumentMissingError(token.value, token.position);
      } else {
        // construct parameters
        std::vector<Value> args;
        args.resize(function->second.arity);

        for (int i = function->second.arity-1; i >= 0; i--) {
          args[i] = stack.top();
          stack.pop();
        }

        // call the function and store result
        stack.push(function->second.handle(args));
      }
    }
  }

  // expected a single result
  if (stack.size() == 1) {
    return stack.top();
  } else {
    throw EvaluationError("Only single expression is allowed", 0);
  }
}

void Evaluator::register_function(std::string const& name, unsigned long arity, std::function<Value(std::vector<Value> const&)> f) {
  if (constants.find(name) != constants.end())
    throw ConflictingNameError("Cannot add function '"+name+"' as it name is already used by a constant.");

  functions.emplace(name, Function(arity, f));
}

void Evaluator::register_constant(std::string const& name, Value value) {
  if (functions.find(name) != functions.end())
    throw ConflictingNameError("Cannot add constant '"+name+"' as it name is already used by a function.");

  constants[name] = value;
}

}
}
