load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "test",
    srcs = ["main.cpp"],
    copts = ["-std=c++1z"],
    linkopts = ["-lsfml-window -lsfml-system"],
    data = [":resources/logo.png"],
    deps = [
        "//core:rinvid_core",
        "//util:util",
    ],
)
