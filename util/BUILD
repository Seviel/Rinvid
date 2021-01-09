load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "util",
    hdrs = [
        "include/color.h",
        "include/image_loader.h",
        "include/math_utils.h",
        "include/vector2.h", 
        "include/vector3.h", 
    ],
    srcs = [
        "image_loader.cpp"
    ],
    deps = [
        "//extern:extern",
    ],
    visibility = [
        "//core:__subpackages__",
        "//examples:__subpackages__",
    ],
)
