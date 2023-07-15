load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "rinvid_system",
    srcs = [
        "keyboard.cpp",
        "mouse.cpp",
    ],
    hdrs = [
        "include/keyboard.h",
        "include/mouse.h",
    ],
    copts = ["-Werror -Wall -Wextra -pedantic"],
    linkopts = select({
        "@bazel_tools//src/conditions:windows": ["-lsfml-window -lsfml-system -lopengl32 -lwinmm -lgdi32"],
        "//conditions:default": ["-lsfml-window -lsfml-system"],
    }),
    visibility = [
        "//examples:__subpackages__",
    ],
    deps = [
        "//core:rinvid_core",
    ],
)
