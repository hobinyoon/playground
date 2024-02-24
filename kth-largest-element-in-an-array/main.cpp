#include <cassert>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <unordered_map>

using namespace std;

class Solution {
  // Returns the last index on the left array
  int Split(vector<int>& nums, int l0, int r0) {
    int l = l0;
    int r = r0;
    int p = nums[(l + r) / 2];
    cout << __LINE__ << ": nums=";
    copy(nums.begin(), nums.end(), ostream_iterator<int>(cout, " "));
    cout << " l=" << l << " r=" << r << " p=" << p << "\n";

    while (true) {
      while (l < r0 && nums[l] < p) l++;
      while (l0 < r && nums[r] > p) r--;
      if (l >= r) break;
      swap(nums[l], nums[r]);
      l++;
      r--;
    }

    cout << __LINE__ << ": nums=";
    copy(nums.begin(), nums.end(), ostream_iterator<int>(cout, " "));
    // cout << " l=" << l << "\n";
    // return l;

    if (l == l0) {
      cout << " l=" << l << "\n";
      return l;  // give left at least 1
    } else {
      cout << " l=" << l - 1 << "\n";
      return l - 1;
    }
  }

  int FindKth(vector<int>& nums, int l0, int r0, int k) {
    if (l0 == r0) {
      assert(k == 1);
      return nums[l0];
    }

    // split while swapping
    int l = Split(nums, l0, r0);
    // left array: [l0, l]. size: l - l0 + 1
    // right array: [l+1, r0]. size: r0 - (l+1) + 1 = r0 - l

    // if size of the right array <= k
    if (r0 - l >= k) {
      // repeat on the right array [r, r0 k]
      return FindKth(nums, l + 1, r0, k);
    } else {
      // find the (k - right.size()) the biggest one on the left array
      return FindKth(nums, l0, l, k - (r0 - l));
    }
  }

 public:
  int findKthLargest(vector<int> nums, int k) {
    return FindKth(nums, 0, nums.size() - 1, k);
  }
};

int main(int argc, char** argv) {
  Solution s;
  assert(s.findKthLargest({3, 2, 1, 5, 6, 4}, 2) == 5);
  assert(s.findKthLargest({99, 99}, 1) == 99);
  assert(s.findKthLargest({3, 1, 2, 4}, 3) == 2);
  assert(s.findKthLargest({5, 2, 4, 1, 3, 6, 0}, 4) == 3);
  cout << __LINE__ << ": done!\n";
  return 0;
}
