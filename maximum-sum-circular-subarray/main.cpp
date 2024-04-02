#include <cassert>
#include <format>
#include <numeric>
#include <queue>
#include <stack>
#include <vector>

#include "logger.h"

using namespace std;

// Using the intuition from
// https://leetcode.com/problems/maximum-sum-circular-subarray/solutions/178422/one-pass
class Solution {
  int maxSubarraySum(vector<int>& nums) {
    assert(nums.size() > 0);

    int s = nums[0];
    int max_s = s;
    for (int r = 1; r < nums.size(); r++) {
      if (s <= 0) {
        s = nums[r];
      } else {
        s += nums[r];
      }
      max_s = max(max_s, s);
    }
    return max_s;
  }

 public:
  int maxSubarraySumCircular(vector<int>& nums) {
    int mid_subarray_sum = maxSubarraySum(nums);
    // Log << "mid_subarray_sum=" << mid_subarray_sum;

    int total = 0;
    for (int i = 0; i < nums.size(); i++) {
      total += nums[i];
      nums[i] *= -1;
    }
    int inverted_mid_subarray_sum = maxSubarraySum(nums);
    // Log << "inverted_mid_subarray_sum=" << inverted_mid_subarray_sum;
    // When the numbers are all negative
    if (total + inverted_mid_subarray_sum == 0) return mid_subarray_sum;

    int boundary_subarrays_sum = total + inverted_mid_subarray_sum;
    return max(mid_subarray_sum, boundary_subarrays_sum);
  }
};

// I think either 5(6) or 7 is a good one.
class Solution7 {
 public:
  int maxSubarraySumCircular(vector<int>& nums) {
    assert(nums.size() > 0);

    // nums1 is an imaginary array of size nums.size() * 2 - 1
    //   nums  nums
    //   |----||----|
    //   nums1
    //   |---------|
    int nums1_size = nums.size() * 2 - 1;

    // [l, r]: subarray index in nums1
    int l = 0;
    int r = nums.size() - 1;
    int s = accumulate(nums.begin(), nums.end(), 0);

    int l1 = l;
    int s1 = 0;
    while (l1 < r) {
      s1 += nums[l1];
      l1++;
      if (s1 <= 0) {
        s -= s1;
        s1 = 0;
        l = l1;
      }
    }
    int max_s = s;

    // Log << format("l={} r={} s={}", l, r, s);
    r++;

    for (; r < nums1_size; r++) {
      if (s > 0) {
        // keep l
        s += nums[r % nums.size()];
        if (r - l == nums.size()) {
          s -= nums[l % nums.size()];
          l++;
          // Log << "advanced l to " << l;
          // After advancing l, the sum of the following numbers could be <= 0.
          // Advance l after those.
          int l1 = l;
          int s1 = 0;
          while (l1 < r) {
            s1 += nums[l1 % nums.size()];
            l1++;
            if (s1 <= 0) {
              s -= s1;
              s1 = 0;
              l = l1;
              // Log << "advanced l to " << l;
            }
          }
        }
      } else {  // s <= 0
        l = r;
        s = nums[r % nums.size()];
      }
      max_s = max(max_s, s);
    }

    return max_s;
  }
};

// Caching nums.size() didn't help at all
class Solution6 {
 public:
  int maxSubarraySumCircular(vector<int>& nums) {
    int nums_size = nums.size();
    assert(nums.size() > 0);

    // nums1 is an imaginary array of size nums.size() * 2 - 1
    //   nums  nums
    //   |----||----|
    //   nums1
    //   |---------|
    int nums1_size = nums_size * 2 - 1;

    // [l, r]: subarray index in nums1
    int l = 0;
    int s = 0;
    int max_s = INT_MIN;

    for (int r = 0; r < nums1_size; r++) {
      if (s > 0) {
        // keep l
        s += nums[r % nums_size];
        if (r - l == nums_size) {
          s -= nums[l % nums_size];
          l++;
          // Log << "advanced l to " << l;
          // After advancing l, the sum of the following numbers could be <= 0.
          // Advance l after those.
          int l1 = l;
          int s1 = 0;
          while (l1 < r) {
            s1 += nums[l1 % nums_size];
            l1++;
            if (s1 <= 0) {
              s -= s1;
              s1 = 0;
              l = l1;
              // Log << "advanced l to " << l;
            }
          }
        }
      } else {  // s <= 0
        l = r;
        s = nums[r % nums_size];
      }
      max_s = max(max_s, s);
    }

    return max_s;
  }
};

// Beat 5%. Can we start from [l, r] = [0, nums.size() - 1]? Probably. Currently
// we are doing duplicate work.
class Solution5 {
 public:
  int maxSubarraySumCircular(vector<int>& nums) {
    assert(nums.size() > 0);

    // nums1 is an imaginary array of size nums.size() * 2 - 1
    //   nums  nums
    //   |----||----|
    //   nums1
    //   |---------|
    int nums1_size = nums.size() * 2 - 1;

    // [l, r]: subarray index in nums1
    int l = 0;
    int s = 0;
    int max_s = INT_MIN;

    for (int r = 0; r < nums1_size; r++) {
      if (s > 0) {
        // keep l
        s += nums[r % nums.size()];
        if (r - l == nums.size()) {
          s -= nums[l % nums.size()];
          l++;
          // Log << "advanced l to " << l;
          // After advancing l, the sum of the following numbers could be <= 0.
          // Advance l after those.
          int l1 = l;
          int s1 = 0;
          while (l1 < r) {
            s1 += nums[l1 % nums.size()];
            l1++;
            if (s1 <= 0) {
              s -= s1;
              s1 = 0;
              l = l1;
              // Log << "advanced l to " << l;
            }
          }
        }
      } else {  // s <= 0
        l = r;
        s = nums[r % nums.size()];
      }
      max_s = max(max_s, s);
    }

    return max_s;
  }
};

class Solution3 {
 public:
  int maxSubarraySumCircular(vector<int>& nums0) {
    // Log << "nums0.size()=" << nums0.size();

    // We can rewrite this with a pointer arithmetic -> Solution5
    vector<int> nums = nums0;
    for (int i = 0; i + 1 < nums0.size(); i++) nums.push_back(nums0[i]);
    // Log << format("nums.size()={}", nums.size());

    int l = 0;
    int r = 0;
    int s = nums[r];
    int max_s = s;
    // int max_l = -1;
    // int max_r = -1;

    while (true) {
      r++;
      if (r == nums.size()) break;
      if (s > 0) {
        // keep l
        s += nums[r];
        if (r - l == nums0.size()) {
          s -= nums[l];
          l++;
          // Log << "advanced l to " << l;
          // After advancing l, the sum of the following numbers could be <= 0.
          // Advance l after those.
          int l1 = l;
          int s1 = 0;
          while (l1 < r) {
            s1 += nums[l1];
            l1++;
            if (s1 <= 0) {
              s -= s1;
              s1 = 0;
              l = l1;
              // Log << "advanced l to " << l;
            }
          }
        }
      } else {  // s <= 0
        l = r;
        s = nums[r];
      }
      max_s = max(max_s, s);
      // bool max_s_updated = false;
      // if (max_s < s) {
      //   max_s = s;
      //   max_l = l;
      //   max_r = r;
      //   max_s_updated = true;
      // }
      // Log << format("l={:3} r={:3} s={:4} max_s={:4} max_l={:3} max_r={:3}
      // {}", l, r, s, max_s, max_l, max_r, max_s_updated ? "*" : ""); l= 61
      // r=242 s=5519 max_s=5519 max_l= 61 max_r=242 Let's check if this is
      // correct so far. Run the brute force by l= 61 r=242. Matched
      //   max_s=5519 max_l=61 max_r=242
      // Optimal is: max_s=5803 max_l=284 max_r=542
    }

    // Log << format("max_s={} max_l={} max_r={}", max_s, max_l, max_r);
    return max_s;
  }
};

// Brute force
class Solution2 {
 public:
  int maxSubarraySumCircular(vector<int>& nums0) {
    Log << "nums0.size()=" << nums0.size();
    vector<int> nums = nums0;
    for (int i = 0; i + 1 < nums0.size(); i++) nums.push_back(nums0[i]);
    // We can rewrite this with a pointer arithmetic later -> Solution5

    int max_s = INT_MIN;
    int max_l = -1;
    int max_r = -1;
    for (int l = 0; l < nums0.size(); l++) {
      int s = 0;
      for (int r = l; r - l + 1 < nums0.size(); r++) {
        s += nums[r];
        if (max_s < s) {
          max_s = s;
          max_l = l;
          max_r = r;
        }
        // max_s = max(max_s, s);
      }
      if (l == 61) break;
    }
    Log << "max_s=" << max_s << " max_l=" << max_l << " max_r=" << max_r;
    return max_s;
  }
};

int Test(const vector<int>& param0) {
  // Log;
  vector<int> param = param0;
  return Solution().maxSubarraySumCircular(param);
}

int main() {
  assert(Test({1, -2, 3, -2}) == 3);
  assert(Test({5, -3, 5}) == 10);
  assert(Test({-3, -2, -3}) == -2);

  // 300 items in the array
  assert(Test({52,   183,  124,  154,  -170, -191, -240, 107,  -178, 171,  75,
               186,  -125, 61,   -298, 284,  21,   -73,  -294, 253,  146,  248,
               -248, 127,  26,   289,  118,  -22,  -300, 26,   -116, -113, -44,
               29,   252,  -278, 47,   254,  -106, 246,  -275, 42,   257,  15,
               96,   -298, -69,  -104, -239, -95,  -4,   76,   -202, 156,  -14,
               -178, 188,  -84,  78,   -195, -125, 28,   109,  125,  -25,  -53,
               58,   287,  55,   -296, 198,  281,  53,   -160, 146,  298,  25,
               -41,  -3,   27,   -242, 169,  287,  -281, 19,   91,   213,  115,
               211,  -218, 124,  -25,  -272, 278,  296,  -177, -166, -192, 97,
               -49,  -25,  168,  -81,  6,    -94,  267,  293,  146,  -1,   -258,
               256,  283,  -156, 197,  28,   78,   267,  -151, -230, -66,  100,
               -94,  -66,  -123, 121,  -214, -182, 187,  65,   -186, 215,  273,
               243,  -99,  -76,  178,  59,   190,  279,  300,  217,  67,   -117,
               170,  163,  153,  -37,  -147, -251, 296,  -176, 117,  68,   258,
               -159, -300, -36,  -91,  -60,  195,  -293, -116, 208,  175,  -100,
               -97,  188,  79,   -270, 80,   100,  211,  112,  264,  -217, -142,
               5,    105,  171,  -264, -247, 138,  275,  227,  -86,  30,   -219,
               153,  10,   -66,  267,  22,   -56,  -70,  -234, -66,  89,   182,
               110,  -146, 162,  -48,  -201, -240, -225, -15,  -275, 129,  -117,
               28,   150,  84,   -264, 249,  -85,  70,   -140, -259, 26,   162,
               5,    -203, 143,  184,  101,  140,  207,  131,  177,  274,  -178,
               -79,  14,   -36,  104,  52,   31,   257,  273,  -52,  74,   276,
               104,  -133, -255, 188,  -252, 229,  200,  -74,  -39,  -250, 142,
               -201, -196, -43,  -40,  255,  -149, -299, -197, -175, -96,  -155,
               -196, -24,  12,   79,   71,   -144, -59,  -120, 227,  -256, -163,
               -297, 116,  286,  -283, -31,  -221, -41,  121,  -170, 160,  205,
               8,    88,   25,   -272, -107, 292,  -180, 299,  94,   -97,  -81,
               -134, 37,   238}) == 5803);

  assert(Test({-5, 3, 5}) == 8);
  Log << "done!";
  return 0;
}
