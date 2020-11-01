load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "rinvid_core",
    srcs = [
        "rinvid_gfx.cpp",
        "shape.cpp",
        "triangle_shape.cpp",
    ],
    hdrs = [
        "include/rinvid_gfx.h",
        "include/shape.h",
        "include/triangle_shape.h",
    ],
    linkopts = ["-lsfml-window -lsfml-system -lGL"],
    deps = ["//util:util"],
    visibility = ["//examples:__subpackages__"],
)
