#include "commands.hpp"

namespace karpenkov {
void cmdCreateOrder(std::istream &in, std::ostream &out,
                    GraphManager &manager) {
  std::string name;
  in >> name;
  manager.createOrder(name);
  out << "Order \"" << name << "\" was created\n";
}

void cmdDeleteOrder(std::istream &in, std::ostream &out,
                    GraphManager &manager) {
  std::string name;
  in >> name;
  manager.deleteOrder(name);
  out << "Order \"" << name << "\" was deleted\n";
}

void cmdInsertMovie(std::istream &in, std::ostream &out,
                    GraphManager &manager) {
  std::string order;
  std::string before;
  std::string after;
  in >> order >> before >> after;
  manager.getOrder(order).insertMovie(before, after);
  out << before << " inserted before " << after << '\n';
}

void cmdShowOrder(std::istream &in, std::ostream &out, GraphManager &manager) {
  std::string order;
  in >> order;
  manager.getOrder(order).showOrder();
}

void cmdShowDependencies(std::istream &in, std::ostream &out,
                         GraphManager &manager) {
  std::string order;
  std::string movie;
  in >> order >> movie;
  manager.getOrder(order).showDependencies(movie);
}

void cmdShowSuccessors(std::istream &in, std::ostream &out,
                       GraphManager &manager) {
  std::string order;
  std::string movie;
  in >> order >> movie;
  manager.getOrder(order).showSuccessors(movie);
}

void cmdFindCycles(std::istream &in, std::ostream &out, GraphManager &manager) {
  std::string order;
  in >> order;
  manager.getOrder(order).findCycles();
}
} // namespace karpenkov
