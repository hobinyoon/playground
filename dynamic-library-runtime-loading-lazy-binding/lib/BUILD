cc_library(
    name = "my_lib",
    srcs = ["my_lib.cc"],
    hdrs = [],
    # This is the key part: it tells Bazel to build a shared library (.so)
    # instead of a static one (.a).
    linkstatic = False,
    visibility = ["//visibility:public"],
)
