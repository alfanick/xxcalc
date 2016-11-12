#include "calculator/tokenizer.hpp"
#include "catch.hpp"

using namespace XX::Calculator;

#define token(x) (tokenizer.process((x)).begin())

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

    SECTION("signed") {
      REQUIRE(token("-1")->type == TokenType::NUMBER);
      REQUIRE(token("+1")->type == TokenType::NUMBER);
      REQUIRE(token("+1e-23")->type == TokenType::NUMBER);
      REQUIRE(token("+1e-23")->value == "+1e-23");
    }

    SECTION("special") {
      REQUIRE(token("inf")->type == TokenType::NUMBER);
      REQUIRE(token("-inf")->type == TokenType::NUMBER);
      REQUIRE(token("Infinity")->type == TokenType::NUMBER);
      REQUIRE(token("-Infinity")->type == TokenType::NUMBER);
      REQUIRE(token("nan")->type == TokenType::NUMBER);
      REQUIRE(token("NaN")->type == TokenType::NUMBER);

      REQUIRE(token("-Infinity")->value == "-Infinity");
    }
  }
}

TEST_CASE("expression tokenization", "[tokenizer]") {
  Tokenizer tokenizer;

  {
    auto t = tokenizer.process("2+2");
    REQUIRE(t.front().type == TokenType::NUMBER); t.pop_front();
    REQUIRE(t.front().type == TokenType::OPERATOR); t.pop_front();
    REQUIRE(t.front().type == TokenType::NUMBER); t.pop_front();
  }

  {
    auto t = tokenizer.process("2+x");
    REQUIRE(t.front().type == TokenType::NUMBER); t.pop_front();
    REQUIRE(t.front().type == TokenType::OPERATOR); t.pop_front();
    REQUIRE(t.front().type == TokenType::IDENTIFIER); t.pop_front();
  }

  {
    auto t = tokenizer.process("x=2");
    REQUIRE(t.front().type == TokenType::IDENTIFIER); t.pop_front();
    REQUIRE(t.front().type == TokenType::OPERATOR); t.pop_front();
    REQUIRE(t.front().type == TokenType::NUMBER); t.pop_front();
  }

  SECTION("signed numbers") {
    {
      auto t = tokenizer.process("-2--2");
      REQUIRE(t.size() == 3);
      REQUIRE(t.front().value == "-2"); t.pop_front();
      REQUIRE(t.front().value == "-"); t.pop_front();
      REQUIRE(t.front().value == "-2"); t.pop_front();
    }

    {
      auto t = tokenizer.process("(1+2)+-2");
      REQUIRE(t.size() == 7);
      REQUIRE(t.back().value == "-2");
    }

    {
      auto t = tokenizer.process("(1+2)++2");
      REQUIRE(t.size() == 7);
      REQUIRE(t.back().value == "+2");
    }

    {
      auto t = tokenizer.process("x--2");
      REQUIRE(t.size() == 3);
      REQUIRE(t.back().value == "-2");
    }
  }
}
