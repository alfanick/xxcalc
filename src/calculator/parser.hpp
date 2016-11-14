#include "tokenizer.hpp"
#include "errors.hpp"

#include <map>
#include <stack>

#pragma once

namespace XX {
namespace Calculator {

/**
 * Parser takes tokenized input and converts it from common infix
 * notation into Reverse Polish Notation. RPN is easier to evaluate
 * in later steps. RPN form was chosen instead of Abstract Syntax
 * Tree as evalution of the former is more complex and requires
 * a large stack size.
 *
 * The parser performs error checking - it requires parentheses
 * to be balanced, operators must be registered and they require
 * two operands (left and right).
 */
class Parser {
  public:

  /**
   * Registers new operator to the parser. An operator must be
   * registered otherwise a UnknownOperatorError is throwed
   * when a token representing unknown operator is found.
   *
   * Operators with smallest precedence are at the top of
   * parsing tree.
   *
   * @param name Name of operator (value of token)
   * @param precedence Precedence (priority) of operator
   * @param associativy Direction of associativity (negative for left,
   *                    positive for right)
   */
  void register_operator(std::string const& name, int precedence, int associativity);

  /**
   * Parses list of tokens into another list of tokens but in RPN
   * notation. Variety of syntax checks are performed during the
   * parsing process. Operators must be registered before they
   * can be parsed, as precedence and associativity must be known
   * beforehand.
   *
   * Parser is implementing the shunting-yard algorithm by Edsger
   * Dijkstra. Its computation is linear O(n) and no recursion
   * is performed.
   *
   * Shorthand syntax for multiplcation (ie. 2x) is supported,
   * multiplication operator is inserted if required.
   *
   * @throw UnknownOperatorError When operator is not registered
   * @throw MissingBracketError When brackets are unbalanced
   * @throw ParsingError When multiple expressions are provided
   *                     or other error occurs
   * @throw EmptyExpressionError When no expression is provdied
   * @param tokens List of tokens representing a single expression
   * @return Tokens in RPN
   */
  virtual TokenList process(TokenList&& tokens) const;

  private:

  /**
   * Compares precedence of two operators. Operator A has lower
   * precedence than operator B if its precedence is smaller or
   * equal or striclty smaller in case of right-associative
   * operator.
   *
   * @param a First token representing operator
   * @param b Another token representing operator
   * @return True if a has lower precedence than b
   */
  bool lower_precedence(Token const& a, Token const& b) const;

  /**
   * Container for operator metadata
   */
  struct Operator {
    //! Operator precedence
    int precedence;
    //! Operator associativity (negative for left, positive for right)
    int associativity;

    /**
     * Creates operator with given precende and associativy
     *
     * @param p Precedence
     * @param a Associativity (negative for left, positive for right)
     */
    Operator(int p, int a) : precedence(p), associativity(a) { }
  };

  //! Stores registered operators with their metadata
  std::map<std::string, Operator> operators;
};

}
}
