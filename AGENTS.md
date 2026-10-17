# AGENTS.md

This file gives coding agents project-specific guidance for working in Rinvid, a CMake-based 2D game and multimedia framework. Use the repository's configured C++ standard.

## Source of truth

Read the relevant project docs before making changes:

- `README.md`: build prerequisites and platform notes.
- `CONTRIBUTING.md`: contribution flow and maintainer expectations.
- `CODING_GUIDELINES.md`: required naming, formatting, license notices, header conventions, and public API documentation.

If this file conflicts with those docs or the existing codebase, prefer the repo docs and local code patterns. Markdown under `extern/` is third-party reference material, not project policy.

## Editing workflow

- Read the relevant module header and implementation, and check existing examples and tests before changing behavior.
- Keep public headers in `include/rinvid/<module>/` and implementations in the existing module directories. Examples belong in `examples/`, tests in `tests/`, and runtime test assets in `tests/resources/`.
- Do not modify `extern/` unless the task explicitly concerns vendored dependencies or third-party docs.
- Make focused changes that match current patterns. Avoid unrelated formatting, renames, and unnecessary dependencies.
- Add or update GoogleTest coverage for behavior changes when practical. Use examples to demonstrate features when useful; `examples/testing_grounds/` is available for experiments.
- Keep changes portable across Ubuntu and Windows, especially platform-specific includes, OpenGL setup, and filesystem paths. CI definitions live in `.github/workflows/`.
- Validate code changes with the build and relevant tests or example target. In the final handoff, report assumptions and any validation that could not be run.

## Build, test, and formatting

From the repository root:

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Tests require a GoogleTest source checkout. If needed, configure with `-DRINVID_GTEST_SOURCE_DIR=/absolute/path/to/googletest`; if unavailable, use `-DRINVID_BUILD_TESTS=OFF` and report that tests were skipped. If CTest has no registered tests, run the built `rinvid_test` executable directly.

### Windows

Use the MinGW64 MSYS2 environment for configure, build, and test work. Follow the README setup and prefer Ninja locally; Windows CI uses `MinGW Makefiles`.

```sh
cmake -S . -B build -G Ninja
cmake --build build -j2
./build/tests/rinvid_test.exe
```

Verified command pattern for invoking MSYS2 Bash from Windows:

```sh
C:\msys64\usr\bin\bash.exe -lc 'export PATH=/mingw64/bin:/usr/bin:$PATH; cd /c/Users/fvasi/Desktop/dreamscape/repos/Rinvid; cmake -S . -B build -G Ninja; cmake --build build -j2'
```

If automation cannot start `bash.exe`, report the environment/sandbox limitation; PowerShell is not the documented build workflow.

### Formatting

Follow `_clang-format` and `CODING_GUIDELINES.md`. Formatting helpers are `apply_clang_format.sh` and `verify_format.py`. Check formatting for changed C/C++ files without reformatting unrelated files, or report why it could not be verified.
