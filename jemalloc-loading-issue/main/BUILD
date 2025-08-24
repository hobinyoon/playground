cc_binary(
    name = "main",
    srcs = ["main.cc"],
    # The key here is the data attribute. Bazel will ensure that this dependency
    # is available in the runfiles directory, where the executable can find it.
    data = [
        "//main/lib:my_lib",
    ],
    linkopts = ["-ldl"],
)