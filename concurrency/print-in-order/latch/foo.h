#pragma once

#include <functional>
#include <latch>

class Foo {
 public:
  Foo() = default;

  void first(std::function<void()> printFirst) {
    printFirst();
    latch_second_ready_.count_down();
  }

  void second(std::function<void()> printSecond) {
    latch_second_ready_.wait();
    printSecond();
    latch_third_ready_.count_down();
  }

  void third(std::function<void()> printThird) {
    latch_third_ready_.wait();
    printThird();
  }

 private:
  // Latches initialized with count=1 (single-use countdown synchronization)
  std::latch latch_second_ready_{1};
  std::latch latch_third_ready_{1};
};
