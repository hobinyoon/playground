#include <iostream>
#include <dlfcn.h>

int main() {
  // Use a hardcoded path. In a real-world scenario, you might use a more robust
  // method to find the library.
  void* handle = dlopen("main/lib/libmy_lib.so", RTLD_LAZY);
  if (!handle) {
    std::cerr << "Cannot open library: " << dlerror() << '\n';
    return 1;
  }

  // Get a function pointer to the hello_from_lib function
  typedef void (*hello_t)();
  hello_t hello = (hello_t)dlsym(handle, "hello_from_lib");
  // Misc:
  //   The way dlsym() errors are fetched from dlerror() is weird.

  const char* dlsym_error = dlerror();
  if (dlsym_error) {
    std::cerr << "Cannot load symbol 'hello_from_lib': " << dlsym_error << '\n';
    dlclose(handle);
    return 1;
  }

  // Call the function from the shared library
  hello();

  // Close the library handle
  dlclose(handle);

  return 0;
}
