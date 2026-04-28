#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/log/initialize.h"
#include "absl/log/log.h"

ABSL_FLAG(std::string, name, "world", "Name to greet.");

int main(int argc, char** argv) {
  absl::ParseCommandLine(argc, argv);
  absl::InitializeLog();
  LOG(INFO) << "Hello, " << absl::GetFlag(FLAGS_name) << "!";

  return 0;
}
