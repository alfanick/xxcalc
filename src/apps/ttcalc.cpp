#include <iostream>

#ifdef READLINE_FOUND
#include <readline/readline.h>
#include <readline/history.h>
#define HISTORY_FILE ".xxcalc_history"
#endif

#include "calculator/tokenizer.hpp"
#include "calculator/parser.hpp"
#include "calculator/linear_solver.hpp"

using namespace XX;

int main(int argc, char** argv) {
  Calculator::Tokenizer tokenizer;
  Calculator::Parser parser;
  Calculator::LinearSolver solver(tokenizer, parser);

#ifdef READLINE_FOUND
  ::read_history(HISTORY_FILE);

  char* line;
  while ((line = ::readline(">>> "))) {
    ::add_history(line);
#else
  std::string line;
  while (std::getline(std::cin, line)) {
#endif

    try {
      Calculator::Value result = solver.process(line);

      if (solver.solved)
        std::cout << "x=";

      std::cout << std::string(result) << std::endl;
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

#ifdef READLINE_FOUND
    free(line);
#endif
  }

#ifdef READLINE_FOUND
  ::write_history(HISTORY_FILE);
#endif

  return EXIT_SUCCESS;
}
