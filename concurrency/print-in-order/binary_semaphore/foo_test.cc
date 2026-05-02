#include "foo.h"

#include <future>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

namespace {

// Verifies that Foo synchronizes method execution order regardless of call order.
// Launches first(), second(), and third() in reverse order on separate threads,
// but expects output to be ordered correctly due to internal synchronization.
TEST(FooTest, PrintsInOrderWhenMethodsRunOutOfOrder) {
  Foo foo;

  std::mutex output_mutex;
  std::vector<std::string> output;

  auto append = [&](std::string value) {
    std::lock_guard<std::mutex> lock(output_mutex);
    output.push_back(std::move(value));
  };

  // Use std::launch::async to run on separate threads. If std::launch::deferred
  // were used instead, tasks would execute lazily on the calling thread during
  // get(), causing third.get() to block indefinitely waiting for first() to run.
  auto third = std::async(std::launch::async,
                          [&] { foo.third([&] { append("third"); }); });
  auto second = std::async(std::launch::async,
                           [&] { foo.second([&] { append("second"); }); });
  auto first = std::async(std::launch::async,
                          [&] { foo.first([&] { append("first"); }); });
  third.get();
  second.get();
  first.get();

  EXPECT_EQ(output, std::vector<std::string>({"first", "second", "third"}));
}

}  // namespace
