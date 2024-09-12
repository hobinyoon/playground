#include <iostream>
#include <string>
#include <vector>

#include "absl/strings/str_format.h"

template <typename... Args, std::size_t... I>
void print_tuple_imp(const std::tuple<Args...>& tup,
                     std::index_sequence<I...>) {
  std::cout << "[" << __LINE__ << "]\n";
  // print_all(std::get<I>(tup)...);
  std::cout << absl::StrFormat("%v-%v-%v", std::get<I>(tup)...) << "\n";
  std::cout << "[" << __LINE__ << "]\n";
}

template <typename... Args>
void print_tuple(const std::tuple<Args...>& tup) {
  std::cout << "[" << __LINE__ << "]\n";
  print_tuple_imp(tup, std::index_sequence_for<Args...>());
}

void UnpackTuple() {
  std::cout << "[" << __LINE__ << "]\n";
  print_tuple(std::make_tuple(1, 3.14, std::string("abc")));
  std::cout << "[" << __LINE__ << "]\n";
}

// template<typename T>
// void print_vector(const std::vector<T>& v){
//   std::cout << "[" << __LINE__ << "]\n";
//   std::cout << std::index_sequence_for<T>() << "\n";
//   // print_vector_imp(v, std::index_sequence_for<T>(v));
// }

template <std::size_t... ParamsIdx>
auto funcPointer(std::index_sequence<ParamsIdx...>) {
  std::cout << "[" << __LINE__ << "]\n";
  // std::cout << absl::StrFormat("%v-%v", std::get<I>()...) << "\n";
  return 0;
  // return [](std::vector<std::string> const &args) {
  //     assert(args.size() == sizeof...(ParamsIdx));
  //     return f(args[ParamsIdx]...);
  // };
}

template <typename... Params>
auto funcPointer(Params...) {
  std::cout << "[" << __LINE__ << "]\n";
  return funcPointer(std::index_sequence_for<Params...>{});
}

void UnpackVector() {
  std::cout << "[" << __LINE__ << "]\n";
  funcPointer(std::vector<int>{1, 2});
  std::cout << "[" << __LINE__ << "]\n";
}

int main() {
  UnpackTuple();
  std::cout << "\n";

  // TODO: Getting close ...
  UnpackVector();

  return 0;
}
