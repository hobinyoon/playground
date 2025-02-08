#include <gflags/gflags.h>
#include <glog/logging.h>

#include <functional>
#include <memory>

template <typename Func>
void Func1(Func&& f) {
  std::function<void()> fo1;
}

template <typename Func>
void Func2(Func&& f) {
  std::function<void()> fo1;
  fo1 = std::forward<Func>(f);
}

int main(int argc, char** argv) {
  FLAGS_logtostdout = true;
  FLAGS_colorlogtostdout = true;
  google::InitGoogleLogging(argv[0]);

  LOG(INFO) << "aa";
  LOG(INFO) << "bb";

  auto int1 = std::make_unique<int>();

  // Compiles ok
  Func1([int1 = std::move(int1)]() {});

  // Compile error probably because of the captured variable needs to be copied in Func2.
  //
  // Func2([int1 = std::move(int1)](){});
  //
  // Alternatively I think we can wrap the function object with a smart pointer and avoid the copy

  // Capturing shared_ptr worked since they are copyable.
  {
    // Incorrect way of converting a unique_ptr to shared_ptr
    // Func2([int2 = std::make_shared<int>(std::move(int1))](){});

    // Compiles ok
    Func2([int2 = std::shared_ptr<int>(std::move(int1))]() {});
  }

  return 0;
}
