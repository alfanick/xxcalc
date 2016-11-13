#include <exception>

#pragma once

namespace XX {
namespace Calculator {

class ParsingError : public std::runtime_error {
  public:
  ParsingError(std::string const& msg, size_t position) : std::runtime_error(msg + " at " + std::to_string(position+1)) { }
};

class UnknownOperatorError : public ParsingError {
  public:
  UnknownOperatorError(std::string const& value, size_t position) : ParsingError("Unknown operator '" + value + "'", position) { }
};

class UnknownFunctionError : public ParsingError {
  public:
  UnknownFunctionError(std::string const& value, size_t position) : ParsingError("Unknown function '" + value + "'", position) { }
};

class ArgumentMissingError : public ParsingError {
  public:
  ArgumentMissingError(std::string const& value, size_t position) : ParsingError("Argument is missing for function '"+value+"'", position) { }
};

class OperandMissingError : public ParsingError {
  public:
  OperandMissingError(std::string const& value, size_t position) : ParsingError("Operand is missing for '"+value+"'", position) { }
};

class MissingBracketError : public ParsingError {
  public:
  MissingBracketError(size_t position) : ParsingError("Bracket is missing", position) { }
};

class UnbalancedBracketError : public ParsingError {
  public:
  UnbalancedBracketError(size_t position) : ParsingError("Unexpected bracket", position) { }
};

}
}
