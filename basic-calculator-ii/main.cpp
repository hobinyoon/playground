#include <cassert>
#include <iostream>
#include <stack>
#include <vector>

#define LogEnabled true
#define Log          \
  if (!LogEnabled) { \
  } else             \
    std::cout << __LINE__ << " " << __FUNCTION__ << ": "

using namespace std;

class Solution {
 public:
  int calculate(string s) {
    // Rewrite without Token
    int n1 = 0;
    int n2 = 0;
    char last_op = '+';

    int num = 0;
    for (int i = 0; i < s.size(); i++) {
      char c = s[i];
      if (isdigit(c)) num = num * 10 + (c - '0');
      if (c == '+' or c == '-' or c == '*' or c == '/' or i == s.size() - 1) {
        if (last_op == '+' or last_op == '-') {
          n1 += n2;
          n2 = num * (last_op == '+' ? 1 : -1);
        } else {
          if (last_op == '*')
            n2 *= num;
          else
            n2 /= num;
        }
        num = 0;
        last_op = c;
      }
      // Ignore other chars including ' '
    }
    return n1 + n2;
  }
};

class Solution6 {
 public:
  int calculate(string s) {
    // Rewrite without Token
    int n1 = 0;
    int n2 = 0;
    char last_op = '+';

    int num = 0;
    for (char c : s) {
      if (c == ' ') continue;
      if (isdigit(c))
        num = num * 10 + (c - '0');
      else {  // c is an op
        if (last_op == '+' || last_op == '-') {
          n1 += n2;
          n2 = num * (last_op == '+' ? 1 : -1);
        } else {
          if (last_op == '*')
            n2 *= num;
          else
            n2 /= num;
        }
        num = 0;
        last_op = c;
      }
    }

    if (last_op == '+' || last_op == '-') {
      n1 += n2;
      n2 = num * (last_op == '+' ? 1 : -1);
    } else {
      if (last_op == '*')
        n2 *= num;
      else
        n2 /= num;
    }

    return n1 + n2;
  }
};

class Solution5 {
 public:
  int calculate(string s) {
    struct Token {
      char type;  // 'n': number, 'o': operator
      int num;
      char op;  // +, -, *, /
    };

    vector<Token> tokens;
    int n = 0;
    for (char c : s) {
      if (c == ' ') continue;
      if (isdigit(c))
        n = n * 10 + (c - '0');
      else {  // c is an op
        tokens.emplace_back('n', n, ' ');
        n = 0;
        tokens.emplace_back('o', 0, c);
      }
    }
    tokens.emplace_back('n', n, ' ');

    // Rewrite without stack since the stack grows only up to 2
    int n1 = 0;
    int n2 = 0;
    char last_op = '+';
    for (const Token& t : tokens) {
      if (t.type == 'n') {
        if (last_op == '+' || last_op == '-') {
          n1 += n2;
          n2 = t.num * (last_op == '+' ? 1 : -1);
        } else {
          if (last_op == '*')
            n2 *= t.num;
          else
            n2 /= t.num;
        }
      } else
        last_op = t.op;
    }

    return n1 + n2;
  }
};

class Solution4 {
 public:
  int calculate(string s) {
    struct Token {
      char type;  // 'n': number, 'o': operator
      int num;
      char op;  // +, -, *, /
    };

    vector<Token> tokens;
    int n = 0;
    for (char c : s) {
      if (c == ' ') continue;
      if (isdigit(c))
        n = n * 10 + (c - '0');
      else {  // c is an op
        tokens.emplace_back('n', n, ' ');
        n = 0;
        tokens.emplace_back('o', 0, c);
      }
    }
    tokens.emplace_back('n', n, ' ');

    stack<int> st;
    char last_op = '+';
    for (const Token& t : tokens) {
      if (t.type == 'n') {
        if (last_op == '+' || last_op == '-') {
          // Evaluate the last 2 items. Now the stack grows only up to 2!
          if (st.size() == 2) {
            int n2 = st.top();
            st.pop();
            int n1 = st.top();
            st.pop();
            st.push(n1 + n2);
          }
          st.push(t.num * (last_op == '+' ? 1 : -1));
        } else {
          int n1 = st.top();
          st.pop();
          if (last_op == '*')
            st.push(n1 * t.num);
          else
            st.push(n1 / t.num);
        }
      } else
        last_op = t.op;
    }

    assert(st.size() <= 2);
    int r = 0;
    while (!st.empty()) {
      r += st.top();
      st.pop();
    }
    return r;
  }
};

class Solution3 {
 public:
  int calculate(string s) {
    struct Token {
      char type;  // 'n': number, 'o': operator
      int num;
      char op;  // +, -, *, /
    };

    vector<Token> tokens;
    int n = 0;
    for (char c : s) {
      if (c == ' ') continue;
      if (isdigit(c))
        n = n * 10 + (c - '0');
      else {  // c is an op
        tokens.emplace_back('n', n, ' ');
        n = 0;
        tokens.emplace_back('o', 0, c);
      }
    }
    tokens.emplace_back('n', n, ' ');

    stack<int> st;
    char last_op = '+';
    for (const Token& t : tokens) {
      if (t.type == 'n') {
        if (last_op == '+') {
          // More optimization opportunity here
          st.push(t.num);
        } else if (last_op == '-') {
          st.push(t.num * -1);
        } else if (last_op == '*') {
          int n1 = st.top();
          st.pop();
          st.push(n1 * t.num);
        } else {
          int n1 = st.top();
          st.pop();
          st.push(n1 / t.num);
        }
      } else
        last_op = t.op;
    }

    int r = 0;
    while (!st.empty()) {
      r += st.top();
      st.pop();
    }
    return r;
  }
};

class Solution2 {
 public:
  int calculate(string s) {
    struct Token {
      char type;  // 'n': number, 'o': operator
      int num;
      char op;  // +, -, *, /
    };

    vector<Token> tokens;
    int n = 0;
    for (char c : s) {
      if (c == ' ') continue;
      if (isdigit(c))
        n = n * 10 + (c - '0');
      else {  // c is an op
        tokens.emplace_back('n', n, ' ');
        n = 0;
        tokens.emplace_back('o', 0, c);
      }
    }
    tokens.emplace_back('n', n, ' ');

    stack<Token> st;
    for (const Token& t : tokens) {
      if (t.type == 'n') {
        if (st.empty()) {
          st.push(t);
          continue;
        }
        char last_op = st.top().op;
        if (last_op == '*' || last_op == '/') {
          st.pop();
          int n1 = st.top().num;
          st.pop();
          if (last_op == '*')
            st.emplace('n', n1 * t.num, ' ');
          else
            st.emplace('n', n1 / t.num, ' ');
        } else {
          st.push(t);
          // More space optimization opportunity here by evaluating the last 2
          // numbers in the stack
        }
      } else
        st.push(t);
    }

    // Reverse the stack
    stack<Token> st1;
    while (!st.empty()) {
      st1.push(st.top());
      st.pop();
    }

    int r = 0;
    int op = 1;  // 1: +, -1: -
    while (!st1.empty()) {
      const Token& t = st1.top();
      if (t.type == 'n')
        r += op * t.num;
      else
        op = t.op == '+' ? 1 : -1;
      st1.pop();
    }
    return r;
  }
};

int main() {
  assert(Solution().calculate(" 3+5 / 2 ") == 5);
  assert(Solution().calculate("2147483647") == 2147483647);

  Log << "done!\n";
  return 0;
}
