#include <iostream>
#include "calculator/tokenizer.hpp"

using namespace XX;

int main(int argc, char** argv) {
  // Calculator::Tokenizer tokenizer;
  // Calculator::Parser parser(tokenizer);
  // Calculator::Evaluator evaluator;
  //
  // evaluator.map_operator("+", "addition");
  // evaluator.map_operator("-", "substraction");
  // evaluator.map_operator("*", "multiplication");
  // evaluator.map_operator("/", "division");
  //
  // evaluator.register_function("log10", [](const Node& node) {
  //   if (node.parameters.size() == 1) {
  //     return Calculator::Node::create_value(log10(node.parameters.first()));
  //   } else {
  //     throw Calculator::ArgumentsError("log10 expects a single argument");
  //   }
  // });
  //
  // while (!eof) {
  //   Tokens t = tokenizer.process(line);
  //   Tree tree = parser.process(line);
  //
  //   evaluator.process(tree);
  // }
  //

  Calculator::Tokenizer tokenizer;

  std::string line;
  while (std::getline(std::cin, line)) {
    Calculator::TokenList tokens = tokenizer.process(line);

    std::cout << tokens << std::endl;
  }

  return EXIT_SUCCESS;
}
