#include <iostream>
#include <vector>

void VectorInitWithUniquePtr() {
  std::vector<std::unique_ptr<int>> vec;
  std::generate_n(std::back_inserter(vec), 2,
                  []() { return std::make_unique<int>(2); });
  for (const auto& v : vec) {
    std::cout << &v << " " << v << " " << v.get() << " " << *v << "\n";
  }
}

void VectorInitWithSharedPtr() {
  std::vector<std::shared_ptr<int>> vec;
  std::generate_n(std::back_inserter(vec), 2, [n = 0]() mutable {
    std::cout << "[" << __LINE__ << "]\n";
    return std::make_shared<int>(++n);
  });
  std::cout << "[" << __LINE__ << "]\n";
  for (const auto& v : vec) {
    std::cout << "[" << __LINE__ << "] " << &v << " " << v << " " << v.get() << " " << *v << "\n";
  }
}

void VectorInitWithSharedPtr2() {
  std::vector<std::shared_ptr<int>> vec;
  for (int i = 0; i < 2; ++i) vec.push_back(std::make_shared<int>(2));
  for (const auto& v : vec) {
    std::cout << &v << " " << v << " " << v.get() << " " << *v << "\n";
  }
}

int main() {
  VectorInitWithUniquePtr();
  VectorInitWithSharedPtr();
  VectorInitWithSharedPtr2();

  return 0;
}
