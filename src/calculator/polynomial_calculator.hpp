#include "tokenizer.hpp"
#include "parser.hpp"
#include "evaluator.hpp"

#pragma once

namespace XX {
namespace Calculator {

/**
 * Polynomial calculator computes value of mathematical
 * expression consisting of polynomials, basic operations
 * and some functions. A symbol 'x' is used as a variable
 * in a polynomial.
 *
 * Operations such as addition, subtraction, multiplication,
 * division and exponentiation are supported by default.
 * Constants pi and e are defined, functions log(value, base)
 * and log10(value) are supported, but only for constant
 * polynomials (double values). Additionally a function ans
 * is defined and it return value of previous evaluation.
 *
 * The functionality can be easily extended by registration
 * of new operators, functions and constants. If depending
 * on used tokenizer and parser, variety of input forms can
 * be used (though only popular infix form is implemented a
 * part of the project).
 */
class PolynomialCalculator {
  public:

  /**
   * Creates an instance of the calculator. It is created
   * with support of addition (+), subtraction (-),
   * multiplication (*), division (/) and exponentiation (^)
   * operators. Functions log and log10 are available registered
   * here, as well as pi, e constants and 'x' symbol (which
   * represents a polynomial of degree 1 with empty constant
   * term).
   *
   * Depending on provided tokenizer and parser, variety of
   * syntax can be supported.
   *
   * @param tokenizer Instance of tokenizer used for lexing
   *                  the input expression
   * @param parser Instance of parser processing tokenized
   *               expression
   */
  PolynomialCalculator(Tokenizer& tokenizer, Parser& parser);

  /**
   * Processes the input expression and returns its computed
   * value. The input is tokenized, parsed and than evaluated.
   * Result of computation is returned and stored as last_value
   * for further usage.
   *
   * If DEBUG macro symbol is defined, tokens before and after
   * parsing are printed to stderr.
   *
   * @param line Expression to be processed (its syntax depends
   *             on used tokenizer and parser)
   * @return Computed polynomial
   */
  Value process(std::string const& line);

  /**
   * Registers new operator. The operator is registered with
   * the parser and handler is registered with the evaluator.
   *
   * Operator is assumed to always require two operands (its
   * arity is equal two), however one must specify precedence
   * and associativity in order to parse it properly.
   *
   * @param name Symbol representing the operator
   * @param precedence Priority of operator
   * @param associativity Direction of associativity
   * @param f Handler for the operator (always takes two args)
   */
  void register_operator(std::string const& name, int precedence, int associativity,
                         std::function<Value(std::vector<Value> const&)> f);

  /**
   * Registers new function. The functions is registered with
   * the evaluator.
   *
   * A functions arity must be defined in order to be parsed
   * properly. Handling function is always called with required
   * number of arguments.
   *
   * @param name Name of function
   * @param arity Number of arguments
   * @param f Handler for the function
   */
  void register_function(std::string const& name, unsigned long arity,
                         std::function<Value(std::vector<Value> const&)> f);

  /**
   * Registers new constant. The evaluator replaces identifier matching
   * constant with its value.
   *
   * @param name Name of constant
   * @param value Value of constant
   */
  void register_constant(std::string const& name, Value value);

  /**
   * Result of last evaluation.
   */
  Value last_value;

  private:

  //! Tokenizer used for processing
  Tokenizer& tokenizer;

  //! Parser used for processing
  Parser& parser;

  //! Evaluator of parsed tokens
  Evaluator evaluator;
};

}
}
