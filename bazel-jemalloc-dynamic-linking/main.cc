#include <iostream>
#include <memory>

int main() {

  {
    auto i = std::make_unique<int>(0);
  }

  std::cout << "Now you can inspect the memory mep. When you are done, press Enter to continue..." << std::endl;
  std::cin.get();

  return 0;
}
