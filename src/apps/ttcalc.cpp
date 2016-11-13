#include <iostream>
#include "calculator/tokenizer.hpp"
#include "calculator/parser.hpp"
#include "calculator/evaluator.hpp"

#include <cmath>
using namespace XX;

int main(int argc, char** argv) {
  Calculator::Tokenizer tokenizer;
  Calculator::Parser parser;
  Calculator::Evaluator evaluator;

  // evaluator.register_function("+", [&evaluator](std::vector<Calculator::Node::unique> const& args) {
  //   return Calculator::Node::unique(new Calculator::ValueNode(double(*args[0]->evaluate(evaluator))
  //                                                             + double(*args[1]->evaluate(evaluator))));
  // });
  //
  // evaluator.register_function("-", [&evaluator](std::vector<Calculator::Node::unique> const& args) {
  //   return Calculator::Node::unique(new Calculator::ValueNode(double(*args[0]->evaluate(evaluator))
  //                                                             - double(*args[1]->evaluate(evaluator))));
  // });
  //
  // evaluator.register_function("*", [&evaluator](std::vector<Calculator::Node::unique> const& args) {
  //   return Calculator::Node::unique(new Calculator::ValueNode(double(*args[0]->evaluate(evaluator))
  //                                                             * double(*args[1]->evaluate(evaluator))));
  // });
  //
  // evaluator.register_function("/", [&evaluator](std::vector<Calculator::Node::unique> const& args) {
  //   return Calculator::Node::unique(new Calculator::ValueNode(double(*args[0]->evaluate(evaluator))
  //                                                             / double(*args[1]->evaluate(evaluator))));
  // });
  //
  // evaluator.register_function("^", [&evaluator](std::vector<Calculator::Node::unique> const& args) {
  //   return Calculator::Node::unique(new Calculator::ValueNode(pow(double(*args[0]->evaluate(evaluator)), double(*args[1]->evaluate(evaluator)))));
  // });
  //
  // parser.register_function("log10", 1);
  // evaluator.register_function("log10", [&evaluator](std::vector<Calculator::Node::unique> const& args) {
  //   return Calculator::Node::unique(new Calculator::ValueNode(log10(double(*args[0]->evaluate(evaluator)))));
  // });

  std::string line;
  while (std::getline(std::cin, line)) {
    Calculator::TokenList tokens = tokenizer.process(line);

    try {
      Calculator::Node::unique tree = parser.process(std::move(tokens));
      std::cout << "ok" << std::endl;

      // std::cout << double(*tree->evaluate(evaluator)) << std::endl;
      // std::cout << tree->tree(0) << std::endl;
    }
    catch (Calculator::ParsingError& error) {
      std::cerr << "[ERROR] " << error.what() << std::endl;
    }

  }

  return EXIT_SUCCESS;
}
