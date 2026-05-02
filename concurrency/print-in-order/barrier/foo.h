#pragma once

#include <barrier>
#include <functional>

class Foo {
 public:
  Foo() = default;

  void first(std::function<void()> printFirst) {
    printFirst();
    barrier_second_ready_.arrive_and_wait();
  }

  void second(std::function<void()> printSecond) {
    barrier_second_ready_.arrive_and_wait();
    printSecond();
    barrier_third_ready_.arrive_and_wait();
  }

  void third(std::function<void()> printThird) {
    barrier_third_ready_.arrive_and_wait();
    printThird();
  }

 private:
  // Barriers initialized with count=2 (2 threads must arrive before proceeding)
  std::barrier<> barrier_second_ready_{2};
  std::barrier<> barrier_third_ready_{2};
};
