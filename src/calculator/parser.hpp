#include "node.hpp"
#include "tokenizer.hpp"
#include "errors.hpp"

#include <map>
#include <stack>

#pragma once

namespace XX {
namespace Calculator {

/**
 * Parser takes tokenized input and converts it into an abstract
 * syntax tree. Such tree obeys precedence of operators (operator
 * with the lowest procedence becomes a root of AST).
 *
 * The parser performs error checking - it requires parentheses
 * to be balanced, operators must be registered and they require
 * two operands (left and right). Functions can be registered with
 * any arity required (an error will be reported if provided
 * arguments are not sufficient).
 *
 * The Abstract Syntax Tree can be a double number ValueNode,
 * a symbol representing variable SymbolNode or a FunctionNode
 * which represents a mathematical operation of a function.
 *
 * The basic instance of the parser support only basic arithmetical
 * operations (addition, subtraction, multiplication, division and
 * exponentation).
 */
class Parser {
  public:

  /**
   * Creates an instance of parser. Registers basic mathematical
   * operations such as addition, subtraction, multiplication,
   * division and exponentation.
   */
  Parser();

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
   * Registers new function to the parser. Functions must be
   * registered as their arity is required for the parsing
   * process.
   *
   * @param name Name of function (value of identifier token)
   * @param arity Arity of function (number of arguments)
   */
  void register_function(std::string const& name, size_t arity);

  /**
   * Parses list of tokens into abstract syntax tree. The AST
   * obeys rules of precedence. Variety of syntax checks are
   * performed during the parsing process. Functions and operators
   * must be registered before they can be parsed, as precedence,
   * associativity and arity must be known beforehand.
   *
   * Parser is implementing the shunting-yard algorithm by Edsger
   * Dijkstra. Its computation is linear O(n) and no recursion
   * is performed.
   *
   * Shorthand syntax for multiplcation (ie. 2x) is supported,
   * multiplication operator is inserted if required.
   *
   * @throw UnknownFunctionError When function is not registered
   * @throw UnknownOperatorError When operator is not registered
   * @throw MissingBracketError When closing bracket is missing
   * @throw UnblaancedBracketError When brackets are unbalanced
   * @throw ParsingError When multiple expressions are provided
   *                     or other error occurs
   * @throw EmptyExpressionError When no expression is provdied
   * @param tokens List of tokens representing a single expression
   * @return Root of AST
   */
  virtual Node::unique process(TokenList tokens) const;

  private:

  /**
   * Creates function node from given token. Arguments are taken
   * from the stack. It requires exactly  as many arguments as
   * registered arity, otherwise exception is thrown. This method
   * can be used for functions or operators as there is not real
   * difference in behaviour between the two (except operators
   * always require two arguments).
   *
   * @throw OperandMissingError When token represents operator and
   *                            not enough arguments are on the stack.
   * @throw ArgumentMissingError When there are not enough arguments
   *                             on the stack.
   * @param token Token with operator or function identifier
   * @param stack Stack of nodes used as input parameters
   * @return Node representing function with given arguments
   */
  Node::unique create_function(Token const& token, std::stack<Node::unique> &stack) const;

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

  /**
   * Container for function metadata
   */
  struct Function {
    //! Arity of function (number of arguments)
    size_t arity;

    /**
     * Creates function with given arity
     *
     * @param a Arity (number of arguments)
     */
    Function(size_t a) : arity(a) { }
  };

  //! Stores registered operators with their metadata
  std::map<std::string, Operator> operators;

  //! Stores registered functions with their metadata
  std::map<std::string, Function> functions;
};

}
}
