#include <functional>
#include <vector>
#include <map>

#include "tokenizer.hpp"
#include "value.hpp"

#pragma once

namespace XX {
namespace Calculator {

class Evaluator {
  public:

  void register_function(std::string const& name, size_t arity, std::function<Value(std::vector<Value> const&)> f);

  Value process(TokenList& tokens);

  private:

  struct Function {
    size_t arity;
    std::function<Value(std::vector<Value> const&)> handle;

    Function(size_t arity, std::function<Value(std::vector<Value> const&)> handle) : arity(arity), handle(handle) { }
  };
  std::map<std::string, Function> functions;
};

}
}
