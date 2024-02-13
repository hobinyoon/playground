#include <cassert>
#include <iostream>
#include <list>
#include <unordered_map>

using namespace std;

class LRUCache {
struct Item {
  int key;
  int value;
};

public:
LRUCache(int capacity)
: c_(capacity) {
  assert(c_ > 0);
}

int get(int key) {
  auto it = m_.find(key);
  if (it == m_.end())
    return -1;

  // Move the list item to the head. Save item before erasing
  Item item = *it->second;
  l_.erase(it->second);
  l_.push_front(item);

  m_[key] = l_.begin();
  return item.value;
}

void put(int key, int value) {
  auto it = m_.find(key);
  if (it != m_.end()) {
    it->second->value = value;
    Item item = *it->second;
    l_.erase(it->second);
    l_.push_front(item);
    m_[key] = l_.begin();
  } else {
    if (m_.size() == c_) {
      // auto it = next(l_.rbegin()).base();  // This syntax didn't work
      auto it = prev(l_.end()); // safe since c_ > 0
      m_.erase(it->key);
      l_.erase(it);
    }
    l_.push_front(Item({key, value}));
    m_[key] = l_.begin();
  }
}

private:
// List of values. No need to store keys. Actually we need to store K as well for updating the map
list<Item> l_;

// Hashmap of K, iterator in the list
unordered_map<int, list<Item>::iterator> m_;

// capacity
int c_;
};

void Test() {
  LRUCache c(2);
  c.put(1, 1);
  c.put(2, 2);
  c.get(1);
  c.put(3,3);
  c.get(2);
  c.put(4,4);
  c.get(1);
  c.get(3);
  c.get(4);
}

int main(int argc, char** argv){
  Test();
  cout << __LINE__ << ":\n";
  return 0;
}
