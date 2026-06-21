#include "graph.hpp"
#include <iostream>
using namespace karpenkov;
int main() {
  Graph graph;
  graph.insertMovie("A", "B");
  graph.insertMovie("B", "C");
  graph.insertMovie("D", "C");
  std::cout << "Order:\n";
  graph.showOrder();
  std::cout << "Dependencies of B:\n";
  graph.showDependencies("B");
  std::cout << '\n';
  std::cout << "Successors of C:\n";
  graph.showSuccessors("C");
  std::cout << '\n';
  graph.findCycles();
  return 0;
}
