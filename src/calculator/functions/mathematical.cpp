#include "../functions.hpp"

#include <cmath>

namespace XX {
namespace Calculator {
namespace Functions {

Value log10(std::vector<Value> const& args) {
  return std::log10(double(args[0]));
}

Value log(std::vector<Value> const& args) {
  return std::log(double(args[0])) / std::log(double(args[1]));
}

}
}
}
