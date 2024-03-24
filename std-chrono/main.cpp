#include <chrono>
#include <iostream>

#define LogEnabled true
#define Log          \
  if (!LogEnabled) { \
  } else             \
    std::cout << __LINE__ << " " << __FUNCTION__ << ": "

using namespace std;

int main() {
  const auto now = chrono::system_clock::now();
  // Seems like in UTC
  Log << "now=" << now << "\n";

  // TODO: How does to_local know which time zone it is?

  // current_zone doesn't seem to be implemented as of clang++ 17.0.6 on MacOS
  // https://stackoverflow.com/questions/70852886/does-gcc-or-clang-implement-chronocurrent-zone
  // auto const local_time = chrono::current_zone()->to_local(now);

  // zoned_time doesn't seem to be implemented either
  // string_view location = "Africa/Casablanca";
  // const chrono::zoned_time zt{location, now};
  // Log << location << " - Zoned Time: " << zt << '\n';

  return 0;
}
