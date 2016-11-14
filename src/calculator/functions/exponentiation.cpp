#include "../functions.hpp"
#include "../errors.hpp"

#include <cmath>

#include <iostream>

namespace XX {
namespace Calculator {
namespace Functions {
Value exponentiation(std::vector<Value> const& args) {
  unsigned long base_degree = args[0].degree();
  unsigned long exponent_degree = args[1].degree();

  if (exponent_degree > 0) {
    throw ExponentationError("Unable to perform complex exponentation - only constant polynomials supported");
  } else
  if (base_degree == 0) {
    return Value(pow(args[0][0], args[1][0]));
  } else
  if (base_degree == 1 && args[0][0] == 0) {
    Value v;
    v[args[1][0]] = pow(args[0][1], args[1][0]);
    return v;
  } else {
    Value v = args[0];
    unsigned long exponent = args[1][0];
    for (unsigned long i = 1; i < exponent; i++) {
      v *= args[0];
    }
    return v;
  }
}

}
}
}
