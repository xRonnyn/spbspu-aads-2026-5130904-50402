#ifndef GRAPHMANAGER_HPP
#define GRAPHMANAGER_HPP
#include "../S3/hashTable.hpp"
#include "graph.hpp"

namespace karpenkov {
class GraphManager {
public:
  void createOrder(const std::string &name);
  void deleteOrder(const std::string &name);

  Graph &getOrder(const std::string &name);
  void listOrders(std::ostream &out);
  bool hasOrder(const std::string &name);

private:
  HashTable<std::string, Graph, StringHash, StringEqual> orders_{StringHash{},
                                                                 StringEqual{}};
};
} // namespace karpenkov

#endif
