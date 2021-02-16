load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "util",
    srcs = [
        "image_loader.cpp",
    ],
    hdrs = [
        "include/color.h",
        "include/image_loader.h",
        "include/math_utils.h",
        "include/rect.h",
        "include/vector2.h",
        "include/vector3.h",
    ],
    visibility = [
        "//core:__subpackages__",
        "//examples:__subpackages__",
    ],
    deps = [
        "//extern",
    ],
)
