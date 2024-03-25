#include <iostream>
#include <map>
#include <queue>
#include <unordered_set>

#define LogEnabled true
#define Log          \
  if (!LogEnabled) { \
  } else             \
    std::cout << __LINE__ << " " << __FUNCTION__ << ": "

using namespace std;

class Node {
 public:
  int val;
  vector<Node*> neighbors;
  Node() {
    val = 0;
    neighbors = vector<Node*>();
  }
  Node(int _val) {
    val = _val;
    neighbors = vector<Node*>();
  }
  Node(int _val, vector<Node*> _neighbors) {
    val = _val;
    neighbors = _neighbors;
  }
};

// Also we can fix up the links in a separate step:
// https://leetcode.com/problems/clone-graph/submissions/1201827873

// BFS
class Solution {
  unordered_map<Node*, Node*> in_clone;

 public:
  Node* cloneGraph(Node* in) {
    if (!in) return nullptr;

    Node* new_root = new Node(in->val);
    unordered_map<Node*, Node*> in_clone;
    in_clone[in] = new_root;

    queue<Node*> q;
    q.push(in);
    while (!q.empty()) {
      Node* n = q.front();
      q.pop();
      Node* n_clone = in_clone[n];  // guaranteed to exist

      for (Node* nei : n->neighbors) {
        auto it = in_clone.find(nei);
        if (it != in_clone.end())
          n_clone->neighbors.push_back(it->second);
        else {
          Node* nei_clone = new Node(nei->val);
          n_clone->neighbors.push_back(nei_clone);
          in_clone[nei] = nei_clone;
          q.push(nei);
        }
      }
    }
    return new_root;
  }
};

// DFS with a call stack
class Solution2 {
  unordered_map<Node*, Node*> in_clone;

  Node* Clone(Node* in) {
    if (!in) return nullptr;

    if (in_clone.contains(in)) return in_clone[in];

    Node* out = new Node(in->val);
    in_clone[in] = out;

    for (Node* n : in->neighbors) out->neighbors.push_back(Clone(n));

    return out;
  }

 public:
  Node* cloneGraph(Node* in) {
    in_clone.clear();
    return Clone(in);
  }
};

Node* BuildGraph(const vector<vector<int>>& adj) {
  unordered_map<int, Node*> nodeid_node;

  auto GetNode = [&nodeid_node](int node_id) {
    auto it = nodeid_node.find(node_id);
    if (it != nodeid_node.end()) return it->second;
    Node* n = new Node(node_id);
    nodeid_node[node_id] = n;
    return n;
  };

  for (int i = 0; i < adj.size(); i++) {
    Node* n = GetNode(i + 1);
    for (int neighbor_id : adj[i]) n->neighbors.push_back(GetNode(neighbor_id));
  }
  return GetNode(1);
}

void DeleteGraph(Node* n) {
  // Build a list of nodes to delete and delete
  unordered_set<Node*> nodes;

  queue<Node*> q;
  if (n) {
    nodes.insert(n);
    q.push(n);
  }
  while (!q.empty()) {
    Node* n1 = q.front();
    q.pop();

    for (Node* nei : n1->neighbors) {
      if (nodes.contains(nei)) continue;
      nodes.insert(nei);
      q.push(nei);
    }
  }

  for (Node* n : nodes) delete n;
}

int main() {
  Node* graph = BuildGraph({{2, 4}, {1, 3}, {2, 4}, {1, 3}});
  Node* cloned_graph = Solution().cloneGraph(graph);

  DeleteGraph(graph);
  DeleteGraph(cloned_graph);

  Log << "done!\n";
  return 0;
}
