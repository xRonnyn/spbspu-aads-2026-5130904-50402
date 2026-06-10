#include <iostream>
#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP
enum State { EMPTY, OCCUPIED, TOMBSTONE };
template <class Key, class Value, class Hash, class Equal> class HashTable {
public:
  HashTable(Hash hash, Equal equal)
      : size_(0), capacity_(10), hash_(hash), equal_(equal) {
    table_ = new Unit[capacity_];
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
