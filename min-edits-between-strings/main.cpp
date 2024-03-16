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

vector<string> diffBetweenTwoStrings(const string& source,
                                     const string& target) {
  int ss = source.size();
  int ts = target.size();
  Log << "ss=" << ss << " ts=" << ts << "\n";

  // [si][ti]
  vector<vector<int>> mds(ss + 1, vector<int>(ts + 1));
  // Init the right and bottom bounds
  for (int s = 0; s <= ss; s++) mds[s][ts] = ss - s;
  for (int t = 0; t <= ts; t++) mds[ss][t] = ts - t;
  for (int s = 0; s <= ss; s++) {
    for (int t = 0; t <= ts; t++) {
      cout << mds[s][t] << " ";
    }
    cout << "\n";
  }
  Log << "\n";

  for (int s = ss - 1; s >= 0; s--)
    for (int t = ts - 1; t >= 0; t--) {
      if (source[s] == target[t])
        mds[s][t] = mds[s + 1][t + 1];
      else
        mds[s][t] = min(1 + mds[s + 1][t], 1 + mds[s][t + 1]);
    }
  for (int s = 0; s <= ss; s++) {
    for (int t = 0; t <= ts; t++) {
      cout << mds[s][t] << " ";
    }
    cout << "\n";
  }

  // Follow the mds map from [0][0] to [ss][ts]
  vector<string> edits;
  int s = 0;
  int t = 0;
  while (s < ss && t < ts) {
    Log << "s=" << s << " t=" << t << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    if (source[s] == target[t]) {
      Log << "mds[s][t] == mds[s + 1][t + 1]\n";
      edits.push_back({source[s]});
      s++;
      t++;
    } else if (mds[s][t] == mds[s + 1][t] + 1) {
      Log << "mds[s][t] == mds[s + 1][t] + 1\n";
      edits.push_back(string("-") + source[s]);
      s++;
    } else if (mds[s][t] == mds[s][t + 1] + 1) {
      Log << "mds[s][t] == mds[s][t + 1] + 1\n";
      edits.push_back(string("+") + target[t]);
      t++;
    }
  }
  Log << "\n";

  // Now s == ss || t == ts
  for (; s < ss; s++) edits.push_back(string("-") + source[s]);
  for (; t < ts; t++) edits.push_back(string("+") + target[t]);

  Log << "edits=[";
  copy(edits.begin(), edits.end(), ostream_iterator<string>(cout, " "));
  cout << "]\n";

  // Log << edits << "\n";
  return edits;
}

int main() {
  /*
      A B D F F G H
    4 5 6 7 6 7 8 7   A
  A 5 4 5 6 5 6 7 6   B
  B 6 5 4 5 4 5 6 5   C
  C 5 4 3 4 3 4 5 4
          ^
  D 6 5 4 3 2 3 4 3
  E 5 4 3 2 1 2 3 2
  F 6 5 4 3 2 1 2 1
  G 7 6 5 4 3 2 1 0
  */
  assert(diffBetweenTwoStrings("ABCDEFG", "ABDFFGH") ==
         vector<string>({"A", "B", "-C", "D", "-E", "F", "+F", "G", "+H"}));
  Log << "done!\n";
  return 0;
}
