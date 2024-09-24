#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "absl/strings/str_format.h"

namespace Tuple {

template <typename... Args, std::size_t... I>
void print_tuple_imp(const std::tuple<Args...>& tup,
                     std::index_sequence<I...>) {
  // print_all(std::get<I>(tup)...);
  std::cout << absl::StrFormat("%v-%v-%v", std::get<I>(tup)...) << "\n";
}

template <typename... Args>
void print_tuple(const std::tuple<Args...>& tup) {
  print_tuple_imp(tup, std::index_sequence_for<Args...>());
}

void Unpack() {
  print_tuple(std::make_tuple(1, 3.14, std::string("abc")));
}

};  // namespace Tuple

namespace Vector {

template <size_t... I>
void PrintVec_(const std::vector<int>& args, const std::index_sequence<I...>) {
  std::cout << absl::StrFormat("%v-%v", args[I]...) << "\n";
}

template <size_t num_args>
void PrintVec(const std::vector<int>&& args) {
  assert(args.size() == num_args);  // just to be sure
  PrintVec_(args, std::make_index_sequence<num_args>{});
}

void Unpack() {
  // This has to be a compile time argument
  PrintVec<2>(std::vector<int>{1, 2});
}
}
;  // namespace Vector

int main() {
  Tuple::Unpack();
  std::cout << "\n";

  Vector::Unpack();

  return 0;
}
