#include <sstream>

#include "logger.h"

using namespace std;

int main() {
  // Ignores the leading and trailing spaces, and treats multiple spaces as one.
  istringstream in(" aa bb  cc dd  ");
  string word;
  while (in >> word) {
    Log << "[" << word << "]";
  }
  Log << "done!";
  return 0;
}
