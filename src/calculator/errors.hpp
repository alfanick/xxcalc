#include <exception>

#pragma once

namespace XX {
namespace Calculator {

class ParsingError : public std::runtime_error {
  public:
  ParsingError(std::string const& msg, size_t position) : std::runtime_error(msg + " at " + std::to_string(position)) { }
};

class UnexpectedIdentifierError : public ParsingError {
  public:
  UnexpectedIdentifierError(std::string const& value, size_t position) : ParsingError("Unexpected identifier '" + value + "'", position) { }
};

class UnexpectedNumberError : public ParsingError {
  public:
  UnexpectedNumberError(std::string const& value, size_t position) : ParsingError("Unexpected number '" + value + "'", position) { }
};

class UnexpectedOperatorError : public ParsingError {
  public:
  UnexpectedOperatorError(std::string const& value, size_t position) : ParsingError("Unexpected operator '" + value + "'", position) { }
};

class UnknownOperatorError : public ParsingError {
  public:
  UnknownOperatorError(std::string const& value, size_t position) : ParsingError("Unknown operator '" + value + "'", position) { }
};

class UnexpectedOpeningBracketError : public ParsingError {
  public:
  UnexpectedOpeningBracketError(size_t position) : ParsingError("Unexpected opening bracket", position) {}
};

class UnexpectedClosingBracketError : public ParsingError {
  public:
  UnexpectedClosingBracketError(size_t position) : ParsingError("Unexpected closing bracket", position) {}
};

}
}
