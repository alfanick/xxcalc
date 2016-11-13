#include <memory>
#include <vector>
#include <string>

#pragma once

namespace XX {
namespace Calculator {

struct Node {
  virtual double evaluate() { return 0.0; };
  virtual std::string id() { return "None"; }
  virtual std::string tree(size_t depth) { return std::string(depth*2, ' ') + id(); }
  // virtual std::unique_ptr<Node> evaluate();
  //

  typedef std::unique_ptr<Node> unique;
};

struct FunctionNode : Node {
  std::string name;

  std::vector<std::unique_ptr<Node>> arguments;

  std::string id() { return "Function("+name+")"; }
  virtual std::string tree(size_t depth) {
    std::string repr = Node::tree(depth);

    for (auto& a : arguments) {
      repr += "\n" + a->tree(depth+1);
    }

    return repr;
  }
  FunctionNode(std::string const& name, std::vector<std::unique_ptr<Node>> && arguments) : name(name), arguments(std::move(arguments)) { }
};

struct SymbolNode : Node {
  std::string name;

  std::string id() { return "Symbol("+name+")"; }
  SymbolNode(std::string const& name) : name(name) {}
};

struct ValueNode : Node {
  double value;

  std::string id() { return "Value("+std::to_string(value)+")"; }
  ValueNode(std::string const& value) : value(std::stod(value)) {}

  virtual double evaluate() { return value; }
};

}
}
