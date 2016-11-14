#include <iostream>

#include "calculator/tokenizer.hpp"
#include "calculator/parser.hpp"
#include "calculator/linear_solver.hpp"

using namespace XX;

int main(int argc, char** argv) {
  Calculator::Tokenizer tokenizer;
  Calculator::Parser parser;
  Calculator::LinearSolver solver(tokenizer, parser);

  std::string line;
  while (std::getline(std::cin, line)) {
    try {
      std::cout << std::string(solver.process(line)) << std::endl;
    }
    catch (Calculator::ValueError& error) {
      std::cerr << "[VALUE] " << error.what() << std::endl;
    }
    catch (Calculator::EvaluationError& error) {
      std::cerr << "[EVALUATION] " << error.what() << std::endl;
    }
    catch (Calculator::ParsingError& error) {
      std::cerr << "[PARSING] " << error.what() << std::endl;
    }

  }

  return EXIT_SUCCESS;
}
