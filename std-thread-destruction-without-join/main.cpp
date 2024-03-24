#include <chrono>
#include <iostream>
#include <map>
#include <thread>

#define LogEnabled true
#define Log          \
  if (!LogEnabled) { \
  } else             \
    std::cout << __LINE__ << " " << __FUNCTION__ << ": "

using namespace std;

void ThreadWorker() { Log << "\n"; }

int main() {
  thread t1(ThreadWorker);
  t1.join();
  // Without join(), we get:
  //   libc++abi: terminating
  //   Abort trap: 6

  // Passing a function name vs function address? Seems like they are the same.
  // https://stackoverflow.com/questions/59987314/pass-a-function-or-pass-a-function-pointer-in-c

  // Not sure if it's true with C++ though.
  // Is passing a function name gets translated to passing an address? Or would
  // it generate a function<> object? Let's do the compiler explorer.
  //   Seems like the former passes a reference and the latter passes a pointer:
  //   https://godbolt.org/z/5795zo1Ee
  thread t2(&ThreadWorker);
  t2.join();

  thread t3([]() { Log << "\n"; });
  t3.join();

  vector<thread> t4;
  for (int i = 0; i < 2; i++) t4.push_back(thread([]() { Log << "\n"; }));
  for (thread& t : t4) t.join();

  // vector<thread> t5(2, thread([]() { Log << "\n"; }));
  // error: no matching function for call to 'construct_at'
  // I think this doesn't work because thread is not copyable.

  // This doesn't work either probably because we can't move the same object
  // multiple times
  // vector<thread> t5(2, std::move(thread([]() { Log << "\n";})));

  Log << "done!\n";
  return 0;
}
