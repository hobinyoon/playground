#include <cassert>
#include <chrono>
#include <iostream>
#include <map>
#include <queue>
#include <stack>
#include <thread>
#include <vector>

#define LogEnabled true
#define Log          \
  if (!LogEnabled) { \
  } else             \
    std::cout << __LINE__ << " " << __FUNCTION__ << ": "

using namespace std;

class Solution {
 public:
  // Beat 99%
  int minCost(int n, vector<int> cuts) {
    cuts.push_back(0);
    cuts.push_back(n);
    sort(cuts.begin(), cuts.end());
    // min_cost from in the [l, r] range. [first_idx, second_idx]
    //   Note: We could have saved half of the memory with smart indexing.
    vector<vector<int>> cache(cuts.size(), vector<int>(cuts.size(), 0));

    // row: left
    // col: right
    //  01234
    // 0*****
    // 1**++o
    // 2****+
    // 3****+
    // [1 2] is always 0
    // To get [1 3], we need [1 2] [2 3]
    // To get [1 4], we need [1 2] [2 4], [1 3] [3 4]
    // int cost = cuts[c] - cuts[r] + MinCost(r, i, cuts) + MinCost(i, c, cuts);

    // For each cell, we need to compare at most 2n cells. This seems like n^3
    // time.
    // Space O(n^2). Keeping a diagonal strip is not enough.

    // w: width
    for (int w = 2; w < cuts.size(); w++) {
      // for (int r = 0; r < cuts.size(); r++) {
      for (int r = 0; r + w < cuts.size(); r++) {
        int c = r + w;
        // min cost in [r:c] ([left:right])
        int min_cost = INT_MAX;
        for (int i = r + 1; i < c; i++)
          min_cost = min(min_cost, cache[r][i] + cache[i][c]);
        assert(min_cost != INT_MAX);
        min_cost += cuts[c] - cuts[r];
        cache[r][c] = min_cost;
      }
    }

    return cache[0][cuts.size() - 1];
  }
};

// Beat 25%
class Solution4 {
  // min_cost from in the [l, r] range. [first_idx, second_idx]
  vector<vector<int>> cache;

  int MinCost(int l, int r, const vector<int>& cuts) {
    if (cache[l][r] != -1) return cache[l][r];

    int min_cost = INT_MAX;
    for (int c = l + 1; c < r; c++) {
      int cost = cuts[r] - cuts[l] + MinCost(l, c, cuts) + MinCost(c, r, cuts);
      min_cost = min(min_cost, cost);
    }
    if (min_cost == INT_MAX) min_cost = 0;

    cache[l][r] = min_cost;
    return min_cost;
  }

 public:
  int minCost(int n, vector<int> cuts) {
    // We could use a 2D array, but only half of it will be utilized
    // Index in cuts
    //   0 -> 0
    //   1 -> cuts[0]
    //   cuts.size() -> cuts[cuts.size() - 1]
    //   cuts.size() + 1 -> n
    //
    // Or it might be easier to index if we put the sentinal values to cuts
    cuts.push_back(0);
    cuts.push_back(n);
    sort(cuts.begin(), cuts.end());
    cache = vector<vector<int>>(cuts.size(), vector<int>(cuts.size(), -1));

    return MinCost(0, cuts.size() - 1, cuts);
  }
};

// Beats 5%
class Solution3 {
  //       l    r     min_cost
  map<pair<int, int>, int> cache;

  int MinCost(int l, int r, const vector<int>& cuts) {
    auto cache_key = make_pair(l, r);
    auto it = cache.find(cache_key);
    if (it != cache.end()) return it->second;

    int min_cost = INT_MAX;
    for (int c : cuts) {
      // We could do binary search. I think it's ok for now
      if (c <= l || r <= c) continue;
      int cost = r - l + MinCost(l, c, cuts) + MinCost(c, r, cuts);
      min_cost = min(min_cost, cost);
    }
    if (min_cost == INT_MAX) min_cost = 0;

    cache[cache_key] = min_cost;
    return min_cost;
  }

 public:
  int minCost(int n, vector<int> cuts) {
    sort(cuts.begin(), cuts.end());
    cache.clear();
    return MinCost(0, n, cuts);
  }
};

class Solution2 {
  //       l    r     min_cost
  map<pair<int, int>, int> cache;

  int MinCost(int l, int r, const vector<int>& cuts, int depth = 0) {
    Log << string(depth * 2, ' ') << "l=" << l << " r=" << r << "\n";
    auto cache_key = make_pair(l, r);
    auto it_c = cache.find(cache_key);
    if (it_c != cache.end()) return it_c->second;

    double m = (l + r) / 2.0;
    // to avoid the floating point inaccuracy
    int two_m = l + r;

    // find the closest to m from cuts
    // l c0 c1 m r
    // boundary cases:
    //   if it == end(), choose cuts.back()
    //   if it == begin(), choose it

    auto it = lower_bound(cuts.begin(), cuts.end(), m);
    vector<int> cut_cand;
    if (it == cuts.end()) {
      it--;
      // Log << "l=" << l << " r=" << r << " *it=" << *it << "\n";
      if (l < *it && *it < r) cut_cand.push_back(*it);
    } else if (it == cuts.begin()) {
      // Log << "l=" << l << " r=" << r << " *it=" << *it << "\n";
      if (l < *it && *it < r) cut_cand.push_back(*it);
    } else {
      // it-1 or it
      // avoid 0.5 coming from the double error by doubling them
      int dist1 = abs(two_m - (*it) * 2);
      auto it2 = it;
      it2--;
      int dist2 = abs(two_m - (*it2) * 2);
      if (dist1 <= dist2 && l < *it && *it < r) cut_cand.push_back(*it);
      if (dist1 >= dist2 && l < *it2 && *it2 < r) cut_cand.push_back(*it2);
    }
    Log << string(depth * 2, ' ') << "cut_cand=";
    if (LogEnabled) {
      copy(cut_cand.begin(), cut_cand.end(), ostream_iterator<int>(cout, " "));
      cout << "\n";
    }

    int min_cost = INT_MAX;
    int min_c = -1;
    for (int c : cut_cand) {
      int cost = r - l + MinCost(l, c, cuts, depth + 1) +
                 MinCost(c, r, cuts, depth + 1);
      // min_cost = min(min_cost, cost);
      if (cost < min_cost) {
        min_cost = cost;
        min_c = c;
      }
    }
    if (min_cost == INT_MAX) min_cost = 0;  // No c in [l, r]
    Log << string(depth * 2, ' ') << "l=" << l << " r=" << r
        << " min_c=" << min_c << " min_cost=" << min_cost << "\n";
    cache[cache_key] = min_cost;
    return min_cost;
  }

 public:
  int minCost(int n, const vector<int>& cuts0) {
    cache.clear();
    auto cuts = cuts0;
    sort(cuts.begin(), cuts.end());
    int r = MinCost(0, n, cuts);
    Log << "r=" << r << "\n";
    return r;
  }
};

int main() {
  Solution s;
  /*
  01234567
  |------|
   | |||
  */
  assert(s.minCost(7, {1, 3, 4, 5}) == 16);
  assert(s.minCost(9, {5, 6, 1, 4, 2}) == 22);
  assert(s.minCost(30, {13, 25, 16, 20, 26, 5, 27, 8, 23, 14, 6, 15, 21, 24, 29,
                        1, 19, 9, 3}) == 127);
  Log << "done!\n";
  return 0;
}
