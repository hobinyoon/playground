#include <cassert>
#include <iostream>
#include <list>
#include <map>
#include <unordered_map>

using namespace std;

class Solution {
  // Do we need memoization? Let's print out the cases where we revist the same
  // subproblem.
  // Ok, most of them are visited once, but some of them are visited multiple
  // times!
  map<pair<int, int>, int> ti_pi_cnt_;

  map<pair<int, int>, bool> cache_;

  bool isMatch(const string &t, const string &p, int ti, int pi) {
    auto it = cache_.find({ti, pi});
    if (it != cache_.end())
      return it->second;

    ti_pi_cnt_[make_pair(ti, pi)]++;
    bool result = false;

    if (pi == p.size())
      return ti == t.size();

    assert(pi <= p.size());
    // assert(ti <= t.size());
    if (ti > t.size()) {
      cout << __LINE__ << ": ti=" << ti << " pi=" << pi
           << " t.size()=" << t.size() << " p.size()=" << p.size() << "\n";
      // 32: ti=2 pi=4 t.size()=1 p.size()=6
      //
      // 01
      // a
      //  ^
      //
      // 0123456
      // .*..a*
      //     ^
      //
      // 01
      // a
      //  ^
      //
      // 0123456
      // .*..a*
      //    ^
      assert(false);
    }

    if (pi + 1 < p.size() && p[pi + 1] == '*') {
      if (isMatch(t, p, ti, pi + 2))
        result = true;
      else {
        for (int i = ti; i < t.size(); i++) {
          // if p[pi] == ., consume any number of any chars in t
          // else, consume any number of p[pi]s in t
          if (p[pi] != '.' && t[i] != p[pi])
            break;
          if (isMatch(t, p, i + 1, pi + 2)) {
            result = true;
            break;
          }
        }
      }
    } else {
      // Assume pattern[pi] != *, well formed pattern
      if (ti < t.size() && (p[pi] == '.' || t[ti] == p[pi]))
        if (isMatch(t, p, ti + 1, pi + 1))
          result = true;
    }

    cache_[{ti, pi}] = result;
    return result;
  }

  bool isMatch_1(const string &t, const string &p, int ti, int pi) {
    ti_pi_cnt_[make_pair(ti, pi)]++;

    if (pi == p.size())
      return ti == t.size();

    assert(pi <= p.size());
    assert(ti <= t.size());

    if (pi + 1 < p.size() && p[pi + 1] == '*') {
      if (isMatch(t, p, ti, pi + 2))
        return true;

      for (int i = ti; i < t.size(); i++) {
        // if p[pi] == '.'
        //   // consume any char in t
        //   ;
        // else
        //   // consume p[pi]
        //   if t[i] != p[pi]
        //     break

        // if p[pi] == ., consume any number of any chars in t
        // else, consume any number of p[pi]s in t
        if (p[pi] != '.' && t[i] != p[pi])
          break;
        if (isMatch(t, p, i + 1, pi + 2))
          return true;
      }
      return false;
    } else {
      // Assume pattern[pi] != *, well formed pattern
      if (p[pi] == '.' || t[ti] == p[pi])
        return isMatch(t, p, ti + 1, pi + 1);
      return false;
    }
  }

public:
  bool isMatch(string s, string p) {
    cache_.clear();

    // bool r = isMatch(s, p, 0, 0);
    // cout << __LINE__ << ":";
    // for (const auto& [ti_pi, cnt]: ti_pi_cnt_) {
    //   const auto& [ti, pi] = ti_pi;
    //   cout << ti << " " << pi << " " << cnt << "\n";
    // }
    // cout << "\n";
    // return r;

    return isMatch(s, p, 0, 0);
  }
};

int main(int argc, char **argv) {
  Solution s;
  assert(s.isMatch("abcaaaaaaabaabcabac", ".*ab.a.*a*a*.*b*b*") == true);
  //   abcaaaaaaabaabcabac
  // .*ab.a.*a*a*.*       b*b*
  assert(s.isMatch("a", ".*..a*") == false);
  cout << __LINE__ << ": done!\n";
  return 0;
}
