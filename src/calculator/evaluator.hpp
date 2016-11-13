#include <functional>
#include <vector>
#include <map>

#pragma once

namespace XX {
namespace Calculator {

struct Node;

class Evaluator {
  public:

  void register_function(std::string const& name, std::function<std::unique_ptr<Node>(std::vector<std::unique_ptr<Node>> const&)> f);

  virtual std::unique_ptr<Node> call(std::string const& symbol);
  virtual std::unique_ptr<Node> call(std::string const& name, std::vector<std::unique_ptr<Node>> const& arguments);

  double process(std::unique_ptr<Node> root);

  private:

  std::map<std::string, std::function<std::unique_ptr<Node>(std::vector<std::unique_ptr<Node>> const&)>> functions;
};

}
}
