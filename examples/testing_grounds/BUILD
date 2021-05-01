load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "test",
    srcs = ["main.cpp"],
    copts = ["-Werror -Wall -Wextra -pedantic"],
    data = [
        ":resources/clck.png",
        ":resources/logo.png",
    ],
    linkopts = select({
        "@bazel_tools//src/conditions:windows": ["-lsfml-window -lsfml-system -lopengl32 -lwinmm -lgdi32"],
        "//conditions:default": ["-lsfml-window -lsfml-system"],
    }),
    deps = [
        "//core:rinvid_core",
        "//util",
    ],
)
