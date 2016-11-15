#include "value.hpp"
#include "errors.hpp"
#include <iostream>
#include <algorithm>
#include <sstream>

namespace XX {
namespace Calculator {


double& Value::operator[](const unsigned long index) {
  if (index >= coefficients.size()) {
    coefficients.resize(index+1, 0.0);
  }

  return coefficients[index];
}

double Value::operator[](const unsigned long index) const {
  return coefficients[index];
}

unsigned long Value::degree() const {
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

  for (unsigned long i = 0; i < other.coefficients.size(); i++) {
    coefficients[i] += other.coefficients[i];
  }

  return *this;
}

Value& Value::operator-=(Value const& other) {
  if (other.coefficients.size() > coefficients.size()) {
    coefficients.resize(other.coefficients.size(), 0.0);
  }

  for (unsigned long i = 0; i < other.coefficients.size(); i++) {
    coefficients[i] -= other.coefficients[i];
  }

  return *this;
}

Value& Value::operator*=(Value const& other) {
  unsigned long self_degree = degree();
  unsigned long other_degree = other.degree();

  if (self_degree == 0 && other_degree == 0) {
    coefficients[0] *= other.coefficients[0];
    return *this;
  }

  std::vector<double> c(self_degree + other_degree+2, 0.0);

  for (unsigned long a = 0; a <= self_degree; a++) {
    for (unsigned long b = 0; b <= other_degree; b++) {
      c[a+b] += coefficients[a] * other.coefficients[b];
    }
  }

  coefficients = c;

  return *this;
}

Value& Value::operator/=(Value const& other) {
  unsigned long self_degree = degree();
  unsigned long other_degree = other.degree();

  if (self_degree < other_degree) {
    throw PolynomialDivisionError();
  } else
  if (other_degree == 0) {
    if (other.coefficients[0] == 0 && self_degree > 0)
      throw PolynomialDivisionError();

    for (unsigned long i = 0; i <= self_degree; i++) {
      coefficients[i] /= other.coefficients[0];
    }
    return *this;
  }

  Value q;
  while (degree() >= other.degree()) {
    unsigned long diff = degree() - other.degree();
    Value d;
    d.coefficients.resize(other.degree() + diff + 1, 0);
    for (unsigned long i = 0; i <= other.degree(); i++) {
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
  unsigned long d = degree();

  if (other.degree() == d) {
    for (unsigned long i = 0; i <= d; i++)
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
  unsigned long d = degree();

  std::ostringstream output;

  if (d == 0) {
    output << (coefficients.empty() ? 0 : coefficients[0]);
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
