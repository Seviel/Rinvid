load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "texture_messing_around",
    srcs = ["main.cpp"],
    linkopts = ["-lsfml-window -lsfml-system"],
    deps = [
        "//core:rinvid_core",
        "//util:util",
    ],
)
