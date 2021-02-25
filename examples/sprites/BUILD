load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "sprites",
    srcs = ["main.cpp"],
    copts = ["-Werror -Wall -Wextra -pedantic"],
    data = [":resources/trashbot.png"],
    linkopts = ["-lsfml-window -lsfml-system"],
    deps = [
        "//core:rinvid_core",
        "//util",
    ],
)
