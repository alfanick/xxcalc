#include "value.hpp"

#pragma once

namespace XX {
namespace Calculator {
namespace Functions {

Value addition(std::vector<Value> const& args);
Value subtraction(std::vector<Value> const& args);
Value multiplication(std::vector<Value> const& args);
Value division(std::vector<Value> const& args);
Value exponentiation(std::vector<Value> const& args);

Value log10(std::vector<Value> const&);
Value log(std::vector<Value> const&);

}
}
}
