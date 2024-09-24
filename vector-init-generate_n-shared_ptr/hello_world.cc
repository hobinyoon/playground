#include <iostream>
#include <vector>

#define LOG std::cout << "[" << __LINE__ << "] "

void VectorInitWithUniquePtr() {
  std::vector<std::unique_ptr<int>> vec;
  std::generate_n(std::back_inserter(vec), 2,
                  []() { return std::make_unique<int>(2); });
  for (const auto& v : vec) {
    LOG << &v << " " << v << " " << v.get() << " " << *v << "\n";
  }
}

void VectorInitWithSharedPtr() {
  std::vector<std::shared_ptr<int>> vec;
  std::generate_n(std::back_inserter(vec), 2, [n = 0]() mutable {
    LOG << "\n";
    return std::make_shared<int>(++n);
  });
  for (const auto& v : vec) {
    LOG << &v << " " << v << " " << v.get() << " " << *v << "\n";
  }
}

void VectorInitWithSharedPtr2() {
  std::vector<std::shared_ptr<int>> vec;
  for (int i = 0; i < 2; ++i) vec.push_back(std::make_shared<int>(2));
  for (const auto& v : vec) {
    LOG << &v << " " << v << " " << v.get() << " " << *v << "\n";
  }
}

// Seems like you can't initialize arr. It is nullptr by default.
void VectorInitWithSharedPtr3() {
  std::array<std::shared_ptr<int>, 2> arr;
  for (const auto& v : arr) {
    LOG << &v << " " << v << " " << v.get() << "\n";
  }
}

// You end up having 2 shared_ptrs pointing the same object
void VectorInitWithSharedPtr4() {
  static int a = 100;
  std::vector<std::shared_ptr<int>> vec(2, std::make_shared<int>(++a));
  for (const auto& v : vec) {
    LOG << &v << " " << v << " " << v.get() << " " << *v << "\n";
  }
}

void ArrayInitWithSharedPtr() {
  std::array<std::shared_ptr<int>, 2> arr;
  std::generate_n(arr.begin(), 2, [n = 0]() mutable {
    LOG << "\n";
    return std::make_shared<int>(++n);
  });
  for (const auto& a : arr) {
    LOG << &a << " " << a << " " << a.get() << " " << *a << "\n";
  }
}

int main() {
  VectorInitWithUniquePtr();
  VectorInitWithSharedPtr();
  VectorInitWithSharedPtr2();
  VectorInitWithSharedPtr3();
  VectorInitWithSharedPtr4();
  ArrayInitWithSharedPtr();
  return 0;
}
