#include <memory>
#include <vector>
#include <string>

#pragma once

namespace XX {
namespace Calculator {

struct Node {
  size_t position;

  virtual double evaluate() { return 0.0; };
  virtual std::string id() { return "None"; }
  virtual std::string tree(size_t depth) { return std::string(depth*2, ' ') + id(); }
  // virtual std::unique_ptr<Node> evaluate();
  //

  typedef std::unique_ptr<Node> unique;

  Node(size_t position) : position(position) { }

  virtual std::string repr() const {
    return "";
  }
};

struct FunctionNode : public Node {
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
  FunctionNode(std::string const& name, std::vector<std::unique_ptr<Node>> && arguments, size_t position) : Node(position), name(name), arguments(std::move(arguments)) { }

  virtual std::string repr() const {
    return name;
  }
};

struct SymbolNode : public Node {
  std::string name;

  std::string id() { return "Symbol("+name+")"; }
  SymbolNode(std::string const& name, size_t position) : Node(position), name(name) {}

  virtual std::string repr() const {
    return name;
  }
};

struct ValueNode : public Node {
  double value;

  std::string id() { return "Value("+std::to_string(value)+")"; }
  ValueNode(std::string const& value, size_t position) : Node(position), value(std::stod(value)) {}

  virtual double evaluate() { return value; }

  virtual std::string repr() const {
    return std::to_string(value);
  }
};

}
}
