#include "calculator/tokenizer.hpp"
#include "calculator/parser.hpp"
#include "catch.hpp"

using namespace XX::Calculator;

#define parse(x) (parser.process(tokenizer.process((x))))

TEST_CASE("operator registration", "[parser]") {
  Tokenizer tokenizer;
  Parser parser;

  SECTION("throws error on unregistered operator") {
    REQUIRE_THROWS_AS(parse("2=2"), UnknownOperatorError);
  }

  SECTION("has basic operators registered by default") {
    REQUIRE(parse("2+2")->repr() == "+");
    REQUIRE(parse("2-2")->repr() == "-");
    REQUIRE(parse("2*2")->repr() == "*");
    REQUIRE(parse("2/2")->repr() == "/");
    REQUIRE(parse("2^2")->repr() == "^");
  }

  SECTION("registers new operators") {
    REQUIRE_THROWS_AS(parse("2=2"), UnknownOperatorError);

    parser.register_operator("=", -1, -1);

    REQUIRE(parse("2=2")->repr() == "=");
  }
}

TEST_CASE("functions", "[parser]") {
  Tokenizer tokenizer;
  Parser parser;

  SECTION("throws error on unregistered function") {
    REQUIRE_THROWS_AS(parse("foo()"), UnknownFunctionError);
  }

  SECTION("registers new functions") {
    parser.register_function("foo", 0);

    REQUIRE(parse("foo()")->repr() == "foo");
  }

  SECTION("obeys arity") {
    parser.register_function("foo", 1);

    REQUIRE_NOTHROW(parse("foo(2)"));
    REQUIRE_THROWS_AS(parse("foo()"), ArgumentMissingError);
    REQUIRE_THROWS_AS(parse("foo(2, 1)"), ParsingError);

    parser.register_function("bar", 3);

    REQUIRE_NOTHROW(parse("bar(1, foo(2), 3)"));
  }
}

#define f(x) (dynamic_cast<FunctionNode&>(*(x)))

TEST_CASE("precendence and associativity", "[parser]") {
  Tokenizer tokenizer;
  Parser parser;

  SECTION("obeys precedence") {
    REQUIRE(parse("2+2*2")->repr() == "+");
    REQUIRE(parse("2*2+2")->repr() == "+");

    REQUIRE(parse("2*2/2")->repr() == "/");
    REQUIRE(parse("2/2*2")->repr() == "*");

    REQUIRE(parse("2*2^2")->repr() == "*");
    REQUIRE(parse("2^2*2")->repr() == "*");
  }

  SECTION("obeys associativity") {
    REQUIRE(f(f(parse("2+2+2")).arguments[0]).arguments.size() == 2);
    REQUIRE(f(f(parse("2-2-2")).arguments[0]).arguments.size() == 2);
    REQUIRE(f(f(parse("2*2*2")).arguments[0]).arguments.size() == 2);
    REQUIRE(f(f(parse("2/2/2")).arguments[0]).arguments.size() == 2);
    REQUIRE(f(f(parse("2^2^2")).arguments[1]).arguments.size() == 2);
  }
}

TEST_CASE("symbols", "[parser]") {
  Tokenizer tokenizer;
  Parser parser;

  SECTION("accept any symbol") {
    REQUIRE(parse("x")->repr() == "x");
    REQUIRE(parse("foobar")->repr() == "foobar");
  }

  SECTION("implicit multiplication") {
    REQUIRE(parse("2x")->repr() == "*");
  }
}

TEST_CASE("brackets", "[parser]") {
  Tokenizer tokenizer;
  Parser parser;

  SECTION("enforce precedence") {
    REQUIRE(parse("(2-2)*2")->repr() == "*");
  }

  SECTION("implicit multiplication") {
    REQUIRE(parse("2(14+2)")->repr() == "*");
  }

  SECTION("skip extra") {
    REQUIRE(parse("(((2)))")->repr()[0] == '2');
    REQUIRE(parse("(((2))+2)")->repr() == "+");
  }

  SECTION("recognized unbalanced") {
    REQUIRE_THROWS_AS(parse("2-(2(2+2)"), MissingBracketError);
    REQUIRE_THROWS_AS(parse("(2(2+2)"), MissingBracketError);
    REQUIRE_THROWS_AS(parse("2(2+2))"), MissingBracketError);
  }
}

TEST_CASE("parsing", "[parser]") {
  Tokenizer tokenizer;
  Parser parser;

  SECTION("requires single expression") {
    REQUIRE_THROWS_AS(parse("2+2, 1-1"), ParsingError);
    REQUIRE_THROWS_AS(parse(""), EmptyExpressionError);
    REQUIRE_THROWS_AS(parse("((()))"), EmptyExpressionError);
  }

  SECTION("detects unknown tokens") {
    REQUIRE_THROWS_AS(parse("@2"), ParsingError);
    REQUIRE_THROWS_AS(parse("2+2*{2-2}"), ParsingError);
  }
}
