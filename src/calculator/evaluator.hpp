#include <functional>
#include <vector>
#include <map>

#include "tokenizer.hpp"

#pragma once

namespace XX {
namespace Calculator {

class Evaluator {
  public:

  void register_function(std::string const& name, size_t arity, std::function<double(std::vector<double> const&)> f);

  double process(TokenList& tokens);

  private:

  struct Function {
    size_t arity;
    std::function<double(std::vector<double> const&)> handle;

    Function(size_t arity, std::function<double(std::vector<double> const&)> handle) : arity(arity), handle(handle) { }
  };
  std::map<std::string, Function> functions;
};

}
}
