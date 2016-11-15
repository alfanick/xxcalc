#include "calculator/linear_solver.hpp"
#include "catch.hpp"

using namespace XX::Calculator;

#include <limits>
#include <cmath>

#define solve(x) (solver.process((x)))

TEST_CASE("solver", "[calculator]") {
  Tokenizer tokenizer;
  Parser parser;
  LinearSolver solver(tokenizer, parser);

  REQUIRE(solve("2 * x + 0.5 = 1") == 0.25);
  REQUIRE(solve("2x + 1 = 2(1-x)") == 0.25);

  REQUIRE(solve("x^2-x^2+x=2") == 2);
  REQUIRE(solve("1-x=x") == 0.5);

  SECTION("position agnostic") {
    REQUIRE(solve("x=2") == 2);
    REQUIRE(solve("2=x") == 2);
    REQUIRE(solve("x-2=0") == 2);
    REQUIRE(solve("0=-2+x") == 2);
  }

  SECTION("restrictions") {
    REQUIRE_THROWS_AS(solve("x^2=5"), NonLinearEquation);
    REQUIRE_THROWS_AS(solve("2=2"), NoSymbolFound);
  }

  SECTION("special cases") {
    REQUIRE_THROWS_AS(solve("x=x"), ExpressionIsTautology);
    REQUIRE_THROWS_AS(solve("x=x+1"), NonSolvableExpression);
  }

  SECTION("solving flag") {
    REQUIRE((solve("2"), solver.solved) == false);
    REQUIRE((solve("x=2"), solver.solved) == true);
    REQUIRE((solve("2"), solver.solved) == false);
  }
}
