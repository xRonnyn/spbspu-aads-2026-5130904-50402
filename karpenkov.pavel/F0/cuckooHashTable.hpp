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
template <class Key, class Value, class Hash1, class Hash2, class Equal>
bool CuckooHashTable<Key, Value, Hash1, Hash2, Equal>::has(
    const Key &key) const {
  size_t i1 = index1(key);

  if (table1_[i1].occupied && equal_(table1_[i1].key, key)) {
    return true;
  }

  size_t i2 = index2(key);

  if (table2_[i2].occupied && equal_(table2_[i2].key, key)) {
    return true;
  }

  return false;
}
template <class Key, class Value, class Hash1, class Hash2, class Equal>
Value &CuckooHashTable<Key, Value, Hash1, Hash2, Equal>::at(const Key &key) {
  size_t i1 = index1(key);

  if (table1_[i1].occupied && equal_(table1_[i1].key, key)) {
    return table1_[i1].value;
  }

  size_t i2 = index2(key);

  if (table2_[i2].occupied && equal_(table2_[i2].key, key)) {
    return table2_[i2].value;
  }

  throw std::runtime_error("no element with such key");
}
template <class Key, class Value, class Hash1, class Hash2, class Equal>
void CuckooHashTable<Key, Value, Hash1, Hash2, Equal>::add(const Key &key,
                                                           const Value &value) {
  if (has(key)) {
    at(key) = value;
    return;
  }

  Unit current;
  current.key = key;
  current.value = value;
  current.occupied = true;

  bool first = true;

  for (size_t kick = 0; kick < MAX_KICKS; ++kick) {
    if (first) {
      size_t index = index1(current.key);

      if (!table1_[index].occupied) {
        table1_[index] = current;
        ++size_;
        return;
      }
      std::swap(current, table1_[index]);
    } else {
      size_t index = index2(current.key);

      if (!table2_[index].occupied) {
        table2_[index] = current;
        ++size_;
        return;
      }
      std::swap(current, table2_[index]);
    }
    first = !first;
  }

  rehash(capacity_ * 2);
  add(current.key, current.value);
}
template <class Key, class Value, class Hash1, class Hash2, class Equal>
void CuckooHashTable<Key, Value, Hash1, Hash2, Equal>::rehash(
    size_t new_capacity) {
  Unit *old_table1 = table1_;
  Unit *old_table2 = table2_;
  size_t old_capacity = capacity_;

  capacity_ = new_capacity;
  size_ = 0;

  table1_ = new Unit[capacity_];
  table2_ = new Unit[capacity_];

  for (size_t i = 0; i < old_capacity; ++i) {
    if (old_table1[i].occupied) {
      add(old_table1[i].key, old_table1[i].value);
    }
    if (old_table2[i].occupied) {
      add(old_table2[i].key, old_table2[i].value);
    }
  }
  delete[] old_table1;
  delete[] old_table2;
}

#endif
