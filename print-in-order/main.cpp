#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <numeric>
#include <random>
#include <thread>
#include <vector>

#define LogEnabled true
#define Log          \
  if (!LogEnabled) { \
  } else             \
    std::cout << __LINE__ << " " << __FUNCTION__ << ": "

using namespace std;

class Foo {
  condition_variable cv2;
  condition_variable cv3;
  int turn = 1;
  mutex m;
  bool sync = true;

 public:
  void first(function<void()> printFirst) {
    printFirst();

    if (sync) {
      {
        // Lock vs atomic variable? It has to be a lock. Atomic veriable
        // wouldn't have helped.
        // https://stackoverflow.com/questions/36126286/using-stdcondition-variable-with-atomicbool
        std::unique_lock lk(m);
        turn = 2;
      }
      cv2.notify_one();
    }
  }

  void second(function<void()> printSecond) {
    if (sync) {
      std::unique_lock lk(m);
      cv2.wait(lk, [&turn = turn] { return turn == 2; });
    }

    printSecond();

    if (sync) {
      {
        std::unique_lock lk(m);
        turn = 3;
      }
      cv3.notify_one();
    }
  }

  void third(function<void()> printThird) {
    if (sync) {
      std::unique_lock lk(m);
      cv3.wait(lk, [&turn = turn] { return turn == 3; });
    }

    printThird();
  }
};

void ThreadWorker(Foo& foo, int worker_id) {
  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_int_distribution<int> distribution(0, 100);
  int us = distribution(generator);
  std::this_thread::sleep_for(std::chrono::microseconds(us));
  switch (worker_id) {
    case 1:
      foo.first([]() { cout << "1"; });
      break;
    case 2:
      foo.second([]() { cout << "2"; });
      break;
    case 3:
      foo.third([]() { cout << "3"; });
      break;
  }
}

int main() {
  vector<int> order(3);
  iota(order.begin(), order.end(), 1);

  std::random_device rd;
  std::mt19937 generator(rd());

  for (int i = 0; i < 100; i++) {
    shuffle(order.begin(), order.end(), generator);
    // copy(order.begin(), order.end(), ostream_iterator<int>(cout, " "));
    // cout << "\n";

    Foo foo;
    thread t1(ThreadWorker, ref(foo), 1);
    thread t2(ThreadWorker, ref(foo), 2);
    thread t3(ThreadWorker, ref(foo), 3);
    t1.join();
    t2.join();
    t3.join();
    cout << "\n";
  }

  Log << "done!\n";
  return 0;
}
