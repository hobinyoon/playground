#include <cassert>
#include <iostream>
#include <map>
#include <numeric>
#include <vector>

#define Log std::cout << __LINE__ << " " << __FUNCTION__ << ": "

using namespace std;

class Solution {
 public:
  int maxPoints(const vector<vector<int>>& points) {
    int n = points.size();
    if (n <= 2) return n;

    int max_cnt = 0;
    for (int i = 0; i < n - 1; i++) {
      map<pair<int, int>, int> dxdy_cnt;
      for (int j = i + 1; j < n; j++) {
        // Log << "(" << points[i][0] << points[i][1] << ") (" << points[j][0]
        // << points[j][1] << ")\n";
        int dx = points[j][0] - points[i][0];
        int dy = points[j][1] - points[i][1];
        // gcd is guaranteed to be not 0 since all points are unique
        int gcd = std::gcd(dx, dy);
        dx = dx / gcd;
        dy = dy / gcd;
        if (dx < 0) {
          dx *= -1;
          dy *= -1;
        } else if (dx == 0 && dy < 0) {
          dy *= -1;
        }
        // Log << "dx=" << dx << " dy=" << dy << " gcd=" << gcd << "\n";
        dxdy_cnt[make_pair(dx, dy)]++;
      }
      // Log << "---\n";
      // for (const auto& [dxdy, cnt] : dxdy_cnt) {
      //   auto [dx, dy] = dxdy;
      //   Log << "dx=" << dx << " dy=" << dy << " cnt=" << cnt << "\n";
      // }
      // Log << "\n";

      for (const auto& [dxdy, cnt] : dxdy_cnt) max_cnt = max(max_cnt, cnt + 1);
    }
    // Log << "max_cnt=" << max_cnt << "\n";
    return max_cnt;
  }
};

int main() {
  Solution s;
  // assert(s.maxPoints({{1, 1}, {2, 2}, {3, 3}}) == 3);
  /*
    4*
    3 *  *
    2  *
    1*  *
    012345
  */
  assert(s.maxPoints({{1, 1}, {3, 2}, {5, 3}, {4, 1}, {2, 3}, {1, 4}}) == 4);
  /*
    4*
    3*
    2
    1*
    0*
   -1*
   -2*
   -3
   -4*
  */
  // assert(s.maxPoints({{0,1},{0,0},{0,4},{0,-2},{0,-1},{0,3},{0,-4}}) == 7);
  Log << "done!\n";
  return 0;
}
