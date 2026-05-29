# Third-Party Notices

This document lists third-party components that can be present in Rinvid source
and binary release packages. Include this file, the Rinvid license, and the
referenced license texts in official archives.

## Rinvid

Rinvid is distributed under the BSD 2-Clause License. See `LICENSE`.

## Runtime and SDK dependencies

- SFML: zlib/libpng license. See `extern/sfml/license.md`.
- FreeType: FreeType License or GPLv2. Rinvid release packages use the
  FreeType License path. See `extern/freetype/LICENSE.TXT` and
  `extern/freetype/docs/FTL.TXT`.
- GLM: MIT License or Happy Bunny License. See `extern/glm/copying.txt`.
- stb/stb_image: MIT License or public domain/Unlicense. See
  `extern/stb/LICENSE`.
- GLAD generated OpenGL loader: generated loader files are vendored under
  `extern/glad`; Khronos `khrplatform.h` carries its own MIT-style notice in
  `extern/glad/include/KHR/khrplatform.h`.

## SFML transitive components

Rinvid builds SFML Window and SFML Audio. Depending on the platform and package,
SFML can include or link to these third-party components:

- libogg: BSD-style license.
- libvorbis: BSD-style license.
- libFLAC: BSD-style/Xiph license.
- miniaudio: public domain or MIT-0.
- dr_mp3: public domain or MIT-0.
- cpp-unicodelib: MIT License.
- Khronos Vulkan headers: Apache License 2.0.
- Additional SFML platform headers can carry their own notices.

## Windows MinGW runtime

Windows MinGW binary packages can include runtime DLLs copied from the active
MSYS2 MinGW64 toolchain:

- `libgcc_s_seh-1.dll`: GPLv3 or later with the GCC Runtime Library Exception.
- `libstdc++-6.dll`: GPLv3 or later with the GCC Runtime Library Exception.
- `libwinpthread-1.dll`: MIT/BSD-style winpthreads notices.

The packaging script copies the corresponding MSYS2 license files when they are
available locally.

## Test and benchmark dependencies

These are not part of normal demo or SDK runtime packages unless test or
benchmark binaries are explicitly shipped:

- GoogleTest: BSD 3-Clause License. See `extern/googletest/LICENSE`.
- Google Benchmark: Apache License 2.0. See `extern/benchmark/LICENSE`.

## Example assets

Example resources under `examples/**/resources` are distributed as part of
Rinvid unless a resource directory includes a more specific notice.
