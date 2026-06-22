#include "GraphManager.hpp"

void karpenkov::GraphManager::createOrder(const std::string &name) {
  if (orders_.has(name)) {
    throw std::runtime_error("order already exists");
  }
  orders_.add(name, Graph{});
}

void karpenkov::GraphManager::deleteOrder(const std::string &name) {
  orders_.drop(name);
}

karpenkov::Graph &karpenkov::GraphManager::getOrder(const std::string &name) {
  return orders_.at(name);
}
bool karpenkov::GraphManager::hasOrder(const std::string &name) {
  return orders_.has(name);
}

void karpenkov::GraphManager::listOrders(std::ostream &out) {
  using Iter = Iterator<std::string, Graph, StringHash, StringEqual>;
  for (Iter it = orders_.begin(); it != orders_.end(); ++it) {
    out << (*it).key << '\n';
  }
}
