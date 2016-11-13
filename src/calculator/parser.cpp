#include "parser.hpp"

#include <limits>

namespace XX {
namespace Calculator {

Parser::Parser() {
  register_operator("+", 1, -1);
  register_operator("-", 1, -1);
  register_operator("*", 5, -1);
  register_operator("/", 5, -1);
  register_operator("^", 10, 1);
}

void Parser::register_operator(std::string const& value, int p, int a) {
  operators.emplace(value, Operator(p, a));
}

void Parser::register_function(std::string const& value, size_t a) {
  functions.emplace(value, Function(a));
  operators.emplace(value, Operator(std::numeric_limits<int>::max(), -1));
}

bool Parser::lower_precedence(Token const& a, Token const& b) const {
  auto operator_a = operators.find(a.value);
  auto operator_b = operators.find(b.value);

  // depends on associativity
  return (operator_a->second.associativity < 0 &&
          operator_a->second.precedence <= operator_b->second.precedence) ||
         (operator_a->second.associativity > 0 &&
          operator_a->second.precedence < operator_b->second.precedence);
}

Node::unique Parser::create_function(Token const& token, std::stack<Node::unique> &stack) const {
  size_t arity = 0;

  // operators are always two-argument
  if (token.type == TokenType::OPERATOR) {
    arity = 2;
  } else {
    auto function = functions.find(token.value);
    arity = function->second.arity;
  }

  // stack must contain required arity
  if (stack.size() < arity) {
    if (token.type == TokenType::OPERATOR) {
      throw OperandMissingError(token.value, token.position);
    } else {
      throw ArgumentMissingError(token.value, token.position);
    }
  }

  std::vector<Node::unique> args;
  args.resize(arity);

  // pass arguments to function
  for (int i = arity - 1; i >= 0; i--) {
    args[i] = std::move(stack.top());
    stack.pop();
  }

  return Node::unique(new FunctionNode(token.value, std::move(args), token.position));
}

Node::unique Parser::process(TokenList tokens) const {
  std::stack<Token> ops;
  std::stack<Node::unique> stack;

  // parse from left to right
  while (!tokens.empty()) {
    auto token = tokens.front();
    tokens.pop_front();

    // convert number to leaf node
    if (token.type == TokenType::NUMBER) {
      // put a number on stack
      stack.emplace(new ValueNode(token.value, token.position));

      // check if implicit multiplication
      if (!tokens.empty() &&
          (tokens.front().type == TokenType::IDENTIFIER ||
           tokens.front().type == TokenType::BRACKET_OPENING)) {
        // insert multiplication
        tokens.emplace_front(TokenType::OPERATOR, tokens.front().position, "*");
      }
    } else
    // create symbol or function
    if (token.type == TokenType::IDENTIFIER) {
      // check if function (must be followed by brackets)
      if (!tokens.empty() && tokens.front().type == TokenType::BRACKET_OPENING) {
        // function must be registered
        if (functions.find(token.value) != functions.end()) {
          ops.push(token);
        } else {
          throw UnknownFunctionError(token.value, token.position);
        }
      } else {
        // must be a variable/symbol
        stack.emplace(new SymbolNode(token.value, token.position));
      }
    } else
    // operator creates a function node
    if (token.type == TokenType::OPERATOR) {
      // any waiting
      while (!ops.empty()) {
        // must be lower precedence
        if ((ops.top().type == TokenType::OPERATOR || ops.top().type == TokenType::IDENTIFIER) &&
            lower_precedence(token, ops.top())) {
          // create function with args
          stack.emplace(create_function(ops.top(), stack));
          ops.pop();
        } else {
          break;
        }
      }

      if (operators.find(token.value) != operators.end()) {
        // new operator
        ops.push(token);
      } else {
        throw UnknownOperatorError(token.value, token.position);
      }
    } else
    // mark bracket
    if (token.type == TokenType::BRACKET_OPENING) {
      ops.push(token);
    } else
    // finish bracket
    if (token.type == TokenType::BRACKET_CLOSING) {
      bool found = false;
      while (!ops.empty()) {
        if (ops.top().type == TokenType::BRACKET_OPENING) {
          found = true;
          ops.pop();
          break;
        } else {
          // create args
          stack.emplace(create_function(ops.top(), stack));
          ops.pop();
        }
      }
      if (!found) {
        throw MissingBracketError(token.position);
      }
    }
  }

  // put remaining
  while (!ops.empty()) {
    if (ops.top().type == TokenType::BRACKET_OPENING) {
      throw UnbalancedBracketError(ops.top().position);
    }

    stack.emplace(create_function(ops.top(), stack));
    ops.pop();
  }

  // everything shoudl become a single expression
  if (stack.size() > 1) {
    while (stack.size() >= 2)
      stack.pop();
    throw ParsingError("Only single expression is allowed", stack.top()->position);
  } else
  if (stack.empty()) {
    throw EmptyExpressionError();
  }


  return std::move(stack.top());
}

}
}
