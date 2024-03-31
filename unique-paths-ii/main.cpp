#include <cassert>
#include <queue>
#include <stack>
#include <vector>

#include "logger.h"

using namespace std;

class Solution {
 public:
  int uniquePathsWithObstacles(vector<vector<int>>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();

    if (grid[0][0] == 1) return 0;
    grid[0][0] = 1;

    for (int k = 1; k < rows + cols - 1; k++) {
      // r + c = k
      // 0 <= c && c < cols
      //   0 <= k - r --> r <= k
      //   k - r < cols --> r > k - cols
      for (int r = max(0, k - cols + 1); r < min(rows, k + 1); r++) {
        int c = k - r;
        // Log << "k=" << k << " r=" << r << " c=" << c;
        if (grid[r][c] == 1) {
          grid[r][c] = 0;
          continue;
        }
        if (r - 1 >= 0) grid[r][c] += grid[r - 1][c];
        if (c - 1 >= 0) grid[r][c] += grid[r][c - 1];
      }
    }
    // Log << grid[rows - 1][cols - 1];
    return grid[rows - 1][cols - 1];
  }
};

class Solution2 {
 public:
  int uniquePathsWithObstacles(vector<vector<int>>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();

    for (int r = 0; r < rows; r++)
      for (int c = 0; c < cols; c++)
        if (grid[r][c] == 1) grid[r][c] = -1;
    if (grid[0][0] == -1 || grid[rows - 1][cols - 1] == -1) return 0;

    grid[rows - 1][cols - 1] = 1;

    queue<pair<int, int>> q;
    q.emplace(rows - 2, cols - 1);
    q.emplace(rows - 1, cols - 2);

    while (!q.empty()) {
      auto [r, c] = q.front();
      q.pop();

      if (r < 0 || c < 0 || r >= rows || c >= cols) continue;
      // Obstacle or already visited
      if (grid[r][c] != 0) continue;
      if (r + 1 < rows && grid[r + 1][c] > 0) grid[r][c] += grid[r + 1][c];
      if (0 <= r - 1 && grid[r - 1][c] > 0) grid[r][c] += grid[r - 1][c];
      if (c + 1 < cols && grid[r][c + 1] > 0) grid[r][c] += grid[r][c + 1];
      if (0 <= c - 1 && grid[r][c - 1] > 0) grid[r][c] += grid[r][c - 1];

      q.emplace(r + 1, c);
      q.emplace(r - 1, c);
      q.emplace(r, c + 1);
      q.emplace(r, c - 1);
    }

    Log << "grid[0][0]=" << grid[0][0];
    return grid[0][0];
  }
};

int Test(const vector<vector<int>>& grid0) {
  vector<vector<int>> grid = grid0;
  return Solution().uniquePathsWithObstacles(grid);
}

int main() {
  assert(Test({{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {1, 0},
               {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {1, 0}, {0, 0},
               {0, 0}, {0, 0}, {0, 0}, {0, 1}, {0, 0}, {0, 0}, {1, 0},
               {0, 0}, {0, 0}, {0, 1}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
               {0, 0}, {0, 0}, {0, 0}, {0, 1}, {0, 0}, {0, 0}, {0, 0},
               {0, 0}, {1, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}) == 0);
  assert(Test({{0, 0, 0}, {0, 1, 0}, {0, 0, 0}}) == 2);

  Log << "done!";
  return 0;
}
