#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>

class Foo {
 public:
  Foo() = default;

  void first(std::function<void()> printFirst) {
    // printFirst() outputs "first". Do not change or remove this line.
    printFirst();

    {
      std::lock_guard<std::mutex> lock(next_mutex_);
      next_ = 2;
    }
    next_cv_.notify_all();
  }

  void second(std::function<void()> printSecond) {
    {
      std::unique_lock<std::mutex> lock(next_mutex_);
      next_cv_.wait(lock, [this] { return next_ == 2; });
    }

    // printSecond() outputs "second". Do not change or remove this line.
    printSecond();

    {
      std::lock_guard<std::mutex> lock(next_mutex_);
      next_ = 3;
    }
    next_cv_.notify_all();
  }

  void third(std::function<void()> printThird) {
    {
      std::unique_lock<std::mutex> lock(next_mutex_);
      next_cv_.wait(lock, [this] { return next_ == 3; });
    }

    // printThird() outputs "third". Do not change or remove this line.
    printThird();
  }

 private:
  std::mutex next_mutex_;
  std::condition_variable next_cv_;
  int next_ = 0;
};
