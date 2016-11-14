#include <exception>
#include <string>

#pragma once

namespace XX {
namespace Calculator {

class Error : public std::runtime_error {
  public:
  Error(std::string const& msg) : std::runtime_error(msg) { }
};

class ParsingError : public Error {
  public:
  ParsingError(std::string const& msg, size_t position) : Error(msg + " at " + std::to_string(position)) { }
};

class EvaluationError : public Error {
  public:
  EvaluationError(std::string const& msg) : Error(msg) { }
  EvaluationError(std::string const& msg, size_t position) : EvaluationError(msg + " at " + std::to_string(position)) { }
};

class ConflictingNameError : public EvaluationError {
  public:
  ConflictingNameError(std::string const& msg) : EvaluationError(msg) { }
};

class SolverError : public EvaluationError {
  public:
  SolverError(std::string const& msg) : EvaluationError(msg) { }
};

class NonLinearEquation : public SolverError {
  public:
  NonLinearEquation() : SolverError("Provided expression is not linear - the solver can process only linear equations") { }
};

class NoSymbolFound : public SolverError {
  public:
  NoSymbolFound() : SolverError("No symbol found in the expression - use x to mark the symbol") { }
};

class ExpressionIsTautology : public SolverError {
  public:
  ExpressionIsTautology() : SolverError("Provided expression is a tautology") { }
};

class NonSolvableExpression : public SolverError {
  public:
  NonSolvableExpression() : SolverError("Provided expression has no valid solutions") { }
};

class ValueError : public Error {
  public:
  ValueError(std::string const& msg) : Error(msg) { }
};

class PolynomialCastError : public ValueError {
  public:
  PolynomialCastError() : ValueError("Cannot cast complex polynomial to double value") { }
};

class ExponentationError : public EvaluationError {
  public:
  ExponentationError(std::string const& msg) : EvaluationError(msg) { }
};

class PolynomialDivisionError : public ValueError {
  public:
  PolynomialDivisionError() : ValueError("Divisor must be same or lower degree than dividend") { }
};

class EmptyExpressionError : public ParsingError {
  public:
  EmptyExpressionError() : ParsingError("Empty expression provided", 0) {}
};

class UnknownOperatorError : public ParsingError {
  public:
  UnknownOperatorError(std::string const& value, size_t position) : ParsingError("Unknown operator '" + value + "'", position) { }
};

class UnknownSymbolError : public EvaluationError {
  public:
  UnknownSymbolError(std::string const& value, size_t position) : EvaluationError("Unknown symbol '" + value + "'", position) { }
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
