load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "rinvid_gui",
    srcs = ["button.cpp"],
    hdrs = ["include/button.h"],
    copts = ["-Werror -Wall -Wextra -pedantic"],
    linkopts = select({
        "@bazel_tools//src/conditions:windows": ["-lsfml-window -lsfml-system -lopengl32 -lwinmm -lgdi32"],
        "//conditions:default": ["-lsfml-window -lsfml-system"],
    }),
    deps = [
        "//core:rinvid_core",
    ],
    visibility = [
        "//examples:__subpackages__",
    ],
)
