#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "../S2/queue.hpp"
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
  Graph()
      : adj_(StringHash{}, StringEqual{}),
        reverse_(StringHash{}, StringEqual{}) {}
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
    if (!reverse_.has(A)) {
      reverse_.add(A, karpenkov::List<std::string>{});
    }
    if (!reverse_.has(B)) {
      reverse_.add(B, karpenkov::List<std::string>{});
    }
    reverse_.at(B).push_back(A);
  }
  void showOrder() {
    HashTable<std::string, size_t, StringHash, StringEqual> indegree(
        StringHash{}, StringEqual{});
    using itHash = Iterator<std::string, karpenkov::List<std::string>,
                            StringHash, StringEqual>;
    for (itHash it = adj_.begin(); it != adj_.end(); ++it) {
      indegree.add((*it).key, 0);
    }
    for (itHash a = adj_.begin(); a != adj_.end(); ++a) {
      karpenkov::List<std::string> l = (*a).value;
      for (LCIter<std::string> b = l.cbegin(); b != l.cend(); ++b) {
        indegree.at(*b)++;
      }
    }
    karpenkov::Queue<std::string> q;
    for (itHash it = adj_.begin(); it != adj_.end(); ++it) {
      if (indegree.at((*it).key) == 0) {
        q.push((*it).key);
      }
    }
    karpenkov::List<std::string> result;
    while (!q.empty()) {
      std::string v = q.front();
      q.pop();
      result.push_back(v);
      karpenkov::List<std::string> &neighbors = adj_.at(v);
      for (LCIter<std::string> it = neighbors.cbegin(); it != neighbors.cend();
           ++it) {
        std::string to = *it;
        indegree.at(to)--;
        if (indegree.at(to) == 0) {
          q.push(to);
        }
      }
    }
    size_t count = 0;
    for (LCIter<std::string> it = result.cbegin(); it != result.cend(); ++it) {
      ++count;
    }
    if (count != adj_.size()) {
      std::cout << "Error: cycle detected\n";
      return;
    }
    for (LCIter<std::string> it = result.cbegin(); it != result.cend(); ++it) {
      std::cout << *it;
      LCIter<std::string> next = it;
      ++next;
      if (next != result.cend()) {
        std::cout << " -> ";
      }
    }
    std::cout << "\n";
  }
  void showDependencies(const std::string &movie) {
    if (!adj_.has(movie)) {
      std::cerr << "error: movie not found\n";
      return;
    }
    HashTable<std::string, bool, StringHash, StringEqual> visit{StringHash{},
                                                                StringEqual{}};
    karpenkov::List<std::string> result;
    dfsDependencies(movie, visit, result);
    for (karpenkov::LCIter<std::string> i = result.cbegin(); i != result.cend();
         ++i) {
      std::cout << (*i) << ' ';
    }
  }

private:
  HashTable<std::string, karpenkov::List<std::string>, StringHash, StringEqual>
      adj_;
  HashTable<std::string, karpenkov::List<std::string>, StringHash, StringEqual>
      reverse_;
  void dfsDependencies(
      const std::string &movie,
      HashTable<std::string, bool, StringHash, StringEqual> &visited,
      karpenkov::List<std::string> &result) {
    if (!visited.has(movie)) {
      visited.add(movie, false);
    }
    if (visited.at(movie)) {
      return;
    }
    visited.at(movie) = true;
    karpenkov::List<std::string> &neighbors = adj_.at(movie);
    for (karpenkov::LCIter<std::string> it = neighbors.cbegin();
         it != neighbors.cend(); ++it) {
      std::string next = *it;
      if (!visited.has(next)) {
        visited.add(next, false);
      }
      if (!visited.at(next)) {
        result.push_back(next);
        dfsDependencies(next, visited, result);
      }
    }
  }
};
} // namespace karpenkov
#endif
