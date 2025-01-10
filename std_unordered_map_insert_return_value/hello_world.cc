#include <cxxabi.h>

#include <iostream>
#include <stdexcept>
#include <string_view>
#include <unordered_map>

#define LOG std::cout << "[" << __LINE__ << "] "

std::string Demangle(std::string_view type_name) {
  int status;
  char* demangled_name = abi::__cxa_demangle(
      // We assume type_name is null terminated.
      type_name.data(), nullptr, nullptr, &status);
  std::string r;
  if (status == 0) {
    r = demangled_name;
    free(demangled_name);
  } else {
    throw std::runtime_error("Demangling failed.");
  }
  return r;
}

int main() {
  std::unordered_map<int, int> map;
  auto r = map.insert(std::make_pair(1, 2));
  LOG << "inserted=" << r.second << "\n";

  LOG << typeid(r.first).name() << "\n";
  LOG << Demangle(typeid(r.first).name()) << "\n";

  // The same pair, thus not inserted.
  r = map.insert(std::make_pair(1, 2));
  LOG << "inserted=" << r.second << "\n";

  // Value is updated, thus not inserted.
  r = map.insert(std::make_pair(1, 3));
  LOG << "inserted=" << r.second << "\n";

  // New key, thus inserted.
  r = map.insert(std::make_pair(2, 3));
  LOG << "inserted=" << r.second << "\n";

  return 0;
}
