cc_binary(
    name = "Rinvid",
    srcs = ["main.cpp"],
    copts = ["-Werror -Wall -Wextra -pedantic"],
    data = [
        ":resources/aquifer.ttf",
        ":resources/bell.wav",
        ":resources/bell_button.png",
        ":resources/clck.png",
        ":resources/logo.png",
        ":resources/minus_button.png",
        ":resources/opacity_button.png",
        ":resources/plus_button.png",
        ":resources/rinvid_bg.png",
        ":resources/rotation_button.png",
        ":resources/run_reset_button.png",
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
