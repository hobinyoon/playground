#include <iostream>

using namespace std;

void A() { int* i = new int; }

int main(int argc, char** argv) {
  // Interesting that leaks didn't detect the leak here
  int* i = new int;

  // It detected the leak here
  A();

  system("leaks hello-world");
  return 0;
}
