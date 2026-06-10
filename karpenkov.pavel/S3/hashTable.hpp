#include <iostream>
#include <stdexcept>
#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP
enum State { EMPTY, OCCUPIED, TOMBSTONE };
template <class Key, class Value, class Hash, class Equal> class HashTable {
public:
  HashTable(Hash hash, Equal equal)
      : size_(0), capacity_(10), hash_(hash), equal_(equal) {
    table_ = new Unit[capacity_];
  }
  void add(Key k, Value v) {
    size_t index = hash_(k) % capacity_;
    for (size_t i = 0; i < capacity_; ++i) {
      size_t cur = (index + cur) % capacity_;
      if (table_[cur].state == EMPTY || table_[cur].state == TOMBSTONE) {
        table_[cur].key = k;
        table_[cur].value = v;
        table_[cur].state = OCCUPIED;
        size_++;
        return;
      }
      if (table_[cur].state == OCCUPIED && equal_(table_[cur].key, k)) {
        table_[cur].key = k;
        return;
      }
    }
    throw std::runtime_error("HashTable is full");
  }
  bool has(Key k) {
    size_t index = hash_(k) % capacity_;
    for (size_t i = 0; i < capacity_; ++i) {
      size_t cur = (index + 1) % capacity_;
      if (table_[cur].state == EMPTY) {
        return false;
      } else if (table_[cur].state == OCCUPIED && equal_(table_[cur].key, k)) {
        return true;
      }
    }
    return false;
  }

private:
  struct Unit {
    Key key;
    Value value;
    State state = EMPTY;
  };
  Unit *table_;
  size_t size_;
  size_t capacity_;
  Hash hash_;
  Equal equal_;
};

#endif
