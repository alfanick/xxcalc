#include "value.hpp"
#include "errors.hpp"
#include <iostream>
#include <algorithm>
#include <sstream>

namespace XX {
namespace Calculator {


double& Value::operator[](const size_t index) {
  if (index >= coefficients.size()) {
    coefficients.resize(index+1, 0.0);
  }

  return coefficients[index];
}

double Value::operator[](const size_t index) const {
  return coefficients[index];
}

size_t Value::degree() const {
  auto it = std::find_if(coefficients.crbegin(), coefficients.crend(), [](double const& a) {
    return a != 0;
  });

  if (it == coefficients.crend())
    return 0;
  else
    return coefficients.crend() - it - 1;
}

Value::operator double() const {
  if (degree() > 0)
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
  size_t self_degree = degree();
  size_t other_degree = other.degree();

  if (self_degree == 0 && other_degree == 0) {
    coefficients[0] *= other.coefficients[0];
    return *this;
  }

  std::vector<double> c(self_degree + other_degree+1, 0.0);

  for (size_t a = 0; a <= self_degree; a++) {
    for (size_t b = 0; b <= other_degree; b++) {
      c[a+b] += coefficients[a] * other.coefficients[b];
    }
  }

  coefficients = c;

  return *this;
}

Value& Value::operator/=(Value const& other) {
  size_t self_degree = degree();
  size_t other_degree = other.degree();

  if (self_degree < other_degree) {
    throw PolynomialDivisionError();
  } else
  if (self_degree == 0 && other_degree == 0) {
    coefficients[0] /= other.coefficients[0];
    return *this;
  }

  Value q;
  while (degree() >= other.degree()) {
    size_t diff = degree() - other.degree();
    Value d;
    d.coefficients.resize(other.degree() + diff + 1, 0);
    for (size_t i = 0; i <= other.degree(); i++) {
      d.coefficients[i+diff] = other.coefficients[i];
    }
    q[diff] = coefficients[degree()] / d.coefficients[d.degree()];
    d *= q[diff];
    *this -= d;
  }

  coefficients = q.coefficients;

  return *this;
}

bool Value::operator==(Value const &other) const {
  size_t d = degree();

  if (other.degree() == d) {
    for (size_t i = 0; i <= d; i++)
      if (other.coefficients[i] != coefficients[i])
        return false;

    return true;
  }

  return false;;
}

bool Value::operator!=(Value const &other) const {
  return !(*this == other);
}

const Value Value::operator+(Value const& other) const {
  return Value(*this) += other;
}

const Value Value::operator-(Value const& other) const {
  return Value(*this) -= other;
}

const Value Value::operator*(Value const& other) const {
  return Value(*this) *= other;
}

const Value Value::operator/(Value const& other) const {
  return Value(*this) /= other;
}

std::string Value::repr(std::string const& name) const {
  size_t d = degree();

  std::ostringstream output;

  if (d == 0) {
    output << coefficients[0];
    return output.str();
  }

  bool need_sign = false;

  for (int i = d; i >= 0; i--) {
    if (coefficients[i] != 0) {
      if (coefficients[i] > 0 && need_sign) {
        output << "+";
        need_sign = false;
      }

      if (i > 0) {
        if (coefficients[i] == -1) {
          output << "-";
        } else
        if (coefficients[i] != 1) {
          output << coefficients[i];
        }

        output << name;

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



}
}
