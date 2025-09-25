#include <list>
#include <memory>
#include <unordered_map>
#include <iostream>

struct CacheEntry {
  int key;
  int value;

  CacheEntry(int k, int v) : key(k), value(v) {}
};

using EntryPtr = std::shared_ptr<CacheEntry>;
using EntryList = std::list<EntryPtr>;

class SharedPtrLRUCache {
private:
  int m_capacity;
  EntryList m_entries;
  std::unordered_map<int, EntryList::iterator> m_cache;

public:
  SharedPtrLRUCache(int capacity) : m_capacity(capacity) {}
  
  void put(int key, int value) {
    auto it = m_cache.find(key);

    // cache hit
    if (it != m_cache.end()) {
      CacheEntry& entry = **it->second;
      entry.value = value;
      m_entries.splice(m_entries.end(), m_entries, it->second);
      return;
    }

    // cache miss
    EntryPtr new_entry = std::make_shared<CacheEntry>(key, value);
    m_entries.push_back(new_entry);
    m_cache[key] = std::prev(m_entries.end());
    while (m_entries.size() > m_capacity) {
      int to_remove = m_entries.front()->key;
      m_cache.erase(to_remove);
      m_entries.pop_front();
    }
  }

  std::shared_ptr<CacheEntry> get(int key) {
    auto it = m_cache.find(key);
    if (it == m_cache.end()) return {};

    m_entries.splice(m_entries.end(), m_entries, it->second);
    return *it->second;
  }

  void print_list() {
    for(const auto& elem : m_entries) {
      std::cout << elem->key << " " << elem->value << " -> ";
    }
    std::cout << std::endl;
  }
};

int main() {
  SharedPtrLRUCache cache(3);
  cache.put(1, 1);
  cache.put(2, 1);
  cache.put(3, 1);

  cache.print_list();

  cache.put(4, 1);

  cache.print_list();

  cache.get(1);
  
  cache.get(2);
  
  cache.print_list();
}
