#include <iostream>
#include <vector>

using namespace std;

struct Node {
  int cost;
  vector<Node*> children;
  ~Node() {
    for (Node* c : children) delete c;
  }
};

void Visit(Node* n, int cost, int& min_cost) {
  cost += n->cost;

  if (n->children.empty()) min_cost = min(min_cost, cost);

  for (Node* c : n->children) {
    Visit(c, cost, min_cost);
  }
}

int getCheapestCost(Node* rootNode) {
  if (!rootNode) return 0;
  int min_cost = INT_MAX;
  Visit(rootNode, 0, min_cost);
  return min_cost;
}

int main() {
#define NN new Node
  unique_ptr<Node> root(
      NN{0,
         {
             NN{5, {NN{4}}},
             NN{3, {NN{2, {NN{1, {NN{1}}}}}, NN{0, {NN{10}}}}},
             NN{6, {NN{1}, NN{5}}},
         }});
#undef NN

  int r = getCheapestCost(root.get());
  cout << r << "\n";
  return 0;
}
