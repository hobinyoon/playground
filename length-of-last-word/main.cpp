#include <cassert>

#include "logger.h"

using namespace std;

// Beat 46%
class Solution {
 public:
  int lengthOfLastWord(string s) {
    int r = s.size() - 1;
    for (; r >= 0 && s[r] == ' '; r--)
      ;
    int l = r;
    for (; l >= 0 && s[l] != ' '; l--)
      ;
    return r - l;
  }
};

// Beat 46%
class Solution2 {
 public:
  int lengthOfLastWord(string s) {
    string word;
    istringstream ss(s);
    while (ss >> word)
      ;
    return word.size();
  }
};

int main() {
  assert(Solution().lengthOfLastWord("Hello World") == 5);
  Log << "done!";
  return 0;
}
