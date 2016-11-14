#include <vector>

namespace XX {
namespace Calculator {

class Value {
  public:

  Value(std::vector<double> coefficients) : coefficients(coefficients) { }
  Value(double x0, double x1) : Value(std::vector<double>{x0, x1}){ }
  Value(double x0) : Value(x0, 0.0) { }
  Value() : Value(0.0) { }

  std::string repr();

  double& operator[](const size_t index);
  size_t degree() const;
  explicit operator double() const;

  Value& operator+=(Value const& other);
  Value& operator-=(Value const& other);
  Value& operator*=(Value const& other);
  Value& operator/=(Value const& other);

  const Value operator+(Value const& other);
  const Value operator-(Value const& other);
  const Value operator*(Value const& other);
  const Value operator/(Value const& other);

  bool operator==(Value const &other) const;
  bool operator!=(Value const &other) const;


  private:

  std::vector<double> coefficients;
};

}
}
