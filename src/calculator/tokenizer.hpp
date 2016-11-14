#include <list>
#include <string>
#include <iostream>

#pragma once

namespace XX {
namespace Calculator {

/**
 * Tokens used in an input to the calculator
 */
enum class TokenType {
  //! Opening bracket '('
  BRACKET_OPENING,
  //! Closing bracket ')'
  BRACKET_CLOSING,

  //! Floating point number (including scientific notation)
  NUMBER,
  //! Operator (such as '+', '-', '*', '/' or '=')
  OPERATOR,
  //! Text identifier (of function or variable)
  IDENTIFIER,
  //! Argument separator
  SEPARATOR,
  //! Other not known input
  UNKNOWN
};


/**
 * Token is a recognized part of input that can be evaluated as
 * a single atom. Tokens are smallest units found in the input.
 * Tokens are used later on by Parser to build an abstract syntax
 * tree.
 */
struct Token {
  //! Type of token
  TokenType type;
  //! Position (relative to the original input)
  size_t position;
  //! A value associated with a token (such as operator, number or identifier text)
  std::string value;

  /**
   * Creates a token of given type found at given position.
   *
   * @param type Type of token
   * @param position Position in original input
   */
  Token(TokenType type, size_t position) : type(type), position(position) {}

  /**
   * Creates a token of given type found at given position
   * with single char value.
   *
   * @param type Type of token
   * @apram position Position in original input
   * @param value Value of token (useful for operators)
   */
  Token(TokenType type, size_t position, char value) : type(type), position(position), value(std::string(1, value)) {}

  /**
   * Creates a token of given type found at given position
   * with given text value.
   *
   * @param type Type of token
   * @param position Position in original input
   * @param value Value of token (useful for numbers or identifiers)
   */
  Token(TokenType type, size_t position, std::string &&value) : type(type), position(position), value(value) {}
};


/**
 * Ordered list of tokens - can be used to represent an expression.
 */
typedef std::list<Token> TokenList;


/**
 * Tokenizer takes a line of text representing an expression and
 * converts it to a list of tokens. Such list can be processed
 * by a parser into an abstract syntax tree which is evaluated
 * later on.
 *
 * This is a basic tokenizer which recognizes operators such as
 * addition (+), subtraction (-), multiplication (*), division (/),
 * power (^) and equal (=), floating point numbers (including signed
 * numbers with scientific notation), text identifiers (for
 * variables and functions) and brackets.
 *
 * No syntax checks are performed at tokenization level as tokens
 * are not bounded by any specific grammar.
 */
class Tokenizer {
  public:

  /**
   * Converts the text expression into a list of tokens.
   *
   * Tokenization is performed as a two step process (with
   * linear complexity O(n)). First step recognizes individual
   * tokens in the input, while the second step recognizes
   * sign operators and merges them the numbers.
   *
   * @param line Text expression
   * @return List of recognized tokens
   */
  virtual TokenList process(std::string const& line) const;

  private:

  /**
   * Extracts floating point number from the line. It assumes that
   * a dot can be used as decimal point, 'e' or 'E' as decimal exponent
   * is used in scientific notation (with sign operator allowed).
   *
   * @param line Line of text
   * @param position Starting position for extraction
   * @return Token representing found number
   */
  Token extract_number(std::string const& line, size_t position) const;

  /**
   * Extracts text identifier from the line. It assumes that a first
   * character as an underscore or an letter - any other character
   * can be an underscore, a letter or a number.
   *
   * @param line Line of text
   * @param position Starting position for extraction
   * @return Token representing found identifier
   */
  Token extract_identifier(std::string const& line, size_t position) const;

  /**
   * Converts special identifiers into numbers such as
   * Infinity or NaN value.
   *
   * @param[in,out] tokens Tokenized expression
   */
  void convert_special_numbers(TokenList& tokens) const;

  /**
   * Merges specific addition and subtraction operator with numbers,
   * therefore creating signed numbers. An operator can be used as
   * a sign of the number if it is used at very beginning of the expression,
   * after opening bracket or after another operator.
   *
   * Furthermore identifiers (such as function names or symbol names)
   * can be signed too.
   *
   * @param[in,out] tokens Tokenized expression
   */
  void merge_signs(TokenList& tokens) const;
};


/**
 * Pretty printer for a token. It includes a position, token
 * name and token value (if appropriate).
 *
 * @param os Output stream
 * @param t Token to convert
 * @return Stream with token
 */
std::ostream& operator<<(std::ostream &os, Token const& t);

/**
 * Pretty printer for a token list.
 *
 * @param os Output stream
 * @param t Token list
 * @return Stream with token list
 */
std::ostream& operator<<(std::ostream &os, TokenList const& t);

}
}
