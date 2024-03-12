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

struct TreeNode {
  int val;
  TreeNode* left;
  TreeNode* right;
  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode* left, TreeNode* right)
      : val(x), left(left), right(right) {}
  ~TreeNode() {
    if (left) delete left;
    if (right) delete right;
  }
};

ostream& operator<<(ostream& os, TreeNode* n) {
  if (n)
    os << n->val;
  else
    os << "nullptr";
  return os;
}

void PrintTree(TreeNode* n, int depth = 0) {
  if (!n) {
    cout << string(depth * 2, ' ') << n << "\n";
    return;
  }
  cout << string(depth * 2, ' ') << n << "\n";
  if (n->left || n->right) {
    PrintTree(n->left, depth + 1);
    PrintTree(n->right, depth + 1);
  }
}

// Beats 24%. I think leetcode is slow at the moment. Similar code is at the top
// 1%.
class Solution {
  TreeNode* buildTree(const vector<int>& inorder, int il, int ir,
                      const vector<int>& postorder, int pl, int pr,
                      int depth = 0) {
    Log << string(depth * 2, ' ') << "inorder=[" << il << " " << ir
        << "] postorder=[" << pl << " " << pr << "]\n";
    if (il > ir) return nullptr;
    int v = postorder[pr];
    TreeNode* n = new TreeNode(v);

    // Inorder index
    int ii = il;
    for (; ii <= ir; ii++)
      if (inorder[ii] == v) break;

    // The subtree [l, inorder_idx] should be visited first either in inorder or
    // postorder!
    int left_size = ii - 1 - il + 1;
    n->left = buildTree(inorder, il, il + left_size - 1, postorder, pl,
                        pl + left_size - 1);
    n->right = buildTree(inorder, il + left_size + 1, ir, postorder,
                         pl + left_size, pr - 1);
    return n;
  }

 public:
  TreeNode* buildTree(const vector<int>& inorder,
                      const vector<int>& postorder) {
    return buildTree(inorder, 0, inorder.size() - 1, postorder, 0,
                     postorder.size() - 1);
  }
};

// Beat 32%
class Solution3 {
  //            num  idx_in_inorder
  unordered_map<int, int> num_idx;

  TreeNode* buildTree(const vector<int>& inorder, int il, int ir,
                      const vector<int>& postorder, int pl, int pr,
                      int depth = 0) {
    Log << string(depth * 2, ' ') << "inorder=[" << il << " " << ir
        << "] postorder=[" << pl << " " << pr << "]\n";
    if (il > ir) return nullptr;
    TreeNode* n = new TreeNode(postorder[pr]);
    int inorder_idx = num_idx[postorder[pr]];
    int left_size = inorder_idx - 1 - il + 1;
    // The subtree [l, inorder_idx] should be visited first either in inorder or
    // postorder!
    n->left = buildTree(inorder, il, il + left_size - 1, postorder, pl,
                        pl + left_size - 1);
    n->right = buildTree(inorder, il + left_size + 1, ir, postorder,
                         pl + left_size, pr - 1);
    return n;
  }

 public:
  TreeNode* buildTree(const vector<int>& inorder,
                      const vector<int>& postorder) {
    num_idx.clear();
    for (int i = 0; i < inorder.size(); i++) num_idx[inorder[i]] = i;
    return buildTree(inorder, 0, inorder.size() - 1, postorder, 0,
                     postorder.size() - 1);
  }
};

class Solution2 {
  //  num  idx_in_inorder
  unordered_map<int, int> num_idx;
  vector<bool> post_used;

  TreeNode* buildTree(int l, int r, const vector<int>& inorder,
                      const vector<int>& postorder, int depth = 0) {
    Log << string(depth * 2, ' ') << "l=" << l << " r=" << r << "\n";
    if (l > r) return nullptr;
    TreeNode* n = nullptr;
    int inorder_idx = -1;
    for (int i = postorder.size() - 1; i >= 0; i--) {
      if (post_used[i]) continue;
      inorder_idx = num_idx[postorder[i]];
      Log << string(depth * 2, ' ') << "i=" << i
          << " inoder_idx=" << inorder_idx << "\n";
      if (l <= inorder_idx && inorder_idx <= r) {
        post_used[i] = true;
        n = new TreeNode(postorder[i]);
        break;
      }
    }
    assert(n);
    n->left = buildTree(l, inorder_idx - 1, inorder, postorder, depth + 1);
    n->right = buildTree(inorder_idx + 1, r, inorder, postorder, depth + 1);
    return n;
  }

 public:
  TreeNode* buildTree(const vector<int>& inorder,
                      const vector<int>& postorder) {
    num_idx.clear();
    for (int i = 0; i < inorder.size(); i++) num_idx[inorder[i]] = i;
    post_used = vector<bool>(postorder.size(), false);
    return buildTree(0, inorder.size() - 1, inorder, postorder);
  }
};

int main() {
  Solution s;
  unique_ptr<TreeNode> n(s.buildTree({9, 3, 15, 20, 7}, {9, 15, 7, 20, 3}));
  PrintTree(n.get());
  Log << "done!\n";
  return 0;
}
