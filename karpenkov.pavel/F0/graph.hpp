#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "../S2/queue.hpp"
#include "../S3/hashTable.hpp"
#include "../common/list.hpp"
#include <iostream>
namespace karpenkov {
enum Color { WHITE, GRAY, BLACK };
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
using AdjTable = HashTable<std::string, karpenkov::List<std::string>,
                           StringHash, StringEqual>;
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
    dfs(movie, adj_, visit, result);
    for (karpenkov::LCIter<std::string> i = result.cbegin(); i != result.cend();
         ++i) {
      std::cout << (*i) << ' ';
    }
  }
  void showSuccessors(const std::string &movie) {
    if (!reverse_.has(movie)) {
      std::cerr << "error: movie not found\n";
      return;
    }
    HashTable<std::string, bool, StringHash, StringEqual> visit{StringHash{},
                                                                StringEqual{}};
    karpenkov::List<std::string> result;
    dfs(movie, reverse_, visit, result);
    for (karpenkov::LCIter<std::string> i = result.cbegin(); i != result.cend();
         ++i) {
      std::cout << (*i) << ' ';
    }
  }
  void findCycles() {
    HashTable<std::string, Color, StringHash, StringEqual> colors{
        StringHash{}, StringEqual{}};
    HashTable<std::string, std::string, StringHash, StringEqual> parent{
        StringHash{}, StringEqual{}};
    karpenkov::List<std::string> cycle;
    using It = Iterator<std::string, karpenkov::List<std::string>, StringHash,
                        StringEqual>;
    for (It it = adj_.begin(); it != adj_.end(); ++it) {
      colors.add((*it).key, WHITE);
    }
    for (It it = adj_.begin(); it != adj_.end(); ++it) {
      const std::string &movie = (*it).key;
      if (colors.at(movie) == WHITE) {
        if (dfsCycle(movie, colors, parent, cycle)) {
          for (karpenkov::LCIter<std::string> c = cycle.cbegin();
               c != cycle.cend(); ++c) {
            std::cout << *c;
            karpenkov::LCIter<std::string> next = c;
            ++next;
            if (next != cycle.cend()) {
              std::cout << " -> ";
            }
          }
          std::cout << '\n';
          return;
        }
      }
    }
    std::cout << "No cycles found\n";
  }

private:
  HashTable<std::string, karpenkov::List<std::string>, StringHash, StringEqual>
      adj_;
  HashTable<std::string, karpenkov::List<std::string>, StringHash, StringEqual>
      reverse_;
  void dfs(const std::string &movie, AdjTable &graph,
           HashTable<std::string, bool, StringHash, StringEqual> &visited,
           karpenkov::List<std::string> &result) {
    if (!visited.has(movie)) {
      visited.add(movie, false);
    }
    if (visited.at(movie)) {
      return;
    }
    visited.at(movie) = true;
    karpenkov::List<std::string> &neighbors = graph.at(movie);
    for (karpenkov::LCIter<std::string> it = neighbors.cbegin();
         it != neighbors.cend(); ++it) {
      std::string next = *it;
      if (!visited.has(next)) {
        visited.add(next, false);
      }
      if (!visited.at(next)) {
        result.push_back(next);
        dfs(next, graph, visited, result);
      }
    }
  }
  bool
  dfsCycle(const std::string &movie,
           HashTable<std::string, Color, StringHash, StringEqual> &colors,
           HashTable<std::string, std::string, StringHash, StringEqual> &parent,
           karpenkov::List<std::string> &cycle) {
    colors.at(movie) = GRAY;
    karpenkov::List<std::string> &neighbors = adj_.at(movie);
    for (karpenkov::LCIter<std::string> i = neighbors.cbegin();
         i != neighbors.cend(); ++i) {
      std::string next = *i;
      if (colors.at(next) == WHITE) {
        parent.add(next, movie);
        if (dfsCycle(next, colors, parent, cycle))
          return true;
      } else if (colors.at(next) == GRAY) {
        cycle.push_front(next);
        std::string cur = movie;
        while (cur != next) {
          cycle.push_front(cur);
          cur = parent.at(cur);
        }
        cycle.push_front(next);
        return true;
      }
    }
    colors.at(movie) = BLACK;
    return false;
  }
};
} // namespace karpenkov
#endif
