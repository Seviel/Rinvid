cc_library(
    name = "rinvid_gui",
    srcs = [
        "button.cpp",
        "label.cpp",
    ],
    hdrs = [
        "include/button.h",
        "include/label.h",
    ],
    copts = ["-Werror -Wall -Wextra -pedantic"],
    linkopts = select({
        "@bazel_tools//src/conditions:windows": ["-lsfml-window -lsfml-system -lopengl32 -lwinmm -lgdi32"],
        "//conditions:default": ["-lsfml-window -lsfml-system"],
    }),
    visibility = [
        "//examples:__subpackages__",
    ],
    deps = [
        "//core:rinvid_core",
        "//extern",
        "//util",
    ],
)
