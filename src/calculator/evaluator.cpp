#include "evaluator.hpp"

#include "node.hpp"

namespace XX {
namespace Calculator {


void Evaluator::register_function(std::string const& name, std::function<std::unique_ptr<Node>(std::vector<std::unique_ptr<Node>> const&)> f) {
  functions[name] = f;
}

std::unique_ptr<Node> Evaluator::call(std::string const& symbol) {
  return nullptr;
}

std::unique_ptr<Node> Evaluator::call(std::string const& name, std::vector<std::unique_ptr<Node>> const& arguments) {
  return functions[name](std::move(arguments));
}

}
}
