load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "primitive_shapes",
    srcs = ["main.cpp"],
    linkopts = ["-lsfml-window -lsfml-system"],
    deps = [
        "//core:rinvid_core",
        "//util",
    ],
)
