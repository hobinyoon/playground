#include <cassert>
#include <format>
#include <iterator>
#include <numeric>
#include <vector>

#include "logger.h"

using namespace std;

// https://leetcode.com/problems/word-pattern/solutions/73409/short-c-read-words-on-the-fly
class Solution {
 public:
  bool wordPattern(string pattern, string s) {
    istringstream in(s);
    string word;

    // Note: this should be faster than Solution2. A key is in int instead of
    // string.
    unordered_map<char, int> p2i;
    unordered_map<string, int> w2i;

    int i = 0;
    for (; in >> word; i++) {
      if (i == pattern.size()) {
        // We got more words than the pattern. Make sure we don't go out of
        // bound of pattern.
        return false;
      }
      // Log << format("i={} pattern[i]={} word={}", i, pattern[i], word);

      // If pattern[i] and word are new, they both are assigned to 0, become
      // equal.
      if (p2i[pattern[i]] != w2i[word]) return false;
      // Log << format("w2i.size()={}", w2i.size());

      // Map them to i + 1. +1 is to avoid 0, which messes up with the test
      // above. The values are updated for each occurrance of the same word, and
      // that's okay. It's hard to tell if this is faster than Solution2 below
      // because of this update.
      p2i[pattern[i]] = w2i[word] = i + 1;
    }

    // Number of words should match number of chars in pattern
    return i == pattern.size();
  }
};

class Solution2 {
  vector<string> Parse(const string& s) {
    vector<string> result;
    int l = 0;
    int r = 0;

    for (; r < s.size(); r++) {
      if (s[r] == ' ') {
        result.push_back(s.substr(l, r - 1 - l + 1));
        l = r + 1;
      }
    }
    result.push_back(s.substr(l, r - 1 - l + 1));

    ostringstream ss;
    copy(result.begin(), result.end(), ostream_iterator<string>(ss, "|"));
    Log << format("result {} {}", result.size(), ss.str());

    return result;
  }

 public:
  bool wordPattern(string pattern, string s) {
    vector<string> words = Parse(s);
    if (words.size() != pattern.size()) return false;

    unordered_map<char, string> p_w;
    unordered_map<string, char> w_p;
    for (int i = 0; i < pattern.size(); i++) {
      const string& word = words[i];
      const char p = pattern[i];

      if (!p_w.contains(p))
        p_w[p] = word;
      else if (p_w[p] != word)
        return false;

      if (!w_p.contains(word))
        w_p[word] = p;
      else if (w_p[word] != p)
        return false;
    }
    return true;
  }
};

int main() {
  assert(Solution().wordPattern("abba", "dog cat cat dog") == true);
  assert(Solution().wordPattern("abba", "dog dog dog dog") == false);
  assert(Solution().wordPattern("papap", "po io po io io") == false);
  Log << "done!";
  return 0;
}
