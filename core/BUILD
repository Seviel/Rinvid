load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "rinvid_core",
    srcs = ["rinvid_gfx.cpp"],
    hdrs = ["include/rinvid_gfx.h"],
    linkopts = ["-lsfml-window -lsfml-system -lGL"],
    visibility = ["//examples:__subpackages__"],
)
