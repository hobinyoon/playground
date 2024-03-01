#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

#define Log std::cout << __LINE__ << " " << __PRETTY_FUNCTION__ << ": "

struct S1 {
  int a;
  // This is optional
  // S1(int a) : a(a) {
  //   Log << "\n";
  // }
};

ostream& operator<<(ostream& os, const S1& s1) {
  os << s1.a;
  return os;
}

class C1 {
  int a;

 public:
  C1(int a)
      : a(a)  // Side note, some people seem to prefer this style:
              // https://stackoverflow.com/questions/268587/can-i-use-identical-names-for-fields-and-constructor-parameters
  {
    Log << "a=" << this->a << "\n";
  }
};

int main() {
  S1 s1{1};  // The constructor seems to be optional. Compiler generates one for
             // you.

  C1 c1{1};  // Initializer list. Calls the constructor above. Not sure why this
             // is not optional.
  C1 c2 = C1{2};  // Copy elison

  Log << s1 << "\n";
  Log << ": done!\n";
  return 0;
}
