cc_binary(
    name = "sprites",
    srcs = ["main.cpp"],
    copts = ["-Werror -Wall -Wextra -pedantic"],
    data = [":resources/trashbot.png"],
    linkopts = select({
        "@bazel_tools//src/conditions:windows": ["-lsfml-window -lsfml-system -lopengl32 -lwinmm -lgdi32"],
        "//conditions:default": ["-lsfml-window -lsfml-system"],
    }),
    deps = [
        "//core:rinvid_core",
        "//system:rinvid_system",
        "//util",
    ],
)
