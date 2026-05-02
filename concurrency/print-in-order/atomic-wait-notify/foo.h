#pragma once

#include <atomic>
#include <functional>

class Foo {
 public:
  Foo() = default;

  void first(std::function<void()> printFirst) {
    printFirst();

    // Release store synchronizes with acquire wait in second()
    second_ready_.store(true, std::memory_order_release);
    second_ready_.notify_one();
  }

  void second(std::function<void()> printSecond) {
    // Acquire wait synchronizes with release store in first()
    second_ready_.wait(false, std::memory_order_acquire);

    printSecond();

    third_ready_.store(true, std::memory_order_release);
    third_ready_.notify_one();
  }

  void third(std::function<void()> printThird) {
    third_ready_.wait(false, std::memory_order_acquire);

    printThird();
  }

 private:
  std::atomic<bool> second_ready_{false};
  std::atomic<bool> third_ready_{false};
};
