#include <format>

#include "logger.h"

using namespace std;

struct S1 {
  int a;
  union V {
    int b;
    char c;
    string d;  // Seems like an object with a non-trivial special member
               // function is not allowed here

    // If we add d, we need provide constructor and destructor.
    V() {}
    ~V() {}

    // And copy constructor if it is used.
    V(const V& right) {}
  };
  V v;

  static S1 String(const string& s) {
    S1 s1;
    s1.a = 5;
    s1.v.d = s;
    return s1;
  }

  friend ostream& operator<<(ostream& os, const S1& s1) {
    os << format("{} {} {} {}", s1.a, s1.v.b, s1.v.c, s1.v.d);
    return os;
  }
};

void Func1() {
  S1 s1;
  s1.a = 1;
  s1.v.b = 2;
  s1.v.c = 'a';
  Log << s1;

  S1 s2;
  s2.a = 2;
  s2.v.b = 98;
  Log << s2;

  S1 s3;
  s3.a = 3;
  s3.v.d = "abc";
  Log << s3;
}

int main() {
  Func1();
  Log << "done!";
  return 0;
}
