#include <functional>
#include <vector>
#include <map>

#include "tokenizer.hpp"
#include "value.hpp"

#pragma once

namespace XX {
namespace Calculator {

/**
 * An evaluator takes tokens tokens in Reverse Polish
 * Notation form and evaluates them as a mathematical
 * expression. Tokens representing numbers are converted
 * into values representing constant polynomials. Tokens
 * which are identifier can be either a function or a
 * constant - both of them must be registered to in order
 * to be recognized.
 *
 * There is no distinction between operators and functions,
 * an operator is just a function with name matching the
 * operator.
 */
class Evaluator {
  public:

  /**
   * Registers new function to the evaluator. A function
   * is called when token with its identifier is found.
   * Called function is guaranteed to have exact number
   * of arguments as arity is checked beforehand.
   *
   * @throw ConflictingNameError When name collides with
   *        already registered constant
   * @param name Name of function (or operator)
   * @param arity Required number of arguments
   * @param f Function handler
   */
  void register_function(std::string const& name, unsigned long arity, std::function<Value(std::vector<Value> const&)> f);

  /**
   * Registers new constant to the evaluator. A token with
   * identifier matching constant name is replaced with
   * a value of the value.
   *
   * @throw ConflictingNameError When name collides with
   *        already registered constant
   * @param name Name of constant
   * @param value Constant value
   */
  void register_constant(std::string const& name, Value value);

  /**
   * Evaluates list of tokens into a polynomial value. Tokens
   * are expected to be in RPN form. A simple stack based
   * algorithm is used.
   *
   * Each number or constant is pushed onto the working stack.
   * If token representing a function (or an operator) is
   * being processed a defined number of arguments is popped
   * of the stack and passed to the function handler. Result of
   * this function called is stored back on the stack.
   *
   * When every token has been processed a value remaining on
   * the stack is returned as a result of evaluation.
   *
   * @throw UnknownSymbolError When identifier is neither a function
   *        or a constant
   * @throw ArgumentMissingError When there are not enough arguments
   *        on the stack to fullfil function arity
   * @throw EvaluationError When multiple expression are identifier
   *        in the input
   * @param tokens Parsed input in RPN form
   * @return Evaluated value (as a polynomial)
   */
  Value process(TokenList& tokens);

  //! Process r-value reference
  Value process(TokenList&& tokens) { return process(tokens); }

  private:

  /**
   * Container for function metadata
   */
  struct Function {
    //! Arity of function (number of arguments)
    unsigned long arity;
    //! Function handle
    std::function<Value(std::vector<Value> const&)> handle;

    /**
     * Creates function of given arity
     *
     * @param arity Number of arguments
     * @param handle Function handle
     */
    Function(unsigned long arity, std::function<Value(std::vector<Value> const&)> handle) : arity(arity), handle(handle) { }
  };

  //! Registered functions
  std::map<std::string, Function> functions;

  //! Registered constants
  std::map<std::string, Value> constants;
};

}
}
