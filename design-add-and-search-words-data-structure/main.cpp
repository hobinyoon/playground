#include <cassert>
#include <iostream>
#include <list>
#include <unordered_map>

using namespace std;

class WordDictionary {
  struct Node {
    vector<Node*> children;
    bool term;  // A word can terminate after this node.

    Node() : children(26, nullptr), term(false) {}

    ~Node() {
      for (Node* c : children) delete c;
    }

    Node* Add(char c) {
      if (children[c - 'a'])
        return children[c - 'a'];
      else {
        Node* n = new Node();
        children[c - 'a'] = n;
        return n;
      }
    }

    Node* AddTerm() {
      term = true;
      return nullptr;
    }

    bool Match(const string& word, int i) {
      if (i == word.size()) return term;

      char c = word[i];
      if (c == '.') {
        for (int j = 0; j < 26; j++) {
          if (children[j] && children[j]->Match(word, i + 1)) return true;
        }
        return false;
      }
      Node* n = children[c - 'a'];
      if (!n) return false;
      return n->Match(word, i + 1);
    }

    void Print(int indent = 0) {
      for (int i = 0; i < 26; i++) {
        if (children[i]) {
          cout << string(indent, ' ') << (char)('a' + i) << "\n";
          children[i]->Print(indent + 1);
        }
      }
    }
  };

  Node root;

 public:
  WordDictionary() {}

  void addWord(string word) {
    Node* n = &root;
    for (char c : word) n = n->Add(c);
    n->AddTerm();
  }

  bool search(string word) { return root.Match(word, 0); }

  void print() { root.Print(); }
};

void Test1() {
  WordDictionary wd;
  wd.addWord("bad");
  wd.addWord("dad");
  wd.addWord("mad");
  // wd.print();
  assert(wd.search("pad") == false);
  assert(wd.search("bad") == true);
  assert(wd.search(".ad") == true);
  assert(wd.search("b..") == true);
}

void Test2() {
  // clang-format off
  // ["WordDictionary","addWord","addWord","addWord","addWord","search","search","addWord","search","search","search","search","search","search"]
  // [[]              ,["at"]   ,["and"]  ,["an"]   ,["add"]  ,["a"]   ,[".at"] ,["bat"]  ,[".at"] ,["an."] ,["a.d."],["b."]  ,["a.d"] ,["."]]
  // clang-format on
  WordDictionary wd;
  wd.addWord("at");
  wd.addWord("and");
  wd.addWord("an");
  wd.addWord("add");
  assert(wd.search("a") == false);
}

void Test3() {
  // clang-format off
  // ["WordDictionary","addWord","addWord","search","search","search","search","search","search","search","search"]
  // [         []     ,["a"]    ,["ab"]   ,["a"]   ,["a."]  ,["ab"]  ,[".a"]  ,[".b"]  ,["ab."] ,["."]   ,[".."]]
  // Expected: [null  ,null     ,null     ,true    ,true    ,true    ,false   ,true    ,false   ,true    ,true]
  // clang-format on
  WordDictionary wd;
  wd.addWord("a");
  wd.addWord("ab");
  assert(wd.search("a") == true);
  assert(wd.search("a.") == true);
  assert(wd.search("ab") == true);
  assert(wd.search(".a") == false);
  assert(wd.search(".b") == true);
  assert(wd.search("ab.") == false);
  assert(wd.search(".") == true);
  assert(wd.search("..") == true);
}

int main(int argc, char** argv) {
  Test1();
  Test2();
  Test3();
  cout << __LINE__ << ": done!\n";
  return 0;
}
