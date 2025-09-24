#include <iostream>
#include <list>
#include <optional>
#include <unordered_map>

struct CacheEntry {
  int key;
  int value;
};

class BasicLRUCache {
private:
  int m_max_entries;
  std::list<CacheEntry> entries;
  std::unordered_map<int, std::list<CacheEntry>::iterator> cache;

public:
  BasicLRUCache(int max_entries) : m_max_entries(max_entries) {}

  void put(int key, int value) {
    auto it = cache.find(key);
    if (it != cache.end()) {
      it->second->value = value;
      entries.splice(entries.end(), entries, it->second);
      return;
    }

    CacheEntry new_entry{key, value};
    entries.push_back(new_entry);
    cache[key] = std::prev(entries.end());

    while (entries.size() > m_max_entries) {
      auto to_remove = entries.begin();
      cache.erase(to_remove->key);
      entries.pop_front();
    }
  }

  std::optional<CacheEntry> get(int key) {
    auto it = cache.find(key);
    if (it == cache.end()) return std::nullopt;

    entries.splice(entries.end(), entries, it->second);
    return *it->second;
  }
};

int main() {
  BasicLRUCache test(3);

  test.put(1, 1);
  test.put(2, 2);
  test.put(3, 3);
  
  auto result = test.get(1);
  if (result.has_value()) std::cout << result->key << " " << result->value << std::endl;
  else std::cout << "No result" << std::endl;

  std::cout << "Expected value: 1 1" << std::endl;

  test.put(4, 4);

  result = test.get(2);
  if (result.has_value()) std::cout << result->key << " " << result->value << std::endl;
  else std::cout << "No result" << std::endl;

  std::cout << "Expected value: No result" << std::endl;

  result = test.get(1);
  if (result.has_value()) std::cout << result->key << " " << result->value << std::endl;
  else std::cout << "No result" << std::endl;

  std::cout << "Expected value: 1 1" << std::endl;
}
