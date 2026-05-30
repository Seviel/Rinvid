#!/usr/bin/env bash

set -euo pipefail

version="0.2.0"
build_dir="build-release-linux"
dist_dir="dist"
platform="linux-x86_64"

usage() {
    cat <<EOF
Usage: bash tools/package_linux_release.sh [options]

Options:
  --version VERSION      Release version. Default: ${version}
  --build-dir DIR       CMake build directory. Default: ${build_dir}
  --dist-dir DIR        Distribution output directory. Default: ${dist_dir}
  --platform NAME       Package platform suffix. Default: ${platform}
  -h, --help            Show this help.

Environment:
  CMAKE=/path/to/cmake  Override the CMake executable. The script otherwise
                        auto-detects a CMake 3.28+ executable when possible.
EOF
}

require_option_value() {
    local option="$1"
    local value="${2:-}"

    if [[ -z "${value}" || "${value}" == --* ]]; then
        echo "Missing value for ${option}" >&2
        usage >&2
        exit 2
    fi
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        --version)
            require_option_value "$1" "${2:-}"
            version="$2"
            shift 2
            ;;
        --build-dir)
            require_option_value "$1" "${2:-}"
            build_dir="$2"
            shift 2
            ;;
        --dist-dir)
            require_option_value "$1" "${2:-}"
            dist_dir="$2"
            shift 2
            ;;
        --platform)
            require_option_value "$1" "${2:-}"
            platform="$2"
            shift 2
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            echo "Unknown argument: $1" >&2
            usage >&2
            exit 2
            ;;
    esac
done

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "${script_dir}/.." && pwd)"

resolve_path() {
    local path="$1"
    if [[ "${path}" = /* ]]; then
        realpath -m "${path}"
    else
        realpath -m "${repo_root}/${path}"
    fi
}

build_path="$(resolve_path "${build_dir}")"
dist_path="$(resolve_path "${dist_dir}")"
package_name="rinvid-${version}-${platform}"
stage_path="${dist_path}/${package_name}"
archive_path="${stage_path}.tar.gz"

demos=(
    "full_demo"
    "gui_showcase"
    "perf_stress"
    "physix"
    "primitive_shapes"
    "screens"
    "shaders"
    "sprites"
    "testing_grounds"
)

require_command() {
    local command_name="$1"
    if ! command -v "${command_name}" >/dev/null 2>&1; then
        echo "Required command not found: ${command_name}" >&2
        exit 1
    fi
}

cmake_command="${CMAKE:-}"

cmake_version() {
    local candidate="$1"
    "${candidate}" --version 2>/dev/null | sed -n '1s/^cmake version //p'
}

version_at_least() {
    local version="$1"
    local required_major="$2"
    local required_minor="$3"
    local required_patch="$4"
    local version_core="${version%%[-+~]*}"
    local major=0
    local minor=0
    local patch=0

    IFS=. read -r major minor patch <<< "${version_core}"
    minor="${minor:-0}"
    patch="${patch:-0}"

    if ! [[ "${major}" =~ ^[0-9]+$ && "${minor}" =~ ^[0-9]+$ && "${patch}" =~ ^[0-9]+$ ]]; then
        return 1
    fi

    if ((major > required_major)); then
        return 0
    fi
    if ((major < required_major)); then
        return 1
    fi
    if ((minor > required_minor)); then
        return 0
    fi
    if ((minor < required_minor)); then
        return 1
    fi
    ((patch >= required_patch))
}

cmake_is_supported() {
    local candidate="$1"
    local version
    version="$(cmake_version "${candidate}")"
    version_at_least "${version}" 3 28 0
}

select_cmake_command() {
    local candidate
    local selected_version

    if [[ -n "${cmake_command}" ]]; then
        require_command "${cmake_command}"
        selected_version="$(cmake_version "${cmake_command}")"
        if ! version_at_least "${selected_version}" 3 28 0; then
            echo "CMake 3.28 or newer is required, but ${cmake_command} is ${selected_version:-unknown}." >&2
            exit 1
        fi
        echo "Using CMake ${selected_version}: ${cmake_command}"
        return
    fi

    for candidate in cmake /opt/cmake/bin/cmake cmake3 /usr/local/bin/cmake /snap/bin/cmake "${HOME}/.local/bin/cmake"; do
        if command -v "${candidate}" >/dev/null 2>&1 && cmake_is_supported "${candidate}"; then
            cmake_command="$(command -v "${candidate}")"
            selected_version="$(cmake_version "${cmake_command}")"
            echo "Using CMake ${selected_version}: ${cmake_command}"
            return
        fi
    done

    if command -v cmake >/dev/null 2>&1; then
        selected_version="$(cmake_version cmake)"
        echo "CMake 3.28 or newer is required, but cmake is ${selected_version:-unknown}." >&2
    else
        echo "CMake 3.28 or newer is required, but no cmake executable was found." >&2
    fi
    echo "Install a newer CMake or run with CMAKE=/path/to/cmake." >&2
    exit 1
}

require_file() {
    local source="$1"
    if [[ ! -f "${source}" ]]; then
        echo "Required file not found: ${source}" >&2
        exit 1
    fi
}

require_dir() {
    local source="$1"
    if [[ ! -d "${source}" ]]; then
        echo "Required directory not found: ${source}" >&2
        exit 1
    fi
}

copy_required_file() {
    local source="$1"
    local destination="$2"
    require_file "${source}"
    mkdir -p "$(dirname "${destination}")"
    cp -a "${source}" "${destination}"
}

copy_optional_file() {
    local source="$1"
    local destination="$2"
    if [[ -f "${source}" ]]; then
        mkdir -p "$(dirname "${destination}")"
        cp -a "${source}" "${destination}"
    fi
}

copy_required_dir() {
    local source="$1"
    local destination="$2"
    require_dir "${source}"
    mkdir -p "$(dirname "${destination}")"
    cp -a "${source}" "${destination}"
}

copy_optional_dir() {
    local source="$1"
    local destination="$2"
    if [[ -d "${source}" ]]; then
        mkdir -p "$(dirname "${destination}")"
        cp -a "${source}" "${destination}"
    fi
}

configure_release_build() {
    if [[ -f "${build_path}/CMakeCache.txt" &&
          ( -f "${build_path}/build.ninja" || -f "${build_path}/Makefile" ) ]]; then
        return
    fi

    local generator_args=()
    if command -v ninja >/dev/null 2>&1; then
        generator_args=(-G Ninja)
    fi

    echo "Configuring release build in ${build_path}"
    "${cmake_command}" -S "${repo_root}" -B "${build_path}" "${generator_args[@]}" \
        -DCMAKE_BUILD_TYPE=Release \
        -DRINVID_BUILD_TESTS=OFF
}

build_release_artifacts() {
    local build_targets=(
        "rinvid"
        "freetype"
        "sfml-window"
        "sfml-audio"
        "${demos[@]}"
    )

    select_cmake_command
    configure_release_build

    echo "Building release package targets in ${build_path}"
    "${cmake_command}" --build "${build_path}" --config Release --parallel --target "${build_targets[@]}"
}

copy_sfml_runtime_libs() {
    local destination="$1"
    local sfml_lib_dir="${build_path}/sfml/lib"

    require_dir "${sfml_lib_dir}"
    mkdir -p "${destination}"

    local copied=0
    while IFS= read -r -d '' library; do
        cp -a "${library}" "${destination}/"
        copied=$((copied + 1))
    done < <(
        find "${sfml_lib_dir}" -maxdepth 1 -type f \
            \( -name "libsfml-audio.so*" \
            -o -name "libsfml-system.so*" \
            -o -name "libsfml-window.so*" \) -print0
        find "${sfml_lib_dir}" -maxdepth 1 -type l \
            \( -name "libsfml-audio.so*" \
            -o -name "libsfml-system.so*" \
            -o -name "libsfml-window.so*" \) -print0
    )

    if [[ "${copied}" -eq 0 ]]; then
        echo "No SFML runtime libraries found in ${sfml_lib_dir}" >&2
        exit 1
    fi
}

write_demo_launcher() {
    local demo="$1"
    local destination="$2"

    cat > "${destination}" <<EOF
#!/usr/bin/env bash
set -euo pipefail
script_dir="\$(cd "\$(dirname "\${BASH_SOURCE[0]}")" && pwd)"
export LD_LIBRARY_PATH="\${script_dir}\${LD_LIBRARY_PATH:+:\${LD_LIBRARY_PATH}}"
exec "\${script_dir}/${demo}" "\$@"
EOF
    chmod +x "${destination}"
}

mkdir -p "${build_path}" "${dist_path}"
resolved_repo_root="$(realpath "${repo_root}")"
resolved_dist_path="$(realpath "${dist_path}")"
resolved_stage_path="$(realpath -m "${stage_path}")"

case "${resolved_dist_path}" in
    "${resolved_repo_root}"/*) ;;
    *)
        echo "Refusing to package outside the repository: ${resolved_dist_path}" >&2
        exit 1
        ;;
esac

case "${resolved_stage_path}" in
    "${resolved_dist_path}"/*) ;;
    *)
        echo "Refusing to stage outside the distribution directory: ${resolved_stage_path}" >&2
        exit 1
        ;;
esac

build_release_artifacts

rm -rf "${stage_path}"
rm -f "${archive_path}"
mkdir -p "${stage_path}"

cat > "${stage_path}/README.txt" <<EOF
Rinvid ${version} ${platform} release

This package is intended for a Linux x86_64 release build produced with CMake.

Contents:
- demos/: runnable example applications, SFML runtime libraries, resources, and
  launcher scripts that set LD_LIBRARY_PATH for the local demo folder.
- sdk/: Rinvid headers, selected dependency headers, librinvid.a, required
  dependency/shared libraries, and SDK link notes.
- licenses/: Rinvid and third-party license notices.

Run demos through their launcher scripts, for example:
  ./demos/full_demo/run_full_demo.sh

Linux system dependencies such as OpenGL, X11, udev, libc, libstdc++, and the
system C/C++ runtime are expected to be provided by the target distribution.
EOF

copy_required_file "${repo_root}/README.md" "${stage_path}/README.md"
copy_required_file "${repo_root}/LICENSE" "${stage_path}/licenses/Rinvid-LICENSE.txt"
copy_required_file "${repo_root}/THIRD_PARTY_NOTICES.md" \
    "${stage_path}/licenses/THIRD_PARTY_NOTICES.md"

license_copies=(
    "extern/sfml/license.md|licenses/SFML-license.md"
    "extern/sfml/readme.md|licenses/SFML-readme.md"
    "extern/freetype/LICENSE.TXT|licenses/FreeType-LICENSE.TXT"
    "extern/freetype/docs/FTL.TXT|licenses/FreeType-FTL.TXT"
    "extern/glm/copying.txt|licenses/GLM-copying.txt"
    "extern/stb/LICENSE|licenses/stb-LICENSE.txt"
    "extern/glad/include/KHR/khrplatform.h|licenses/Khronos-khrplatform-notice.h"
)

for entry in "${license_copies[@]}"; do
    source_path="${entry%%|*}"
    destination_path="${entry##*|}"
    copy_optional_file "${repo_root}/${source_path}" "${stage_path}/${destination_path}"
done

build_license_copies=(
    "_deps/ogg-src/COPYING|licenses/libogg-COPYING.txt"
    "_deps/vorbis-src/COPYING|licenses/libvorbis-COPYING.txt"
    "_deps/flac-src/COPYING.Xiph|licenses/libFLAC-COPYING.Xiph.txt"
)

for entry in "${build_license_copies[@]}"; do
    source_path="${entry%%|*}"
    destination_path="${entry##*|}"
    copy_optional_file "${build_path}/${source_path}" "${stage_path}/${destination_path}"
done

for demo in "${demos[@]}"; do
    demo_build_dir="${build_path}/examples/${demo}"
    demo_stage_dir="${stage_path}/demos/${demo}"
    executable_path="${demo_build_dir}/${demo}"

    copy_required_file "${executable_path}" "${demo_stage_dir}/${demo}"
    copy_sfml_runtime_libs "${demo_stage_dir}"
    copy_optional_dir "${demo_build_dir}/resources" "${demo_stage_dir}/resources"
    copy_optional_file "${repo_root}/examples/${demo}/README.md" "${demo_stage_dir}/README.md"
    write_demo_launcher "${demo}" "${demo_stage_dir}/run_${demo}.sh"
done

sdk_path="${stage_path}/sdk"
copy_required_dir "${repo_root}/include/rinvid" "${sdk_path}/include/rinvid"
copy_required_dir "${repo_root}/extern/sfml/include/SFML" "${sdk_path}/include/SFML"
copy_required_dir "${repo_root}/extern/glm/glm" "${sdk_path}/include/glm"
copy_required_dir "${repo_root}/extern/glad/include/glad" "${sdk_path}/include/glad"
copy_required_dir "${repo_root}/extern/glad/include/KHR" "${sdk_path}/include/KHR"
copy_required_dir "${repo_root}/extern/freetype/include/freetype" "${sdk_path}/include/freetype"
copy_required_file "${repo_root}/extern/freetype/include/ft2build.h" "${sdk_path}/include/ft2build.h"

copy_required_file "${build_path}/librinvid.a" "${sdk_path}/lib/librinvid.a"
copy_required_file "${build_path}/freetype/libfreetype.a" "${sdk_path}/lib/libfreetype.a"
copy_sfml_runtime_libs "${sdk_path}/lib"

cat > "${sdk_path}/README.txt" <<EOF
Rinvid SDK link notes for ${platform}

The monolithic Rinvid archive is sdk/lib/librinvid.a. The SFML files in
sdk/lib are shared libraries used for both linking and runtime loading.

Typical manual link line:
  -lrinvid -lfreetype -lsfml-window -lsfml-audio -lsfml-system -lGL -ldl -pthread

When running a consumer executable, make the SFML shared libraries available
through LD_LIBRARY_PATH, rpath, installation into a system library directory, or
by copying them next to the executable and using a launcher script similar to
the demo launchers.
EOF

tar -czf "${archive_path}" -C "${dist_path}" "${package_name}"

echo "Created package: ${archive_path}"
if command -v sha256sum >/dev/null 2>&1; then
    sha256sum "${archive_path}"
fi
