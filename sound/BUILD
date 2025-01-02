cc_library(
    name = "rinvid_sound",
    srcs = [
        "sound.cpp",
    ],
    hdrs = [
        "include/sound.h",
    ],
    copts = ["-Werror -Wall -Wextra -pedantic"],
    linkopts = select({
        "@bazel_tools//src/conditions:windows": ["-lsfml-window -lsfml-audio -lopengl32 -lwinmm -lgdi32"],
        "//conditions:default": ["-lsfml-window -lsfml-audio"],
    }),
    visibility = [
        "//examples:__subpackages__",
    ],
    deps = [
    ],
)
