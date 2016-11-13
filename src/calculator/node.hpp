#include <memory>
#include <vector>
#include <string>

#pragma once

#include "evaluator.hpp"

namespace XX {
namespace Calculator {

struct Node {
  typedef std::unique_ptr<Node> unique;

  virtual std::string repr() const {
    return "";
  }
  virtual Node::unique evaluate(Evaluator &evaluator);

  operator double() const;
};

struct FunctionNode : public Node {
  std::string name;
  std::vector<Node::unique> arguments;

  FunctionNode(std::string const& name, std::vector<Node::unique> && arguments) : name(name), arguments(std::move(arguments)) { }

  virtual std::string repr() const {
    return name;
  }

  virtual Node::unique evaluate(Evaluator &evaluator);
};

struct SymbolNode : public Node {
  std::string name;

  SymbolNode(std::string const& name) : name(name) {}

  virtual std::string repr() const {
    return name;
  }

  virtual Node::unique evaluate(Evaluator &evaluator);
};

struct ValueNode : public Node {
  double value;

  ValueNode(double value) : value(value) {}
  ValueNode(std::string const& value) : value(std::stod(value)) {}

  virtual std::string repr() const {
    return std::to_string(value);
  }

  virtual Node::unique evaluate(Evaluator &evaluator);
};

}
}
