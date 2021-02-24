load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "test",
    srcs = ["main.cpp"],
    copts = ["-Werror -Wall -Wextra -pedantic"],
    data = [
        ":resources/clck.png",
        ":resources/logo.png",
    ],
    linkopts = ["-lsfml-window -lsfml-system"],
    deps = [
        "//core:rinvid_core",
        "//util",
    ],
)
