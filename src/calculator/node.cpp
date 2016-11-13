#include "node.hpp"

namespace XX {
namespace Calculator {

Node::operator double() const {
  return dynamic_cast<const ValueNode*>(this)->value;
}

Node::unique Node::evaluate(Evaluator &evaluator) {
  return nullptr;
}

Node::unique FunctionNode::evaluate(Evaluator &evaluator) {
  return evaluator.call(name, arguments);
}

Node::unique SymbolNode::evaluate(Evaluator &evaluator) {
  return evaluator.call(name);
}

Node::unique ValueNode::evaluate(Evaluator &evaluator) {
  return Node::unique(new ValueNode(value));
}

}
}
