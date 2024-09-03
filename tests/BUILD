load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "rinvid_test",
    srcs = [
        "include/sprite_test.h",
        "include/texture_test.h",
        "include/util_test.h",
        "sprite_test.cpp",
        "test_main.cpp",
        "texture_test.cpp",
        "util_test.cpp",
    ],
    copts = ["-Werror -Wall -Wextra -pedantic"],
    data = [
        ":resources/valid_image.png",
    ],
    linkopts = select({
        "@bazel_tools//src/conditions:windows": ["-lgtest"],
        "//conditions:default": ["-lpthread -lgtest"],
    }),
    deps = [
        "//core:rinvid_core",
        "//util:util_debug",
    ],
)
