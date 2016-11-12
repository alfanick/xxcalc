#include "calculator/tokenizer.hpp"
#include "catch.hpp"

#define token(x) (tokenizer.process((x)).begin())

using namespace XX::Calculator;

TEST_CASE("single tokens", "[tokenizer]") {
  Tokenizer tokenizer;

  SECTION("brackets") {
    REQUIRE(token("(")->type == TokenType::BRACKET_OPENING);
    REQUIRE(token(")")->type == TokenType::BRACKET_CLOSING);
  }

  SECTION("operators") {
    REQUIRE(token("+")->type == TokenType::OPERATOR);
    REQUIRE(token("-")->type == TokenType::OPERATOR);
    REQUIRE(token("*")->type == TokenType::OPERATOR);
    REQUIRE(token("/")->type == TokenType::OPERATOR);
    REQUIRE(token("=")->type == TokenType::OPERATOR);
    REQUIRE(token("^")->type == TokenType::OPERATOR);

    REQUIRE(token("=")->value == "=");
  }

  SECTION("identifiers") {
    REQUIRE(token("x")->type == TokenType::IDENTIFIER);
    REQUIRE(token("x123x")->type == TokenType::IDENTIFIER);
    REQUIRE(token("_foo_bar")->type == TokenType::IDENTIFIER);

    REQUIRE(token("_foo_123")->value == "_foo_123");
  }

  SECTION("numbers") {
    REQUIRE(token("1")->type == TokenType::NUMBER);
    REQUIRE(token("1")->value == "1");
    REQUIRE(token("1.23")->type == TokenType::NUMBER);
    REQUIRE(token("1.23")->value == "1.23");
    REQUIRE(token(".23")->type == TokenType::NUMBER);
    REQUIRE(token(".23")->value == ".23");

    SECTION("scientific notation") {
      REQUIRE(token("1e23")->type == TokenType::NUMBER);
      REQUIRE(token("1e23")->value == "1e23");
      REQUIRE(token("1e-23")->type == TokenType::NUMBER);
      REQUIRE(token("1e-23")->value == "1e-23");
      REQUIRE(token("1.01e+23")->type == TokenType::NUMBER);
      REQUIRE(token("1.01e+23")->value == "1.01e+23");
    }

    SECTION("signed numbers") {
      REQUIRE(token("-1")->type == TokenType::NUMBER);
      REQUIRE(token("+1")->type == TokenType::NUMBER);
      REQUIRE(token("+1e-23")->type == TokenType::NUMBER);
      REQUIRE(token("+1e-23")->value == "+1e-23");
    }
  }
}
