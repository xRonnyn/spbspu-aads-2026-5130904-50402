#include "graph.hpp"
#include <iostream>
#include <string>
using namespace karpenkov;
int main() {
  HashTable<std::string, Graph, StringHash, StringEqual> graphs(StringHash{},
                                                                StringEqual{});
  std::string cmd;
  while (std::cin >> cmd) {
    if (cmd == "create-order") {
      std::string name;
      std::cin >> name;
      if (!graphs.has(name)) {
        Graph g;
        graphs.add(name, g);
      }
    } else if (cmd == "insert-movie") {
      std::string graphName, a, b;
      std::cin >> graphName >> a >> b;
      if (!graphs.has(graphName)) {
        std::cout << "Error: graph not found\n";
        continue;
      }
      graphs.at(graphName).insertMovie(a, b);
    } else if (cmd == "show-order") {
      std::string graphName;
      std::cin >> graphName;
      graphs.at(graphName).showOrder();
    } else if (cmd == "show-dependencies") {
      std::string graphName, movie;
      std::cin >> graphName >> movie;
      graphs.at(graphName).showDependencies(movie);
    } else if (cmd == "show-successors") {
      std::string graphName, movie;
      std::cin >> graphName >> movie;
      graphs.at(graphName).showSuccessors(movie);
    } else if (cmd == "find-cycles") {
      std::string graphName;
      std::cin >> graphName;

      graphs.at(graphName).findCycles();
    } else if (cmd == "exit") {
      break;
    } else {
      std::cout << "Unknown command\n";
    }
  }
  return 0;
}
