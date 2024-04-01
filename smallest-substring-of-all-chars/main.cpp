#include <cassert>
#include <format>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "logger.h"

using namespace std;

string getShortestUniqueSubstring(const vector<char>& arr, const string& str) {
  unordered_set<char> arr_set;
  for (char c : arr) arr_set.insert(c);

  int l = 0;
  int r = -1;
  unordered_map<char, int> char_cnt;

  int s_l = -1;
  int s_r = -1;
  int s_len = INT_MAX;

  while (r < static_cast<int>(str.size())) {
    // Expand
    while (true) {
      r++;
      if (r == str.size()) break;
      auto it = arr_set.find(str[r]);
      if (it == arr_set.end()) continue;
      char_cnt[str[r]]++;
      if (char_cnt.size() == arr.size()) {
        if (r - l + 1 < s_len) {
          s_len = r - l + 1;
          s_l = l;
          s_r = r;
        }
        break;
      }
    }
    if (r == str.size()) break;
    // Log << format("After expand: l={:2} r={:2} s_len={:2}", l, r, s_len);

    // Shrink
    while (l <= r) {
      auto it = arr_set.find(str[l]);
      if (it != arr_set.end()) {
        char_cnt[str[l]]--;
        if (char_cnt[str[l]] == 0) char_cnt.erase(str[l]);
      }
      l++;
      if (char_cnt.size() < arr.size()) break;
      if (r - l + 1 < s_len) {
        s_len = r - l + 1;
        s_l = l;
        s_r = r;
      }
    }
    // Log << format("After shrink: l={:2} r={:2} s_len={:2}", l, r, s_len);
  }

  if (s_l == -1) return "";

  // Log << "s_len=" << s_len;
  return str.substr(s_l, s_r - s_l + 1);
}

int main() {
  assert(getShortestUniqueSubstring({'A'}, "") == "");
  assert(getShortestUniqueSubstring({'A'}, "B") == "");
  assert(getShortestUniqueSubstring({'A'}, "A") == "A");
  // clang-format off
  //                                                  0123456789012345
  assert(getShortestUniqueSubstring({'A', 'B', 'C'}, "ADOBECODEBANCDDD") == "BANC");
  //                                                           ^^^^
  // clang-format on
  Log << "done!";
  return 0;
}
