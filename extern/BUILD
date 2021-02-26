load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "extern",
    srcs = [
        "stb_image.cpp",
    ],
    hdrs =
        glob(
            [
                "glm/**/*.hpp",
                "glm/**/*.h",
                "glm/**/*.inl",
                "include/stb_image.h",
            ],
        ),
    # This is needed to suppress warnings introduced by stb_image.h
    copts = ["-Wno-sign-compare -Wno-unused-but-set-variable"],
    visibility = [
        "//core:__subpackages__",
        "//examples:__subpackages__",
        "//util:__subpackages__",
    ],
)
