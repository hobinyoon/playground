#include <unistd.h>

#include <cassert>
#include <chrono>
#include <iostream>
#include <map>
#include <queue>
#include <stack>
#include <thread>
#include <vector>

#define Log std::cout << __LINE__ << " " << __FUNCTION__ << ": "

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

// BFS
TreeNode* BuildTree(vector<TreeNode*>& nodes) {
  assert(nodes.size() > 0);

  TreeNode* root = nodes[0];
  queue<TreeNode*> q;
  q.push(root);

  int i = 1;
  while (!q.empty()) {
    TreeNode* n = q.front();
    q.pop();
    if (!n) continue;
    if (i == nodes.size()) break;
    n->left = nodes[i++];
    q.push(n->left);
    if (i == nodes.size()) break;
    n->right = nodes[i++];
    q.push(n->right);
  }
  return root;
}

unique_ptr<TreeNode> BuildTree(string tree_str) {
  vector<TreeNode*> nodes;
  while (true) {
    size_t p = tree_str.find(',');
    string n_str = tree_str.substr(0, p);
    if (p == string::npos) {
      nodes.push_back(new TreeNode(stoi(n_str)));
      break;
    }
    assert(p != 0);
    if (n_str == "null") {
      nodes.push_back(nullptr);
    } else {
      nodes.push_back(new TreeNode(stoi(n_str)));
    }
    tree_str = tree_str.substr(p + 1);
  }
  return unique_ptr<TreeNode>(BuildTree(nodes));
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

void Print(stack<TreeNode*>& st0) {
  auto st(st0);
  vector<TreeNode*> v;
  while (!st.empty()) {
    v.push_back(st.top());
    st.pop();
  }
  for (int i = v.size() - 1; i >= 0; i--) cout << v[i] << " ";
  cout << "\n";
}

class Solution {
 public:
  vector<int> postorderTraversal(TreeNode* root) {
    // PrintTree(root);

    stack<TreeNode*> st1;
    if (root) st1.push(root);
    stack<TreeNode*> st2;
    vector<int> r;

    while (!st1.empty()) {
      // Log << "st1=";
      // Print(st1);
      // Log << "st2=";
      // Print(st2);
      // cout << "\n";
      TreeNode* n = st1.top();
      // std::this_thread::sleep_for(std::chrono::milliseconds(200));
      if (!n->left && !n->right) {  // leaf
        r.push_back(n->val);        // eval n
        st2.push(n);
        st1.pop();
        continue;
      }
      bool visited_child = false;
      if (n->right && !st2.empty() && st2.top() == n->right) {
        // st2.top(); use if needed
        st2.pop();
        visited_child = true;
      }
      if (n->left && !st2.empty() && st2.top() == n->left) {
        // st2.top(); use if needed
        st2.pop();
        visited_child = true;
      }
      if (visited_child) {
        r.push_back(n->val);  // eval n
        st2.push(n);
        st1.pop();
        continue;
      }

      if (n->right) st1.push(n->right);
      if (n->left) st1.push(n->left);
    }

    // Log << "r=";
    // copy(r.begin(), r.end(), ostream_iterator<int>(cout, " "));
    // cout << "\n";
    return r;
  }
};

int main() {
  Solution s;
  /*
  2
    3
      1
  */
  assert(s.postorderTraversal(BuildTree("2,null,3,null,1").get()) ==
         vector<int>({1, 3, 2}));
  /*
  4
    3
      1
     2
  */
  assert(s.postorderTraversal(BuildTree("4,null,3,null,1,2").get()) ==
         vector<int>({2, 1, 3, 4}));
  Log << "done!\n";
  return 0;
}
