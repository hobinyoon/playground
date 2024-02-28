#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

class Solution {
  int FindSmallest(const vector<int>& nums) {
    int l = 0;
    int r = nums.size() - 1;
    while (l < r) {
      int m = (l + r) / 2;
      // This condition becomes easier when you compare m with the right most
      // element. Compare this with FindSmallest2 below
      if (nums[m] < nums[r])
        r = m;
      else  // r < m
        l = m + 1;
    }
    return l;
  }

  int FindSmallest2(const vector<int>& nums) {
    int l = 0;
    int r = nums.size() - 1;
    while (l < r) {
      if (nums[l] < nums[r]) return l;
      int m = (l + r) / 2;
      if (nums[m] < nums[l])
        r = m;
      else  // num[m] > num[l]
        l = m + 1;
    }
    return l;
  }

  int Index(const vector<int>& nums, int l, int r, int t) {
    while (l <= r) {
      int m = (l + r) / 2;
      if (t == nums[m]) return m;
      if (t < nums[m])
        r = m - 1;
      else
        l = m + 1;
    }
    return -1;
  }

 public:
  int search(vector<int> nums, int target) {
    int p = FindSmallest(nums);
    cout << __LINE__ << ": p=" << p << "\n";
    if (target <= nums[nums.size() - 1])
      return Index(nums, p, nums.size() - 1, target);
    else
      return Index(nums, 0, p - 1, target);
  }
};

int main() {
  Solution s;
  // assert(s.search({4, 5, 6, 7, 0, 1, 2}, 0) == 4);
  // assert(s.search({1}, 1) == 0);
  assert(s.search({5, 1, 3}, 5) == 0);
  cout << __LINE__ << ": done!\n";
  return 0;
}
