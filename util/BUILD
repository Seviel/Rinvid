load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "util",
    srcs = [
        "collision_detection.cpp",
        "error_handler.cpp",
        "image_loader.cpp",
        "windows_utils.cpp",
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
        "include/windows_utils.h",
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

# Same as util but with -DRINVID_DEBUG_MODE added.
# There probably is a better way to do this.
cc_library(
    name = "util_debug",
    srcs = [
        "collision_detection.cpp",
        "error_handler.cpp",
        "image_loader.cpp",
        "windows_utils.cpp",
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
        "include/windows_utils.h",
    ],
    copts = ["-Werror -Wall -Wextra -pedantic -DRINVID_DEBUG_MODE"],
    visibility = [
        "//core:__subpackages__",
        "//examples:__subpackages__",
        "//tests:__subpackages__",
    ],
    deps = [
        "//util",
    ],
)
