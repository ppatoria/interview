#include <iostream>
#include <list>
#include <utility>
#include <vector>

template <typename K, typename V> class Hashtable {
private:
  size_t capacity;
  std::vector<std::list<std::pair<K, V>>> buffer;

  int hash(const K &key) { return std::hash<K>{}(key) % capacity; }

public:
  Hashtable() : capacity(100), buffer(capacity) {}

  void insert(const K &key, const V &value) {
    auto index = hash(key);
    auto &pairs = buffer[index];
    for (auto &pair : pairs) {
      auto &[currentKey, currentValue] = pair;
      if (currentKey == key) {
        currentValue = value;
        return;
      }
    }
    buffer[index].push_back({key, value});
  }

  bool remove(const K &key) {
    auto index = hash(key);
    auto &pairs = buffer[index];
    for (auto pair = pairs.begin(); pair != pairs.end(); ++pair) {
      auto &[currentKey, currentValue] = *pair;
      if (currentKey == key) {
        pairs.erase(pair);
        return true;
      }
    }
    return false;
  }

  const std::pair<K, V> *find(const K &key) {
    auto index = hash(key);
    auto &pairs = buffer[index];
    for (auto pair = pairs.begin(); pair != pairs.end(); ++pair) {
      auto &[currentKey, currentValue] = *pair;
      if (currentKey == key) {
        return &(*pair);
      }
    }
    return nullptr;
  }
};

int main() {

  Hashtable<int, int> ht;

  ht.insert(11, 11);
  ht.insert(21, 21);

  auto result = ht.find(11);
  std::cout << result->first << std::endl;

  if (ht.remove(11)) {
    auto result = ht.find(11);
    if (result)
      std::cout << result->first << std::endl;
    else
      std::cout << "Not found 11 " << std::endl;
  }

  result = ht.find(21);
  if (result)
    std::cout << result->first << std::endl;
  else
    std::cout << "Not found 21 " << std::endl;

  return 0;
}
