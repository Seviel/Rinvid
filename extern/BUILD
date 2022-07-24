load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "extern",
    srcs = [
        "stb_image.cpp",
    ] + select({
        "@bazel_tools//src/conditions:windows": ["glad/src/glad.c"],
        "//conditions:default": [],
    }),
    hdrs =
        glob(
            [
                "glm/**/*.hpp",
                "glm/**/*.h",
                "glm/**/*.inl",
                "stb/stb_image.h",
            ],
        ) + select({
            "@bazel_tools//src/conditions:windows": [
                "glad/include/glad/glad.h",
                "glad/include/KHR/khrplatform.h",
            ],
            "//conditions:default": [],
        }),
    # This is needed to suppress warnings introduced by stb_image.h
    copts = ["-Wno-sign-compare -Wno-unused-but-set-variable"],
    visibility = [
        "//core:__subpackages__",
        "//examples:__subpackages__",
        "//util:__subpackages__",
    ],
)
