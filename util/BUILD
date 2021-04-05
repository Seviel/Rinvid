load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "util",
    srcs = [
        "collision_detection.cpp",
        "error_handler.cpp",
        "image_loader.cpp",
    ],
    hdrs = [
        "include/collision_detection.h",
        "include/color.h",
        "include/error_handler.h",
        "include/image_loader.h",
        "include/math_utils.h",
        "include/rect.h",
        "include/vector2.h",
        "include/vector3.h",
    ],
    copts = ["-Werror -Wall -Wextra -pedantic"],
    visibility = [
        "//core:__subpackages__",
        "//examples:__subpackages__",
    ],
    deps = [
        "//extern",
    ],
)
