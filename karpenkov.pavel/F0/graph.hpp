#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "../S3/hashTable.hpp"
#include "../common/list.hpp"
#include <iostream>
namespace karpenkov {
struct StringHash {
  size_t operator()(const std::string &s) const {
    return std::hash<std::string>{}(s);
  }
};

struct StringEqual {
  bool operator()(const std::string &a, const std::string &b) const {
    return a == b;
  }
};
class Graph {
public:
  Graph();
  void insertMovie(const std::string &A, const std::string &B) {
    if (!adj_.has(B)) {
      adj_.add(B, karpenkov::List<std::string>{});
    }
    if (!adj_.has(A)) {
      adj_.add(A, karpenkov::List<std::string>{});
    }
    if (adj_.at(A).contains(B)) {
      std::cerr << "edge already exist";
      return;
    }
    adj_.at(A).push_back(B);
  }

private:
  HashTable<std::string, karpenkov::List<std::string>, StringHash, StringEqual>
      adj_;
};
} // namespace karpenkov
#endif
