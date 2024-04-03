#include "logger.h"

using namespace std;

struct S1 {
  int a;

  friend ostream& operator<<(ostream& os, const S1& s1) {
    os << s1.a;
    return os;
  }
};

void Func1() {
  // friend function can not be defined in a local class
  //
  // struct S1 {
  //   int a;
  //   friend ostream& operator<<(ostream& os, const S1& s1) {
  //     os << s1.a;
  //     return os;
  //   }
  // };

  S1 s1;
  Log << s1;
}

int main() {
  Func1();
  Log << "done!";
  return 0;
}
