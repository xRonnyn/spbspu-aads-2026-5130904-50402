#ifndef CUCKOOHASHTABLE_HPP
#define CUCKOOHASHTABLE_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>

template <class Key, class Value, class Hash1, class Hash2, class Equal>
class CuckooIterator;

template <class Key, class Value, class Hash1, class Hash2, class Equal>
class CuckooHashTable {
  friend class CuckooIterator<Key, Value, Hash1, Hash2, Equal>;

public:
  struct Unit {
    Key key;
    Value value;
    bool occupied = false;
  };

  CuckooHashTable(Hash1 hash1, Hash2 hash2, Equal equal);
  ~CuckooHashTable();

  CuckooHashTable(const CuckooHashTable &other);
  CuckooHashTable &operator=(const CuckooHashTable &other);

  CuckooHashTable(CuckooHashTable &&other) noexcept;
  CuckooHashTable &operator=(CuckooHashTable &&other) noexcept;

  void add(const Key &key, const Value &value);

  bool has(const Key &key) const;

  Value &at(const Key &key);
  const Value &at(const Key &key) const;

  Value drop(const Key &key);

  void rehash(size_t new_capacity);

  size_t size() const;

  CuckooIterator<Key, Value, Hash1, Hash2, Equal> begin();
  CuckooIterator<Key, Value, Hash1, Hash2, Equal> end();

private:
  static constexpr size_t MAX_KICKS = 32;

  size_t index1(const Key &key) const;
  size_t index2(const Key &key) const;

  void swap(CuckooHashTable &other) noexcept;

  Unit *table1_;
  Unit *table2_;

  size_t capacity_;
  size_t size_;

  Hash1 hash1_;
  Hash2 hash2_;
  Equal equal_;
};
template <class Key, class Value, class Hash1, class Hash2, class Equal>
CuckooHashTable<Key, Value, Hash1, Hash2, Equal>::CuckooHashTable(Hash1 hash1,
                                                                  Hash2 hash2,
                                                                  Equal equal)
    : capacity_(10), size_(0), hash1_(hash1), hash2_(hash2), equal_(equal) {
  table1_ = new Unit[capacity_];
  table2_ = new Unit[capacity_];
}

template <class Key, class Value, class Hash1, class Hash2, class Equal>
CuckooHashTable<Key, Value, Hash1, Hash2, Equal>::~CuckooHashTable() {
  delete[] table1_;
  delete[] table2_;
}
template <class Key, class Value, class Hash1, class Hash2, class Equal>
size_t CuckooHashTable<Key, Value, Hash1, Hash2, Equal>::size() const {
  return size_;
}

template <class Key, class Value, class Hash1, class Hash2, class Equal>
size_t
CuckooHashTable<Key, Value, Hash1, Hash2, Equal>::index1(const Key &key) const {
  return hash1_(key) % capacity_;
}

template <class Key, class Value, class Hash1, class Hash2, class Equal>
size_t
CuckooHashTable<Key, Value, Hash1, Hash2, Equal>::index2(const Key &key) const {
  return hash2_(key) % capacity_;
}

#endif
