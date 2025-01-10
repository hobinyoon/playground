#include <cxxabi.h>

#include <iostream>

#define LOG std::cout << "[" << __LINE__ << "] "

// TODO: Move the leaks directory to here

int main() {
  // Asan detected this leak on Ubuntu18, but not on MacOS 15.2.
  int* i = new int(1);
  LOG << *i << "\n";
  return 0;
}
