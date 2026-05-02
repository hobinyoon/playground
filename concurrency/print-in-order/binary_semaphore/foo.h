#pragma once

#include <functional>
#include <semaphore>

class Foo {
 public:
  Foo() = default;

  void first(std::function<void()> printFirst) {
    printFirst();
    second_ready_.release();
  }

  void second(std::function<void()> printSecond) {
    second_ready_.acquire();
    printSecond();
    third_ready_.release();
  }

  void third(std::function<void()> printThird) {
    third_ready_.acquire();
    printThird();
  }

 private:
  // Initialize with 0: threads block on acquire() until release() is called
  std::binary_semaphore second_ready_{0};
  std::binary_semaphore third_ready_{0};
};
