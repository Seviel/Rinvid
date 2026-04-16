# AGENTS.md

This file gives coding agents project-specific guidance for working in the Rinvid repository.

## Project summary

Rinvid is a C++17 2D game and multimedia framework built with CMake. The core library is compiled from:

- `core/`
- `gui/`
- `platformers/`
- `sound/`
- `system/`
- `util/`
- `extern/stb_image.cpp`
- `extern/glad/src/*.c`

Examples live in `examples/` and are built by default. Tests live in `tests/` and use GoogleTest from a source checkout.

## Important repo layout

- `CMakeLists.txt`: root build definition for the `rinvid` library and all examples
- `core/include`, `gui/include`, `platformers/include`, `sound/include`, `system/include`, `util/include`, `data_types/include`: public headers
- `tests/`: GoogleTest-based test executable `rinvid_test`
- `examples/testing_grounds/`: scratch/demo area for in-progress features
- `extern/`: vendored third-party code and docs
- `.github/workflows/`: Ubuntu and Windows CI

## Source of truth

Honor the existing project docs first:

- `README.md`: build prerequisites and platform notes
- `CONTRIBUTING.md`: contribution flow and maintainer expectations
- `CODING_GUIDELINES.md`: coding style rules that are stricter than generic C++ defaults

If this file conflicts with those docs or the existing codebase, prefer the repo docs and local code patterns.

## Editing rules

1. Do not modify `extern/` unless the task is explicitly about updating vendored dependencies or third-party docs.
2. Keep new production code within the existing module structure instead of creating ad hoc top-level folders.
3. Put public headers in the relevant `*/include/` directory and implementations beside the module `.cpp` files.
4. Preserve the license banner at the top of source and header files. New C/C++ source files should include the same BSD 2-Clause notice format used elsewhere in the repo.
5. Keep header extensions as `.h`.
6. Preserve include ordering:
   - standard library
   - SFML/other external headers
   - Rinvid headers
7. Follow naming from `CODING_GUIDELINES.md`:
   - files and functions: `snake_case`
   - types: `PascalCase`
   - member fields: trailing underscore
   - macros/include guards: `MACRO_CASE`
8. Use fixed-width integer types like `std::uint32_t`.
9. Use uniform initialization and uppercase `F` suffix for float literals.
10. Always use braces for control-flow blocks, even single-line bodies.
11. Public API changes should include Doxygen comments in the style already used in headers.
12. End files with a trailing newline.

## Build and test expectations

Preferred validation is through CMake builds that match the repo setup.

On Windows, use the MinGW64 MSYS2 terminal for local configure/build/test work. Do not assume PowerShell is the normal Windows workflow for this repository.

### Configure

Standard configure:

```sh
cmake -S . -B build
```

Disable tests if GoogleTest source is unavailable:

```sh
cmake -S . -B build -DRINVID_BUILD_TESTS=OFF
```

Use a non-default GoogleTest checkout if needed:

```sh
cmake -S . -B build -DRINVID_GTEST_SOURCE_DIR=/absolute/path/to/googletest
```

### Build

```sh
cmake --build build
```

### Test

If tests are enabled and `rinvid_test` builds, run:

```sh
ctest --test-dir build --output-on-failure
```

If `ctest` is not configured by the generator, run the produced `rinvid_test` executable directly.

### Windows notes

For Windows local development, follow the repo README workflow from a MinGW64 MSYS2 terminal. Prefer Ninja for local builds unless the task specifically needs to mirror CI's `MinGW Makefiles` generator.

Example local Windows flow:

```sh
mkdir build
cd build
cmake -S .. -B . -G Ninja
ninja all
./tests/rinvid_test.exe
```

### Formatting

Formatting is expected to follow `_clang-format`. The repository also includes:

- `apply_clang_format.sh`
- `verify_format.py`

Run formatting or mention clearly if you could not verify formatting in the current environment.

## CI awareness

CI currently builds on:

- Ubuntu (`.github/workflows/ubuntu_ci.yml`)
- Windows with MSYS2/MinGW (`.github/workflows/windows_ci.yml`)

Keep changes portable across both environments. Be careful with platform-specific includes and behavior, especially around `_WIN32`, `__unix__`, OpenGL setup, and filesystem/path assumptions.

Note that Windows CI currently uses `MinGW Makefiles`, while the documented local Windows workflow uses Ninja from the MinGW64 MSYS2 environment.

## Examples and tests policy

- New user-facing features should usually be demonstrated in `examples/testing_grounds/` first.
- When a feature is mature, prefer giving it a dedicated example under `examples/`.
- Behavior changes should add or update GoogleTest coverage in `tests/` when practical.
- Test resources belong under `tests/resources/` if the test executable needs runtime assets.

## Markdown guidance

This repository contains many Markdown files under `extern/` from vendored dependencies such as GLM, GoogleTest, and stb. Treat them as third-party reference material, not project policy. For Rinvid-specific guidance, prioritize Markdown files in the repo root, `examples/`, `tests/`, `tools/`, and `sound/`.

## Agent workflow

1. Read the relevant module header and implementation before making changes.
2. Check whether the requested behavior already exists in an example or test.
3. Make the smallest change that matches current patterns.
4. Validate with a build and, when possible, tests or the nearest example target.
5. In the final handoff, mention any validation you could not run and any assumptions made.

## Avoid

- Reformatting unrelated files
- Renaming files or symbols without a task-driven reason
- Introducing new dependencies casually
- Editing vendored code in `extern/` as part of ordinary feature work
- Adding inconsistent style that fights `CODING_GUIDELINES.md`
