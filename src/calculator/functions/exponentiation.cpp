#include "../functions.hpp"
#include "../errors.hpp"

#include <cmath>

namespace XX {
namespace Calculator {
namespace Functions {

Value exponentiation(std::vector<Value> const& args) {
  size_t base_degree = args[0].degree();
  size_t exponent_degree = args[1].degree();

  if (exponent_degree > 0) {
    throw ExponentationError("Unable to perform complex exponentation - only constant polynomials supported");
  } else
  if (base_degree == 0) {
    return Value(pow(args[0][0], args[1][0]));
  } else
  if (base_degree == 1 && args[0][0] == 0) {
    Value v;
    v[args[1][0]] = 1.0;
    return v;
  } else {
    Value v = args[0];
    size_t exponent = args[1][0];
    for (size_t i = 1; i < exponent; i++) {
      v *= args[0];
    }
    return v;
  }
}

}
}
}
