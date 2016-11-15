#include "calculator/parser.hpp"
#include "calculator/evaluator.hpp"
#include "calculator/errors.hpp"
#include "calculator/functions.hpp"
#include "catch.hpp"

#include <limits>
#include <cmath>

using namespace XX::Calculator;

TEST_CASE("constants and functions registration", "[evaluator]") {
  Evaluator evaluator;

  evaluator.register_constant("foo", 123);
  REQUIRE_THROWS_AS(evaluator.register_function("foo", 1, Functions::log10), ConflictingNameError);

  evaluator.register_function("bar", 1, Functions::log10);
  REQUIRE_THROWS_AS(evaluator.register_constant("bar", 123), ConflictingNameError);

}

#define eval(x) (evaluator.process(parser.process(tokenizer.process((x)))))

TEST_CASE("evaluation", "[evaluator]") {
  Tokenizer tokenizer;
  Parser parser;
  Evaluator evaluator;

  parser.register_operator("+", 1, -1);
  evaluator.register_function("+", 2, Functions::addition);
  parser.register_operator("-", 1, -1);
  evaluator.register_function("-", 2, Functions::subtraction);
  parser.register_operator("*", 5, -1);
  evaluator.register_function("*", 2, Functions::multiplication);

  SECTION("operators") {
    REQUIRE(eval("2+4") == 6);
    REQUIRE(eval("2*4") == 8);
    REQUIRE(eval("2+4*6") == 26);
    REQUIRE(eval("2*4+6") == 14);
    REQUIRE(eval("(2+4)*6") == 36);

    REQUIRE(eval("2+-4") == -2);
    REQUIRE(eval("2--4") == 6);
    REQUIRE(eval("2++4") == 6);
  }

  SECTION("constants") {
    REQUIRE_THROWS_AS(eval("foo"), UnknownSymbolError);

    evaluator.register_constant("foo", 123);
    REQUIRE(eval("foo") == 123);
  }

  SECTION("functions") {
    REQUIRE_THROWS_AS(eval("double(2)"), UnknownSymbolError);

    evaluator.register_function("double", 1, [](std::vector<Value> const& args) {
      return args[0] * 2;
    });

    REQUIRE(eval("double(2)") == 4);
    REQUIRE(eval("double(4*-0.5)") == -4);
    REQUIRE_THROWS_AS(eval("double()"), ArgumentMissingError);
    REQUIRE_THROWS_AS(eval("double(1, 2)"), EvaluationError);

    SECTION("no arguments") {
      int counter = 0;
      evaluator.register_function("counter", 0, [&](std::vector<Value> const& args) {
        return counter++;
      });

      REQUIRE(eval("counter()") == 0);
      REQUIRE(eval("counter*2") == 2);
      REQUIRE(eval("counter*2") == 4);
      REQUIRE_THROWS_AS(eval("counter(123)"), EvaluationError);
    }

    SECTION("multiple arguments") {
      evaluator.register_function("mod", 2, [](std::vector<Value> const& args) {
        return std::fmod(double(args[0]), double(args[1]));
      });

      REQUIRE(eval("mod(17, 4)") == 1);
      REQUIRE_THROWS_AS(eval("mod(1)"), ArgumentMissingError);
      REQUIRE_THROWS_AS(eval("mod(1, 2, 3)"), EvaluationError);
    }
  }

  SECTION("multiple expressions") {
    REQUIRE_THROWS_AS(eval("2 2"), EvaluationError);
    REQUIRE_THROWS_AS(eval("2, 2"), EvaluationError);
  }

  SECTION("polynomials") {
    evaluator.register_constant("x", Value(0, 1));

    REQUIRE(eval("x") == Value(0, 1));
    REQUIRE(eval("x*x") == Value({0, 0, 1}));
    REQUIRE(eval("x+x") == eval("2x"));
  }
}
