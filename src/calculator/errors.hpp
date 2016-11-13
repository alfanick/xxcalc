#include <exception>

#pragma once

namespace XX {
namespace Calculator {

class ParsingError : public std::runtime_error {
  public:
  ParsingError(std::string const& msg, size_t position) : std::runtime_error(msg + " at " + std::to_string(position) + " character") { }
};

class EvaluationError : public std::runtime_error {
  public:
  EvaluationError(std::string const& msg) : std::runtime_error(msg) { }
  EvaluationError(std::string const& msg, size_t position) : EvaluationError(msg + " at " + std::to_string(position)) { }
};

class EmptyExpressionError : public ParsingError {
  public:
  EmptyExpressionError() : ParsingError("Empty expression provided", 0) {}
};

class UnknownOperatorError : public ParsingError {
  public:
  UnknownOperatorError(std::string const& value, size_t position) : ParsingError("Unknown operator '" + value + "'", position) { }
};

class UnknownFunctionError : public EvaluationError {
  public:
  UnknownFunctionError(std::string const& value, size_t position) : EvaluationError("Unknown function '" + value + "'", position) { }
};

class ArgumentMissingError : public EvaluationError {
  public:
  ArgumentMissingError(std::string const& value, size_t position) : EvaluationError("Argument is missing for function '"+value+"'", position) { }
};

class OperandMissingError : public EvaluationError {
  public:
  OperandMissingError(std::string const& value, size_t position) : EvaluationError("Operand is missing for '"+value+"'", position) { }
};

class MissingBracketError : public ParsingError {
  public:
  MissingBracketError(size_t position) : ParsingError("Bracket is missing", position) { }
};

}
}
