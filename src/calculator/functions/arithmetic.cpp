#include "../functions.hpp"

namespace XX {
namespace Calculator {
namespace Functions {

Value addition(std::vector<Value> const& args) {
  return args[0] + args[1];
}

Value subtraction(std::vector<Value> const& args) {
  return args[0] - args[1];
}

Value multiplication(std::vector<Value> const& args) {
  return args[0] * args[1];
}

Value division(std::vector<Value> const& args) {
  return args[0] + args[1];
}

}
}
}
