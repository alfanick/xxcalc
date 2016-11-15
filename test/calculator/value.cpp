#include "calculator/value.hpp"
#include "calculator/errors.hpp"
#include "catch.hpp"

#include <limits>

using namespace XX::Calculator;

TEST_CASE("initialization", "[value]") {
  REQUIRE(Value()[0] == 0);
  REQUIRE(Value(1.0)[0] == 1.0);
  REQUIRE(Value(1.0, 2.0)[0] == 1.0);
  REQUIRE(Value(1.0, 2.0)[1] == 2.0);

  Value zero;
  zero[3] = 3.0;
  REQUIRE(zero[0] == 0.0);
  REQUIRE(zero[1] == 0.0);
  REQUIRE(zero[2] == 0.0);
  REQUIRE(zero[3] == 3.0);
  zero[5] = 5.0;
  REQUIRE(zero[3] == 3.0);
  REQUIRE(zero[4] == 0.0);
  REQUIRE(zero[5] == 5.0);
}

TEST_CASE("casting", "[value]") {
  REQUIRE(Value() == 0.0);
  REQUIRE(Value(1.0) == 1.0);
  REQUIRE(Value(1.0, 0.0) == 1.0);

  double k;
  REQUIRE_THROWS_AS((k = double(Value(1.0, 1.0))), PolynomialCastError);
}

TEST_CASE("degree", "[value]") {
  REQUIRE(Value().degree() == 0);
  REQUIRE(Value(1).degree() == 0);
  REQUIRE(Value({1, 1, 1, 1}).degree() == 3);
  REQUIRE(Value({1, 1, 1, 0, 0, 0}).degree() == 2);
  REQUIRE(Value({1, 0, 1}).degree() == 2);
  REQUIRE(Value({1, 0, -1}).degree() == 2);
}

TEST_CASE("compound assignment", "[value]") {
  Value a(1);

  SECTION("addition") {
    a += Value({1, 2, 3});
    REQUIRE(a[0] == 2);
    REQUIRE(a[1] == 2);
    REQUIRE(a[2] == 3);
  }

  SECTION("subtraction") {
    a -= Value({1, 2, 3});
    REQUIRE(a[0] == 0);
    REQUIRE(a[1] == -2);
    REQUIRE(a[2] == -3);
  }

  SECTION("multiplication") {
    Value a({5, 0, 10, 6});
    Value b({1, 2, 4});

    a *= b;

    REQUIRE(a[0] == 5);
    REQUIRE(a[1] == 10);
    REQUIRE(a[2] == 30);
    REQUIRE(a[3] == 26);
    REQUIRE(a[4] == 52);
    REQUIRE(a[5] == 24);
  }

  SECTION("division") {
    Value a({-10, -3, 1});
    Value b(2, 1);

    a /= b;

    REQUIRE(a.degree() == 1);
    REQUIRE(a[0] == -5);
    REQUIRE(a[1] == 1);

    b = Value({1, 2, 3, 4});
    REQUIRE(a.degree() < b.degree());
    REQUIRE_THROWS_AS(a /= b, PolynomialDivisionError);

    b = Value();
    REQUIRE_THROWS_AS(a /= b, PolynomialDivisionError);
  }
}

TEST_CASE("comparison", "[value]") {
  REQUIRE(Value({1,2,3}) == Value({1,2,3}));
  REQUIRE(Value({1,2,3,0}) == Value({1,2,3}));
  REQUIRE(Value({1,2,3}) == Value({1,2,3,0}));
  REQUIRE(Value({1,2,3}) != Value({1,2,3,4}));
  REQUIRE(Value({1,2,3,4}) != Value({1,2,3}));
}

TEST_CASE("operations", "[value]") {
  REQUIRE(Value({1,2,3}) + Value({3,2,1}) == Value({4,4,4}));
  REQUIRE(Value({1,2,3}) - Value({1,2,3}) == Value(0));
  REQUIRE(Value({1,2,3}) * Value(2) == Value({2,4,6}));
  REQUIRE(Value(2) * Value(0,1) == Value(0,2));
  REQUIRE(Value(2) / Value(0) == Value(std::numeric_limits<double>::infinity()));
  REQUIRE(Value(2) / Value(2) == Value(1));
  REQUIRE(Value(0,2) / Value(0,1) == Value(2));
  REQUIRE(Value(0,1) / Value(0,2) == Value(0.5));
}

TEST_CASE("string conversion", "[value]") {
  REQUIRE(std::string(Value()) == "0");
  REQUIRE(std::string(Value(1)) == "1");
  REQUIRE(std::string(Value(1, 2)) == "2x+1");
  REQUIRE(std::string(Value({1, 0, 2})) == "2x^2+1");
  REQUIRE(std::string(Value(-1, 2)) == "2x-1");
  REQUIRE(std::string(Value({1, 0, -2})) == "-2x^2+1");
  REQUIRE(std::string(Value({-1, 0, -2})) == "-2x^2-1");

}
