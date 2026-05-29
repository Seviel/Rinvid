param(
    [string]$Version = "0.2.0",
    [string]$BuildDir = "build-release-windows",
    [string]$DistDir = "dist"
)

$ErrorActionPreference = "Stop"

function Copy-RequiredFile {
    param(
        [string]$Source,
        [string]$Destination
    )

    if (-not (Test-Path -LiteralPath $Source -PathType Leaf)) {
        throw "Required file not found: $Source"
    }

    $destinationDirectory = Split-Path -Parent $Destination
    New-Item -ItemType Directory -Force -Path $destinationDirectory | Out-Null
    Copy-Item -LiteralPath $Source -Destination $Destination -Force
}

function Copy-OptionalFile {
    param(
        [string]$Source,
        [string]$Destination
    )

    if (Test-Path -LiteralPath $Source -PathType Leaf) {
        $destinationDirectory = Split-Path -Parent $Destination
        New-Item -ItemType Directory -Force -Path $destinationDirectory | Out-Null
        Copy-Item -LiteralPath $Source -Destination $Destination -Force
    }
}

function Copy-RequiredDirectory {
    param(
        [string]$Source,
        [string]$Destination
    )

    if (-not (Test-Path -LiteralPath $Source -PathType Container)) {
        throw "Required directory not found: $Source"
    }

    New-Item -ItemType Directory -Force -Path (Split-Path -Parent $Destination) | Out-Null
    Copy-Item -LiteralPath $Source -Destination $Destination -Recurse -Force
}

function Copy-OptionalDirectory {
    param(
        [string]$Source,
        [string]$Destination
    )

    if (Test-Path -LiteralPath $Source -PathType Container) {
        New-Item -ItemType Directory -Force -Path (Split-Path -Parent $Destination) | Out-Null
        Copy-Item -LiteralPath $Source -Destination $Destination -Recurse -Force
    }
}

$repoRoot = Resolve-Path (Join-Path $PSScriptRoot "..")
$buildPath = Resolve-Path (Join-Path $repoRoot $BuildDir)
$distPath = Join-Path $repoRoot $DistDir
$packageName = "rinvid-$Version-windows-mingw64"
$stagePath = Join-Path $distPath $packageName
$zipPath = "$stagePath.zip"

$resolvedRepoRoot = [System.IO.Path]::GetFullPath($repoRoot)
$resolvedDistPath = [System.IO.Path]::GetFullPath($distPath)
$resolvedStagePath = [System.IO.Path]::GetFullPath($stagePath)

if (-not $resolvedDistPath.StartsWith($resolvedRepoRoot, [System.StringComparison]::OrdinalIgnoreCase)) {
    throw "Refusing to package outside the repository: $resolvedDistPath"
}

if (-not $resolvedStagePath.StartsWith($resolvedDistPath, [System.StringComparison]::OrdinalIgnoreCase)) {
    throw "Refusing to stage outside the distribution directory: $resolvedStagePath"
}

if (Test-Path -LiteralPath $stagePath) {
    Remove-Item -LiteralPath $stagePath -Recurse -Force
}

if (Test-Path -LiteralPath $zipPath) {
    Remove-Item -LiteralPath $zipPath -Force
}

New-Item -ItemType Directory -Force -Path $stagePath | Out-Null

$readme = @"
Rinvid $Version Windows MinGW64 release

This package was built with MSYS2 MinGW64 and Ninja.

Contents:
- demos/: runnable example applications with their runtime DLLs and resources.
- sdk/: Rinvid headers, selected dependency headers, librinvid.a, required
  dependency/import libraries, and runtime DLLs for MinGW64 consumers.
- licenses/: Rinvid and third-party license notices.

Run demos by opening an executable from its own folder under demos/.
For SDK use, link with sdk/lib/librinvid.a plus the dependency/import libraries
in sdk/lib, and ship the DLLs from sdk/bin next to your final executable.
"@

Set-Content -LiteralPath (Join-Path $stagePath "README.txt") -Value $readme -Encoding ASCII
Copy-RequiredFile (Join-Path $repoRoot "README.md") (Join-Path $stagePath "README.md")
Copy-RequiredFile (Join-Path $repoRoot "LICENSE") (Join-Path $stagePath "licenses/Rinvid-LICENSE.txt")
Copy-RequiredFile (Join-Path $repoRoot "THIRD_PARTY_NOTICES.md") `
    (Join-Path $stagePath "licenses/THIRD_PARTY_NOTICES.md")

$licenseCopies = @(
    @("extern/sfml/license.md", "licenses/SFML-license.md"),
    @("extern/sfml/readme.md", "licenses/SFML-readme.md"),
    @("extern/freetype/LICENSE.TXT", "licenses/FreeType-LICENSE.TXT"),
    @("extern/freetype/docs/FTL.TXT", "licenses/FreeType-FTL.TXT"),
    @("extern/glm/copying.txt", "licenses/GLM-copying.txt"),
    @("extern/stb/LICENSE", "licenses/stb-LICENSE.txt"),
    @("extern/glad/include/KHR/khrplatform.h", "licenses/Khronos-khrplatform-notice.h")
)

foreach ($copy in $licenseCopies) {
    Copy-OptionalFile (Join-Path $repoRoot $copy[0]) (Join-Path $stagePath $copy[1])
}

$buildLicenseCopies = @(
    @("_deps/ogg-src/COPYING", "licenses/libogg-COPYING.txt"),
    @("_deps/vorbis-src/COPYING", "licenses/libvorbis-COPYING.txt"),
    @("_deps/flac-src/COPYING.Xiph", "licenses/libFLAC-COPYING.Xiph.txt")
)

foreach ($copy in $buildLicenseCopies) {
    Copy-OptionalFile (Join-Path $buildPath $copy[0]) (Join-Path $stagePath $copy[1])
}

$msysLicenseRoot = "C:/msys64/mingw64/share/licenses"
Copy-OptionalFile (Join-Path $msysLicenseRoot "gcc-libs/COPYING3") `
    (Join-Path $stagePath "licenses/MinGW-gcc-libs-COPYING3.txt")
Copy-OptionalFile (Join-Path $msysLicenseRoot "gcc-libs/COPYING.RUNTIME") `
    (Join-Path $stagePath "licenses/MinGW-gcc-runtime-exception.txt")
Copy-OptionalFile (Join-Path $msysLicenseRoot "gcc-libs/README") `
    (Join-Path $stagePath "licenses/MinGW-gcc-libs-README.txt")
Copy-OptionalFile (Join-Path $msysLicenseRoot "libwinpthread/COPYING") `
    (Join-Path $stagePath "licenses/MinGW-libwinpthread-COPYING.txt")

$demos = @(
    "full_demo",
    "gui_showcase",
    "perf_stress",
    "physix",
    "primitive_shapes",
    "screens",
    "shaders",
    "sprites",
    "testing_grounds"
)

foreach ($demo in $demos) {
    $demoBuildDir = Join-Path $buildPath "examples/$demo"
    $demoStageDir = Join-Path $stagePath "demos/$demo"
    $exePath = Join-Path $demoBuildDir "$demo.exe"

    Copy-RequiredFile $exePath (Join-Path $demoStageDir "$demo.exe")

    Get-ChildItem -LiteralPath $demoBuildDir -Filter "*.dll" -File | ForEach-Object {
        Copy-RequiredFile $_.FullName (Join-Path $demoStageDir $_.Name)
    }

    Copy-OptionalDirectory (Join-Path $demoBuildDir "resources") (Join-Path $demoStageDir "resources")
    Copy-OptionalFile (Join-Path $repoRoot "examples/$demo/README.md") (Join-Path $demoStageDir "README.md")
}

$sdkPath = Join-Path $stagePath "sdk"
Copy-RequiredDirectory (Join-Path $repoRoot "include/rinvid") (Join-Path $sdkPath "include/rinvid")
Copy-RequiredDirectory (Join-Path $repoRoot "extern/sfml/include/SFML") (Join-Path $sdkPath "include/SFML")
Copy-RequiredDirectory (Join-Path $repoRoot "extern/glm/glm") (Join-Path $sdkPath "include/glm")
Copy-RequiredDirectory (Join-Path $repoRoot "extern/glad/include/glad") (Join-Path $sdkPath "include/glad")
Copy-RequiredDirectory (Join-Path $repoRoot "extern/glad/include/KHR") (Join-Path $sdkPath "include/KHR")
Copy-RequiredDirectory (Join-Path $repoRoot "extern/freetype/include/freetype") `
    (Join-Path $sdkPath "include/freetype")
Copy-RequiredFile (Join-Path $repoRoot "extern/freetype/include/ft2build.h") `
    (Join-Path $sdkPath "include/ft2build.h")

$sdkLibraries = @(
    @("librinvid.a", "librinvid.a"),
    @("freetype/libfreetype.a", "libfreetype.a"),
    @("sfml/lib/libsfml-audio.a", "libsfml-audio.a"),
    @("sfml/lib/libsfml-system.a", "libsfml-system.a"),
    @("sfml/lib/libsfml-window.a", "libsfml-window.a")
)

foreach ($library in $sdkLibraries) {
    Copy-RequiredFile (Join-Path $buildPath $library[0]) (Join-Path $sdkPath "lib/$($library[1])")
}

$sdkReadme = @"
Rinvid SDK link notes for MSYS2 MinGW64

The monolithic Rinvid archive is sdk/lib/librinvid.a. The other libraries in
sdk/lib are dependency/import libraries required when linking a consumer
application manually.

Typical link order:
  -lrinvid -lfreetype -lsfml-window -lsfml-audio -lsfml-system -lopengl32 -lwinmm -lgdi32

Copy the DLLs from sdk/bin next to the executable you ship.
"@

Set-Content -LiteralPath (Join-Path $sdkPath "README.txt") -Value $sdkReadme -Encoding ASCII

$sdkBin = Join-Path $sdkPath "bin"
New-Item -ItemType Directory -Force -Path $sdkBin | Out-Null
Get-ChildItem -LiteralPath (Join-Path $stagePath "demos") -Recurse -Filter "*.dll" -File |
    Sort-Object Name -Unique |
    ForEach-Object {
        Copy-RequiredFile $_.FullName (Join-Path $sdkBin $_.Name)
    }

Compress-Archive -LiteralPath $stagePath -DestinationPath $zipPath -CompressionLevel Optimal

Write-Host "Created package: $zipPath"
