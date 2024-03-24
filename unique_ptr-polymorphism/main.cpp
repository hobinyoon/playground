#include <chrono>
#include <iostream>

#define LogEnabled true
#define Log          \
  if (!LogEnabled) { \
  } else             \
    std::cout << __LINE__ << " " << __FUNCTION__ << ": "

using namespace std;

struct P {
  int a;

  virtual ~P() = default;
  virtual void M1() { Log << "\n"; }
};

// Can you drop the public in struct C : public P? Yes
//
// Access specifiers: https://en.cppreference.com/w/cpp/language/access
//
// The default specifier is private for class and public for struct:
// https://stackoverflow.com/questions/4796789/default-inheritance-access-specifier
struct C : P {
  int b;

  C() { Log << "\n"; }
  ~C() { Log << "\n"; }

  void M1() { Log << "\n"; }
};

int main() {
  unique_ptr<P> p = make_unique<C>();
  p->M1();
  Log << "done!\n";
  return 0;
}
