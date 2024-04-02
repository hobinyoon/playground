#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <sstream>
#include <thread>

struct OSyncStream {
  std::stringstream ss;

  OSyncStream() = default;
  OSyncStream(const OSyncStream&) = delete;
  ~OSyncStream() { std::cout << ss.str() << "\n"; }

  OSyncStream& operator=(const OSyncStream&) = delete;

  template <class T>
  OSyncStream& operator<<(const T& t) {
    ss << t;
    return *this;
  }
};

#define LogEnabled true
#define Log                                                          \
  if (!LogEnabled) {                                                 \
  } else                                                             \
    OSyncStream() << this_thread::get_id() << " " << __LINE__ << " " \
                  << __FUNCTION__ << ": "

#endif
