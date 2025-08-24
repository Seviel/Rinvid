cc_binary(
    name = "Game",
    srcs = ["main.cpp"],
    copts = ["-Werror -Wall -Wextra -pedantic"],
    data = [
        ":resources/intro.png",
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
