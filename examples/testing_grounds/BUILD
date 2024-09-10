load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "test",
    srcs = ["main.cpp"],
    copts = ["-Werror -Wall -Wextra -pedantic"],
    data = [
        ":resources/aquifer.ttf",
        ":resources/clck.png",
        ":resources/default_button.png",
        ":resources/logo.png",
        ":resources/rinvid_bg.png",
        ":resources/wind.wav",
    ],
    linkopts = select({
        "@bazel_tools//src/conditions:windows": ["-lsfml-window -lsfml-system -lopengl32 -lwinmm -lgdi32"],
        "//conditions:default": ["-lsfml-window -lsfml-system"],
    }),
    deps = [
        "//core:rinvid_core",
        "//gui:rinvid_gui",
        "//sound:rinvid_sound",
        "//system:rinvid_system",
        "//util",
    ],
)
