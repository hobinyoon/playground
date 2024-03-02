/*
Not sure how this can be done in DP. Probably not.

BFS:
  Time: V + E -> n + n^2 (finding an edge takes up to n for each node) -> n^2
  Space: n (queue + set = n)

DFS:
  Time: V + E -> n + n^2 -> n^2. It feels a lot less efficient. The search space
  expansion is not as efficient. We could visit the same node in multiple
  different ways, thus cache stores all possible paths.
  Space: n for the call stack. Cache could store n! different paths.
*/

#include <cassert>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <vector>

using namespace std;

#define Log std::cout << __LINE__ << " " << __FUNCTION__ << ": "

class DFS {
  //   s,      words            min_steps
  map<pair<string, set<string>>, int> cache;

  // Assume a != b
  bool OffByOne(const string& a, const string& b) {
    if (a.size() != b.size()) return false;

    int dist = 0;
    for (int i = 0; i < a.size(); i++) {
      if (a[i] != b[i]) dist++;
      if (dist > 1) return false;
    }
    return true;
  }

  int MinSteps(int n, const string& s, const string& t, set<string>& words) {
    Log << "n=" << n << " s=" << s << " t=" << t << " words=[";
    copy(words.begin(), words.end(), ostream_iterator<string>(cout, " "));
    cout << "]\n";

    if (s == t) return n;

    auto it = cache.find(make_pair(s, words));
    if (it != cache.end()) return it->second;

    int min_steps = INT_MAX;
    for (const string& w : words) {
      Log << "w=" << w << "\n";
      if (!OffByOne(s, w)) continue;
      Log << "\n";
      set<string> words1 = words;
      words1.erase(w);
      int steps = MinSteps(n + 1, w, t, words1);
      min_steps = min(min_steps, steps);
    }

    cache[make_pair(s, words)] = min_steps;
    Log << "min_steps=" << min_steps << "\n";
    return min_steps;
  }

 public:
  int shortestWordEditPath(const string source, const string target,
                           const vector<string> words) {
    cache.clear();
    set<string> words1(words.begin(), words.end());
    words1.erase(source);
    int r = MinSteps(0, source, target, words1);
    if (r == INT_MAX) r = -1;
    Log << "r=" << r << "\n";
    return r;
  }
};

class BFS {
  // Assume a != b
  bool OffByOne(const string& a, const string& b) {
    if (a.size() != b.size()) return false;

    int dist = 0;
    for (int i = 0; i < a.size(); i++) {
      if (a[i] != b[i]) dist++;
      if (dist > 1) return false;
    }
    return true;
  }

 public:
  int shortestWordEditPath(const string& source, const string& target,
                           const vector<string>& words0) {
    list<string> words(words0.begin(), words0.end());
    //         word    dist
    queue<pair<string, int>> q;
    q.push({source, 0});

    while (!q.empty()) {
      auto [w, dist] = q.front();
      q.pop();

      for (auto it = words.begin(); it != words.end();) {
        const string& w1 = *it;
        if (!OffByOne(w, w1)) {
          it++;
          continue;
        }
        if (w1 == target) {
          return dist + 1;
        }
        q.push({w1, dist + 1});
        it = words.erase(it);
      }
    }
    return -1;
  }
};

int main() {
  assert(BFS().shortestWordEditPath(
             "bit", "dog", {"but", "put", "big", "pot", "pog", "dog", "lot"}) ==
         5);
  assert(BFS().shortestWordEditPath("abc", "ab", {"abc", "ab"}) == -1);
  Log << "done!\n";
  return 0;
}
