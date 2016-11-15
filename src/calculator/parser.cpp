#include "parser.hpp"

#include <limits>

namespace XX {
namespace Calculator {

void Parser::register_operator(std::string const& value, int p, int a) {
  operators.emplace(value, Operator(p, a));
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

TokenList Parser::process(TokenList&& tokens) const {
  std::stack<Token> ops;
  TokenList output;

  // parse from left to right
  while (!tokens.empty()) {
    auto token = tokens.front();
    tokens.pop_front();

    // convert number to leaf node
    if (token.type == TokenType::NUMBER) {
      // push number
      output.push_back(token);

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
        ops.push(token);
      } else {
        // must be a variable/symbol
        output.push_back(token);
      }
    } else
    // separators
    if (token.type == TokenType::SEPARATOR) {
      while (!ops.empty() && ops.top().type != TokenType::BRACKET_OPENING) {
        output.push_back(ops.top());
        ops.pop();
      }
    }
    // operator creates a function node
    if (token.type == TokenType::OPERATOR) {
      // any waiting
      while (!ops.empty()) {
        // must be lower precedence
        if ((ops.top().type == TokenType::OPERATOR ||
             ops.top().type == TokenType::IDENTIFIER) &&
            lower_precedence(token, ops.top())) {
          // push args
          output.push_back(ops.top());
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
          output.push_back(ops.top());
          ops.pop();
        }
      }
      if (!found) {
        throw MissingBracketError(token.position);
      }
    } else
    if (token.type == TokenType::UNKNOWN) {
      throw ParsingError("Unknown token", token.position);
    }
  }

  // put remaining
  while (!ops.empty()) {
    if (ops.top().type == TokenType::BRACKET_OPENING) {
      throw MissingBracketError(ops.top().position);
    }

    output.push_back(ops.top());
    ops.pop();
  }

  if (output.empty()) {
    throw EmptyExpressionError();
  }

  return output;
}

}
}
