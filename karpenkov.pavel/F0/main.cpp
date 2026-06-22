#include <iostream>
#include <limits>

#include "commands.hpp"

int main() {
  karpenkov::GraphManager manager;

  using Cmd =
      void (*)(std::istream &, std::ostream &, karpenkov::GraphManager &);

  using CommandTable = HashTable<std::string, Cmd, karpenkov::StringHash,
                                 karpenkov::StringEqual>;

  CommandTable commands{karpenkov::StringHash{}, karpenkov::StringEqual{}};

  commands.add("create-order", karpenkov::cmdCreateOrder);
  commands.add("delete-order", karpenkov::cmdDeleteOrder);
  commands.add("insert-movie", karpenkov::cmdInsertMovie);
  commands.add("show-order", karpenkov::cmdShowOrder);
  commands.add("show-dependencies", karpenkov::cmdShowDependencies);
  commands.add("show-successors", karpenkov::cmdShowSuccessors);
  commands.add("find-cycles", karpenkov::cmdFindCycles);
  commands.add("list-orders", karpenkov::cmdListOrders);

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      commands.at(cmd)(std::cin, std::cout, manager);
    } catch (const std::exception &) {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
}
