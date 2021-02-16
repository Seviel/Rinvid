load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "rinvid_core",
    srcs = [
        "animation.cpp",
        "circle_shape.cpp",
        "quad_shape.cpp",
        "rectangle_shape.cpp",
        "rinvid_gfx.cpp",
        "shape.cpp",
        "sprite.cpp",
        "texture.cpp",
        "triangle_shape.cpp",
    ],
    hdrs = [
        "include/animation.h",
        "include/circle_shape.h",
        "include/fixed_polygon_shape.h",
        "include/quad_shape.h",
        "include/rectangle_shape.h",
        "include/rinvid_gfx.h",
        "include/shape.h",
        "include/sprite.h",
        "include/texture.h",
        "include/triangle_shape.h",
    ],
    linkopts = ["-lsfml-window -lsfml-system -lGL"],
    visibility = ["//examples:__subpackages__"],
    deps = ["//util"],
)
