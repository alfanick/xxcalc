#include <stdexcept>
#include <string>

#pragma once

namespace XX {
namespace Calculator {

/**
 * Generic calculator error
 */
class Error : public std::runtime_error {
  public:
  Error(std::string const& msg) : std::runtime_error(msg) { }
};


/**
 * Generic parsing error
 */
class ParsingError : public Error {
  public:
  ParsingError(std::string const& msg, unsigned long position) :
    Error(msg + " at " + std::to_string(position)) { }
};

/**
 * Parsed expression contains no tokens (the input is
 * empty or consists just of brackets).
 */
class EmptyExpressionError : public ParsingError {
  public:
  EmptyExpressionError() :
    ParsingError("Empty expression provided", 0) {}
};

/**
 * Parser encountered not registered operator, but tokenizer
 * recognizes input as an operator.
 */
class UnknownOperatorError : public ParsingError {
  public:
  UnknownOperatorError(std::string const& value, unsigned long position) :
    ParsingError("Unknown operator '" + value + "'", position) { }
};

/**
 * Brackets in the input must be unbalanced (either an opening
 * or a closing bracket is missing).
 */
class MissingBracketError : public ParsingError {
  public:
  MissingBracketError(unsigned long position) :
    ParsingError("Bracket is missing", position) { }
};


/**
 * Generic value error
 */
class ValueError : public Error {
  public:
  ValueError(std::string const& msg) : Error(msg) { }
};

/**
 * Only constant polynomials (polynomials of degree 0) can be
 * directly casted to double value. Values like 2x+3 cannot be
 * casted to double.
 */
class PolynomialCastError : public ValueError {
  public:
  PolynomialCastError() :
    ValueError("Cannot cast complex polynomial to double value") { }
};

/**
 * In order to perform valid polynomial division, a degree
 * of divident must be greater or equal to a degree of divisor.
 * Otherwise result would not be a polynomial (x/x^2 = x^-1
 * which is not valid polynomial).
 */
class PolynomialDivisionError : public ValueError {
  public:
  PolynomialDivisionError() :
    ValueError("Divisor must be same or lower degree than dividend") { }
};


/**
 * Generic evaluation error (expression is tokenized and parsed,
 * but is not valid in context of evaluation).
 */
class EvaluationError : public Error {
  public:
  EvaluationError(std::string const& msg) : Error(msg) { }
  EvaluationError(std::string const& msg, unsigned long position) :
    EvaluationError(msg + " at " + std::to_string(position)) { }
};

/**
 * A name must be unique for a constant and a function. Thrown
 * when trying to register an already used name.
 */
class ConflictingNameError : public EvaluationError {
  public:
  ConflictingNameError(std::string const& msg) : EvaluationError(msg) { }
};

/**
 * Polynomials of degree greater than zero, can exponentiated only
 * by natural numbers in order to still be a polynomial. If an exponent
 * is a polynomial of degree greater than zero or not a natural number
 * this exception is thrown.
 */
class ExponentationError : public EvaluationError {
  public:
  ExponentationError(std::string const& msg) : EvaluationError(msg) { }
};

/**
 * Functions and constants must be registered with the evaluator.
 * Encountering unregistered identifier during evaluation results
 * in this error.
 */
class UnknownSymbolError : public EvaluationError {
  public:
  UnknownSymbolError(std::string const& value, unsigned long position) :
    EvaluationError("Unknown symbol '" + value + "'", position) { }
};

/**
 * If too little arguments for a function or an operator is provided
 * this error is thrown. The arity is tested during evaluation.
 */
class ArgumentMissingError : public EvaluationError {
  public:
  ArgumentMissingError(std::string const& value, unsigned long position) :
    EvaluationError("Argument is missing for function '"+value+"'", position) { }
};


/**
 * Generic solver error
 */
class SolverError : public EvaluationError {
  public:
  SolverError(std::string const& msg) : EvaluationError(msg) { }
};

/**
 * Solving operator can solve only linear equations (of a degree
 * at most one).
 */
class NonLinearEquation : public SolverError {
  public:
  NonLinearEquation() :
    SolverError("Provided expression is not linear - the solver can process only linear equations") { }
};

/**
 * A symbol must be found on left or right side of the
 * equation (one side of equation must be polynomial of
 * exactly degree one).
 */
class NoSymbolFound : public SolverError {
  public:
  NoSymbolFound() :
    SolverError("No symbol found in the expression - use x to mark the symbol") { }
};

/**
 * Provided equation is a tautology (it is always true, no
 * matter what it a value of symbol, ie. x=x). No single
 * value can be found.
 */
class ExpressionIsTautology : public SolverError {
  public:
  ExpressionIsTautology() :
    SolverError("Provided expression is a tautology") { }
};

/**
 * Provided equation has no valid solutions (it is
 * never true, ie. x=x+1). No value at all can be found.
 */
class NonSolvableExpression : public SolverError {
  public:
  NonSolvableExpression() :
    SolverError("Provided expression has no valid solutions") { }
};

}
}
