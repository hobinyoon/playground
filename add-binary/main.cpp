#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

class Solution {
 public:
  string addBinary(string a, string b) {
    int ai = a.size() - 1;
    int bi = b.size() - 1;
    int carry = 0;
    string cs;

    while (ai >= 0 || bi >= 0 || carry > 0) {
      int ad;  // a digit
      int bd;  // b digit
      if (ai >= 0)
        ad = a[ai] - '0';
      else
        ad = 0;
      if (bi >= 0)
        bd = b[bi] - '0';
      else
        bd = 0;
      int c = ad + bd + carry;
      if (c >= 2) {
        c -= 2;
        carry = 1;
      } else {
        carry = 0;
      }
      cs = char(c + '0') + cs;
      ai--;
      bi--;
    }
    return cs;
  }
};

int main() {
  Solution s;
  assert(s.addBinary("11", "1") == "100");
  cout << __LINE__ << ": done!\n";
  return 0;
}
