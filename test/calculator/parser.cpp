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

  SECTION("registers new operators") {
    parser.register_operator("=", -1, -1);

    REQUIRE(std::next(std::next(parse("2=2").begin()))->value == "=");
  }
}

TEST_CASE("precendence and associativity", "[parser]") {
  Tokenizer tokenizer;
  Parser parser;

  parser.register_operator("+", 1, -1);
  parser.register_operator("-", 1, -1);
  parser.register_operator("*", 5, -1);
  parser.register_operator("/", 5, -1);
  parser.register_operator("^", 10, 1);

  SECTION("obeys precedence") {
    REQUIRE(parse("2+2*2").rbegin()->value == "+");
    REQUIRE(parse("2*2+2").rbegin()->value == "+");

    REQUIRE(parse("2*2/2").rbegin()->value == "/");
    REQUIRE(parse("2/2*2").rbegin()->value == "*");

    REQUIRE(parse("2*2^2").rbegin()->value == "*");
    REQUIRE(parse("2^2*2").rbegin()->value == "*");
  }

  SECTION("obeys associativity") {
    REQUIRE(std::next(parse("2+2+2").rbegin())->value == "2");
    REQUIRE(std::next(parse("2^2^2").rbegin())->value == "^");
  }
}

TEST_CASE("symbols", "[parser]") {
  Tokenizer tokenizer;
  Parser parser;

  SECTION("accept any symbol") {
    REQUIRE(parse("x").begin()->value == "x");
    REQUIRE(parse("foobar").begin()->value == "foobar");
  }

  SECTION("implicit multiplication") {
    parser.register_operator("*", 5, -1);
    REQUIRE(parse("2x").size() == 3);
    REQUIRE(parse("2x").rbegin()->value == "*");
  }
}

TEST_CASE("brackets", "[parser]") {
  Tokenizer tokenizer;
  Parser parser;

  parser.register_operator("+", 1, -1);
  parser.register_operator("*", 5, -1);

  SECTION("enforce precedence") {
    REQUIRE(parse("2+2*2").rbegin()->value == "+");
    REQUIRE(parse("(2+2)*2").rbegin()->value == "*");
  }

  SECTION("implicit multiplication") {
    REQUIRE(parse("2(14+2)").rbegin()->value == "*");
  }

  SECTION("skip extra") {
    REQUIRE(parse("(((2)))").begin()->value == "2");
    REQUIRE(parse("(((2))+2)").rbegin()->value == "+");
  }

  SECTION("recognized unbalanced") {
    REQUIRE_THROWS_AS(parse("2+(2(2+2)"), MissingBracketError);
    REQUIRE_THROWS_AS(parse("(2(2+2)"), MissingBracketError);
    REQUIRE_THROWS_AS(parse("2(2+2))"), MissingBracketError);
  }
}

TEST_CASE("parsing", "[parser]") {
  Tokenizer tokenizer;
  Parser parser;

  parser.register_operator("+", 1, -1);
  parser.register_operator("*", 5, -1);

  SECTION("requires non empty expression") {
    REQUIRE_THROWS_AS(parse(""), EmptyExpressionError);
    REQUIRE_THROWS_AS(parse("((()))"), EmptyExpressionError);
  }

  SECTION("detects unknown tokens") {
    REQUIRE_THROWS_AS(parse("@2"), ParsingError);
    REQUIRE_THROWS_AS(parse("2+2*{2+2}"), ParsingError);
  }
}
