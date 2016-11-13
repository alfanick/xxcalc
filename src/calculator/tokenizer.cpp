#include "tokenizer.hpp"

#include <iostream>
#include <cstring>

namespace XX {
namespace Calculator {

TokenList Tokenizer::process(std::string const& line) const {
  TokenList tokens;
  size_t position = 0;

  // Process from left to right
  while (position < line.size()) {
    switch (line[position]) {
      // Brackets
      case '(':
        tokens.emplace_back(TokenType::BRACKET_OPENING, position);
        position++;
        break;
      case ')':
        tokens.emplace_back(TokenType::BRACKET_CLOSING, position);
        position++;
        break;
      // Operators
      case '+':
      case '-':
      case '/':
      case '*':
      case '^':
      case '=':
        tokens.emplace_back(TokenType::OPERATOR, position, line[position]);
        position++;
        break;
      // Separator
      case ',':
        tokens.emplace_back(TokenType::SEPARATOR, position);
        position++;
        break;
      // Skip white characters
      case ' ':
      case '\t':
      case '\r':
      case '\n':
        position++;
        continue;
      // Otherwise a number, an identifier or unknown token
      default:
        // A number must start with a digit or a decimal dot
        if (std::isdigit(line[position]) || line[position] == '.') {
          tokens.emplace_back(extract_number(line, position));
          position += tokens.back().value.length();
        } else
        // An identifier must start with a letter or an underscore
        if (std::isalpha(line[position]) || line[position] == '_') {
          tokens.emplace_back(extract_identifier(line, position));
          position += tokens.back().value.length();
        } else {
          tokens.emplace_back(TokenType::UNKNOWN, position, line[position]);
          position++;
        }
    }
  }

  // Convert infinity and nan
  convert_special_numbers(tokens);
  // Merge sign operators with numbers
  merge_signs(tokens);

  return tokens;
}

Token Tokenizer::extract_number(std::string const& line, size_t position) const {
  Token token(TokenType::NUMBER, position);

  // Dot is accepted once
  bool accept_dot = true;
  // Exponent is accepted
  bool accept_exponent = true;
  // Sign is not accepted initially
  bool accept_sign = false;

  size_t current = position;

  // Process from left to right
  while (current < line.size()) {
    // Dot is accepted only once and only before exponent specification
    if (accept_dot && line[current] == '.' && accept_exponent) {
      accept_dot = false;
      current++;
    } else
    // Exponent is accepted only once and it can be followed by sign
    if (accept_exponent && (line[current] == 'E' || line[current] == 'e')) {
      accept_exponent = false;
      accept_sign = true;
      current++;
    } else
    // Sign is accepted only once
    if (accept_sign && (line[current] == '+' || line[current] == '-')) {
      accept_sign = false;
      current++;
    } else
    // Digits are always accepted
    if (std::isdigit(line[current])) {
      accept_sign = false;
      current++;
    } else {
      break;
    }
  }

  // Copy extracted number
  token.value = line.substr(position, current - position);

  return token;
}

Token Tokenizer::extract_identifier(std::string const& line, size_t position) const {
  Token token(TokenType::IDENTIFIER, position);

  size_t current = position;

  // Process from left to right
  while (current < line.size()) {
    // First character must be a letter or an underscore, numbers can follow
    if (std::isalpha(line[current]) || line[current] == '_' ||
        (std::isalnum(line[current]) && current > position)) {
      current++;
    } else {
      break;
    }
  }

  // Copy extracted identifier
  token.value = line.substr(position, current - position);

  return token;
}

void Tokenizer::convert_special_numbers(TokenList& tokens) const {
  // Process from left to right
  for (auto token = tokens.begin(); token != tokens.end(); ++token) {
    // Processing only identifiers
    if (token->type != TokenType::IDENTIFIER)
      continue;

    // Convert infinity and nan to number
    if (strcasecmp(token->value.c_str(), "inf") == 0 ||
        strcasecmp(token->value.c_str(), "infinity") == 0 ||
        strcasecmp(token->value.c_str(), "nan") == 0) {
      token->type = TokenType::NUMBER;
    }
  }
}

void Tokenizer::merge_signs(TokenList& tokens) const {
  // Process from right to left
  for (auto token = tokens.end(); token != tokens.begin(); --token) {
    // Look for sign only before a number
    if (token->type != TokenType::NUMBER)
      continue;

    auto preceeding = std::prev(token);

    if (preceeding != tokens.end()) {
      // Only interested in sign operators
      if (preceeding->type == TokenType::OPERATOR &&
          (preceeding->value == "-" || preceeding->value == "+")) {
        auto prev = std::prev(preceeding);

        // An operator is the sign operator if it is directly at
        // the beginning or after opening bracket or after another operator.
        if (prev == tokens.end() ||
            prev->type == TokenType::BRACKET_OPENING ||
            prev->type == TokenType::OPERATOR) {
          // Merge sign with the number
          token->value.insert(0, preceeding->value);
          // Update position of number with a sign
          token->position = preceeding->position;
          // Remove sign operator
          tokens.erase(preceeding);
        }
      }
    }
  }
}

std::ostream& operator<<(std::ostream &os, Token const& t) {
  os << t.position << ' ';

  switch (t.type) {
    case TokenType::BRACKET_OPENING:
      os << "BRACKET_OPENING";
      break;
    case TokenType::BRACKET_CLOSING:
      os << "BRACKET_CLOSING";
      break;
    case TokenType::OPERATOR:
      os << "OPERATOR " << t.value;
      break;
    case TokenType::IDENTIFIER:
      os << "IDENTIFIER '" << t.value << "'";
      break;
    case TokenType::NUMBER:
      os << "NUMBER " << t.value;
      break;
    case TokenType::SEPARATOR:
      os << "SEPARATOR";
      break;
    case TokenType::UNKNOWN:
      os << "UNKNOWN '" << t.value << "'";
      break;
  }

  return os;
}

std::ostream& operator<<(std::ostream &os, TokenList const& tokens) {
  size_t level = 0;

  for (const auto& token : tokens) {
    if (token.type == TokenType::BRACKET_CLOSING && level > 0) {
      level--;
    }

    os << std::string(level, '\t') << token << std::endl;

    if (token.type == TokenType::BRACKET_OPENING) {
      level++;
    }
  }

  return os;
}

}
}
