#include "parser.hpp"

#include <stack>

namespace XX {
namespace Calculator {

Parser::Parser() {
  register_operator("+", 1, -1);
  register_operator("-", 1, -1);
  register_operator("*", 5, -1);
  register_operator("/", 5, -1);
  register_operator("^", 10, 1);
  register_function("log10", 1);
  register_function("smth", 3);
}

void Parser::register_operator(std::string const& value, int p, int a) {
  operators.emplace(value, Operator(p, a));
  functions.emplace(value, Function(2));
}

void Parser::register_function(std::string const& value, int a) {
  functions.emplace(value, Function(a));
  operators.emplace(value, Operator(10000, -1));
}

bool Parser::lower_precedence(Token const& a, Token const& b) const {
  auto operator_a = operators.find(a.value);
  if (operator_a == operators.end())
    throw UnknownOperatorError(a.value, a.position);

  auto operator_b = operators.find(b.value);
  if (operator_b == operators.end())
    throw UnknownOperatorError(b.value, b.position);

  return (operator_a->second.associativity == -1 &&
          operator_a->second.precedence <= operator_b->second.precedence) ||
         (operator_a->second.associativity == 1 &&
          operator_a->second.precedence < operator_b->second.precedence);
}

Node::unique Parser::process(TokenList tokens) const {
  std::stack<Token> ops;
  std::stack<std::unique_ptr<Node>> s;

  while (!tokens.empty()) {
    auto token = tokens.front();
    tokens.pop_front();

    if (token.type == TokenType::NUMBER) {
      s.emplace(new ValueNode(token.value));

      // check if implicit multiplication
      if (!tokens.empty() &&
          (tokens.front().type == TokenType::IDENTIFIER ||
           tokens.front().type == TokenType::BRACKET_OPENING)) {
        // insert multiplication
        tokens.emplace_front(TokenType::OPERATOR, tokens.front().position, "*");
      }
    } else
    if (token.type == TokenType::IDENTIFIER) {
      // check if function
      // TODO validate with functions list
      if (!tokens.empty() && tokens.front().type == TokenType::BRACKET_OPENING) {
        ops.push(token);
      } else {
        s.emplace(new SymbolNode(token.value));
      }
    } else
    if (token.type == TokenType::OPERATOR) {
      while (!ops.empty()) {
        if ((ops.top().type == TokenType::OPERATOR || ops.top().type == TokenType::IDENTIFIER) &&
            lower_precedence(token, ops.top())) {

            auto f = functions.find(ops.top().value);
            std::vector<std::unique_ptr<Node>> args;
            args.resize(f->second.arity);

            for (int i = f->second.arity - 1; i >= 0; i--) {
              args[i] = std::move(s.top());
              s.pop();
            }

            s.emplace(new FunctionNode(ops.top().value, std::move(args)));
            ops.pop();
        } else {
          break;
        }
      }

      ops.push(token);
    } else
    if (token.type == TokenType::BRACKET_OPENING) {
      ops.push(token);
    } else
    if (token.type == TokenType::BRACKET_CLOSING) {
      while (!ops.empty()) {
        if (ops.top().type == TokenType::BRACKET_OPENING) {
          ops.pop();
          break;
        } else {
            auto f = functions.find(ops.top().value);
            std::vector<std::unique_ptr<Node>> args;
            args.resize(f->second.arity);

            for (int i = f->second.arity - 1; i >= 0; i--) {
              args[i] = std::move(s.top());
              s.pop();
            }

            s.emplace(new FunctionNode(ops.top().value, std::move(args)));
          ops.pop();
        }
      }
    }
  }

  while (!ops.empty()) {
            auto f = functions.find(ops.top().value);
            std::vector<std::unique_ptr<Node>> args;
            args.resize(f->second.arity);

            for (int i = f->second.arity - 1; i >= 0; i--) {
              args[i] = std::move(s.top());
              s.pop();
            }

            s.emplace(new FunctionNode(ops.top().value, std::move(args)));
    ops.pop();
  }

  while (!s.empty()) {
    auto k = std::move(s.top());
    s.pop();

    std::cout << k->tree(0) << std::endl;
  }

  return ValueNode::unique(new ValueNode("2"));
}

}
}
