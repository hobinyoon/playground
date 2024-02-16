#include <cassert>
#include <iostream>
#include <list>
#include <unordered_map>

using namespace std;

class Trie {

struct Node {
  unordered_map<char, Node*> links;

  ~Node() {
    for (auto [c, n]: links)
      delete n;
  }

  Node* Insert(char c) {
    // cout << __LINE__ << ": " << c << "\n";
    auto it = links.find(c);
    if (it != links.end()) {
      return it->second;
    } else {
      Node* child = new Node();
      links[c] = child;
      return child;
    }
  }

  Node* Get(char c) {
    auto it = links.find(c);
    if (it == links.end())
      return nullptr;
    else
      return it->second;
  }
};

Node root;

public:
Trie() {
}

~Trie() {
}

void insert(string word) {
  Node* n = &root;
  for (char c: word) {
    n = n->Insert(c);
  }
  n->Insert('\0');
}

bool search(string word) {
  Node* n = &root;
  for (char c: word) {
    n = n->Get(c);
    if (!n)
      return false;
  }
  return n->Get('\0');
}

bool startsWith(string prefix) {
  Node* n = &root;
  for (char c: prefix) {
    n = n->Get(c);
    if (!n)
      return false;
  }
  return true;
}

};

int main(int argc, char** argv){
  Trie t;
  t.insert("apple");
  assert(t.search("apple") == true);
  assert(t.search("app") == false);
  assert(t.startsWith("app") == true);
  t.insert("app");
  assert(t.search("app") == true);
  cout << __LINE__ << ": done!\n";
  return 0;
}
