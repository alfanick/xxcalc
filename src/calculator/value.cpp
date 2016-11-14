#include "value.hpp"
#include "errors.hpp"
#include <iostream>
#include <algorithm>
#include <sstream>

namespace XX {
namespace Calculator {


std::string Value::repr(std::string const& name) const {
  size_t d = degree();

  if (d == 0)
    return "0";

  std::ostringstream output;
  bool need_sign = false;

  for (int i = d-1; i >= 0; i--) {
    if (coefficients[i] != 0) {
      if (coefficients[i] > 0 && need_sign) {
        output << "+";
        need_sign = false;
      }

      if (i > 0) {
        output << coefficients[i] << name;

        if (i > 1) {
          output << "^" << i;
        }
        need_sign = true;
      } else {
        output << coefficients[i];
      }

    }
  }

  return output.str();
}

double& Value::operator[](const size_t index) {
  if (index >= coefficients.size()) {
    coefficients.resize(index+1, 0.0);
  }

  return coefficients[index];
}

size_t Value::degree() const {
  auto it = std::find_if(coefficients.crbegin(), coefficients.crend(), [](double const& a) {
    return a != 0;
  });

  return coefficients.crend() - it;
}

Value::operator double() const {
  if (degree() > 1)
      throw PolynomialCastError();

  return coefficients[0];
}

Value::operator std::string() const {
  return repr("x");
}

Value& Value::operator+=(Value const& other) {
  if (other.coefficients.size() > coefficients.size()) {
    coefficients.resize(other.coefficients.size(), 0.0);
  }

  for (size_t i = 0; i < other.coefficients.size(); i++) {
    coefficients[i] += other.coefficients[i];
  }

  return *this;
}

Value& Value::operator-=(Value const& other) {
  if (other.coefficients.size() > coefficients.size()) {
    coefficients.resize(other.coefficients.size(), 0.0);
  }

  for (size_t i = 0; i < other.coefficients.size(); i++) {
    coefficients[i] -= other.coefficients[i];
  }

  return *this;
}

Value& Value::operator*=(Value const& other) {
  std::vector<double> c(coefficients.size() + other.coefficients.size()-1, 0.0);

  for (size_t a = 0; a < coefficients.size(); a++) {
    for (size_t b = 0; b < other.coefficients.size(); b++) {
      c[a+b] += coefficients[a] * other.coefficients[b];
    }
  }

  coefficients = c;

  return *this;
}

bool Value::operator==(Value const &other) const {
  size_t d = degree();

  if (other.degree() == d) {
    for (size_t i = 0; i < d; i++)
      if (other.coefficients[i] != coefficients[i])
        return false;

    return true;
  }

  return false;;
}

bool Value::operator!=(Value const &other) const {
  return !(*this == other);
}

  // Value& operator/=(Value const& other);

const Value Value::operator+(Value const& other) {
  return Value(*this) += other;
}

const Value Value::operator-(Value const& other) {
  return Value(*this) -= other;
}

const Value Value::operator*(Value const& other) {
  return Value(*this) *= other;
}

  // const Value operator/(Value const& other);



}
}
