#include "calculator/polynomial_calculator.hpp"
#include "catch.hpp"

using namespace XX::Calculator;

#include <limits>
#include <cmath>

#define calc(x) (calculator.process((x)))

TEST_CASE("polynomial calculator", "[calculator]") {
  Tokenizer tokenizer;
  Parser parser;
  PolynomialCalculator calculator(tokenizer, parser);

  REQUIRE(calc("(3+(4-1))*5") == 30);

  SECTION("built-in constants") {
    REQUIRE(calc("x") == Value(0, 1));
    REQUIRE(calc("pi") == M_PI);
    REQUIRE(calc("e") == M_E);
  }

  SECTION("built-in operations") {
    REQUIRE(calc("1+1") == 2);
    REQUIRE(calc("1+-1") == 0);

    REQUIRE(calc("1-1") == 0);
    REQUIRE(calc("1--1") == 2);

    REQUIRE(calc("2*4") == 8);
    REQUIRE(calc("2*0") == 0);

    REQUIRE(calc("2/4") == 0.5);
    REQUIRE(calc("2/0") == std::numeric_limits<double>::infinity());
    REQUIRE(calc("0/0") != calc("0/0"));

    REQUIRE(calc("2^0") == 1);
    REQUIRE(calc("2^4") == 16);

    SECTION("polynomials") {
      REQUIRE(calc("2x+1") == Value(1, 2));
      REQUIRE(calc("2x+4x") == Value(0, 6));

      REQUIRE(calc("2x-2x-1") == -1);
      REQUIRE(calc("2x--2x") == Value(0, 4));

      REQUIRE(calc("2x*4x") == Value({0, 0, 8}));
      REQUIRE(calc("2x*(4x+1)") == Value({0, 2, 8}));

      REQUIRE(calc("2x/x") == 2);
      REQUIRE(calc("(x^2-3x-10)/(x+2)") == Value(-5, 1));

      REQUIRE(calc("x^5") == Value({0, 0, 0, 0, 0, 1}));
      REQUIRE(calc("(x+1)^5") == Value({1, 5, 10, 10, 5, 1}));
    }
  }

  SECTION("built-in functions") {
    REQUIRE(calc("log(16, 2)") == 4);
    REQUIRE(calc("log(5, 5)") == 1);

    REQUIRE(calc("log10(10)") == 1);
    REQUIRE(calc("log10(100)") == 2);

    REQUIRE((calc("17"), calc("ans")) == 17);
  }

  SECTION("precedence") {
    REQUIRE(calc("2+2*2") == 6);
    REQUIRE(calc("2*2+2") == 6);
    REQUIRE(calc("(2+2)*2") == 8);
    REQUIRE(calc("2*(2+2)") == 8);

    REQUIRE(calc("2+2-4") == calc("-4+2+2"));
  }
}
