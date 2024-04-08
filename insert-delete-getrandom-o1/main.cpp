#include <cassert>
#include <format>
#include <map>
#include <vector>

#include "logger.h"

using namespace std;

class RandomizedSet {
  //            num  index_in_vector
  unordered_map<int, int> m;
  //     num
  vector<int> v;

 public:
  bool insert(int val) {
    if (m.contains(val)) return false;
    v.push_back(val);
    m[val] = v.size() - 1;
    return true;
  }

  bool remove(int val) {
    if (!m.contains(val)) return false;

    // Move the last item in v to m[val]
    v[m[val]] = v.back();
    m[v.back()] = m[val];
    v.pop_back();

    m.erase(val);
    return true;
  }

  int getRandom() { return v[rand() % v.size()]; }

  string state() {
    ostringstream ss;
    ss << "v=[";
    copy(v.begin(), v.end(), ostream_iterator<int>(ss, " "));
    ss << "] m={";
    for (const auto& [num, idx] : m) ss << format("{}:{} ", num, idx);
    ss << "}";
    return ss.str();
  }
};

int main() {
  RandomizedSet s;
  assert(s.insert(1) == true);
  assert(s.remove(2) == false);
  assert(s.insert(2) == true);
  // Log << s.state();
  int a = s.getRandom();
  assert(a == 1 || a == 2);
  assert(s.remove(1) == true);
  assert(s.insert(2) == false);
  assert(s.getRandom() == 2);
  // Log << s.state();
  Log << "done!";
  return 0;
}
