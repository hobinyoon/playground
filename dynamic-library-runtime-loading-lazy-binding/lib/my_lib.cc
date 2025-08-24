#include <iostream>

// Function to be called from the main program
extern "C" void hello_from_lib() {
  std::cout << "Hello from the dynamically loaded library!" << std::endl;
}
