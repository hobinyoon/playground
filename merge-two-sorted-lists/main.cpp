#include <iostream>

using namespace std;

struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Recursive
class Solution {
public:
ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
  if (!list1)
    return list2;
  if (!list2)
    return list1;
  // If both are nullptr? Should work.

  if (list1->val <= list2->val) {
    list1->next = mergeTwoLists(list1->next, list2);
    return list1;
  } else {
    list2->next = mergeTwoLists(list1, list2->next);
    return list2;
  }
}
};

// Iterative
class Solution2 {
public:
ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
  if (!list1)
    return list2;
  if (!list2)
    return list1;

  // head and current
  ListNode* h = nullptr;
  ListNode* c = h;

  while (true) {
    if (list1->val <= list2->val) {
      if (!h) {
        h = list1;
        c = h;
      } else {
        c->next = list1;
        c = list1;
      }
			list1 = list1->next;
      if (!c->next) {
        c->next = list2;
        break;
      }
    } else {
      if (!h) {
        h = list2;
        c = h;
      } else {
        c->next = list2;
        c = list2;
      }
			list2 = list2->next;
      if (!c->next) {
        c->next = list1;
        break;
      }
    }
  }
  return h;
}
};

void Print(ListNode* list1) {
  cout << __LINE__ << ":";
  for (ListNode* l = list1; l; l=l->next) {
    cout << " " << l->val;
  }
  cout << "\n";
}

int main()
{
  ListNode* list1 = new ListNode(1, new ListNode(2, new ListNode(4)));
  ListNode* list2 = new ListNode(1, new ListNode(3, new ListNode(4)));
  Print(list1);
  Print(list2);

  Solution s;
  ListNode* r = s.mergeTwoLists(list1, list2);
  Print(r);

  std::cout<<"Hello World";
  return 0;
}
