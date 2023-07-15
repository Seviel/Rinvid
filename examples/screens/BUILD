load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "screens",
    srcs = [
        "level_one.cpp",
        "level_one.h",
        "level_two.cpp",
        "level_two.h",
        "main.cpp",
    ],
    copts = ["-Werror -Wall -Wextra -pedantic"],
    data = [
        ":resources/guardian.png",
        ":resources/level_1.png",
        ":resources/level_2.png",
    ],
    deps = [
        "//core:rinvid_core",
        "//gui:rinvid_gui",
        "//system:rinvid_system",
        "//util",
    ],
)
