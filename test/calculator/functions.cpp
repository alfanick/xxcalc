#include "calculator/functions.hpp"
#include "calculator/errors.hpp"
#include "catch.hpp"

#include <limits>
#include <cmath>

using namespace XX::Calculator;

TEST_CASE("arithmetic functions", "[functions]") {
  REQUIRE(Functions::addition({1, 2}) == 1+2);
  REQUIRE(Functions::subtraction({1, 2}) == 1-2);
  REQUIRE(Functions::multiplication({2, 3}) == 2*3);
  REQUIRE(double(Functions::division({2, 3})) == Approx(2.0/3.0));
}

TEST_CASE("exponentiation function", "[functions]") {
  SECTION("arithmetic") {
    REQUIRE(Functions::exponentiation({2, 4}) == 16);
    REQUIRE(double(Functions::exponentiation({2, -2})) == Approx(0.25));
  }

  SECTION("polynomials") {
    REQUIRE(Functions::exponentiation({Value(0, 1), 0}) == 1);
    REQUIRE(Functions::exponentiation({Value(0, 1), 1}) == Value(0, 1));
    REQUIRE(Functions::exponentiation({Value(0, 4), 2}) == Value({0, 0, 16}));
    REQUIRE(std::string(Functions::exponentiation({Value({4, 4, 3}), 3})) == "27x^6+108x^5+252x^4+352x^3+336x^2+192x+64");
  }

  SECTION("non-polynomial result") {
    REQUIRE_THROWS_AS(Functions::exponentiation({2, Value(0, 1)}), ExponentationError);
    REQUIRE_THROWS_AS(Functions::exponentiation({Value(0, 1), 1.23}), ExponentationError);
  }
}
