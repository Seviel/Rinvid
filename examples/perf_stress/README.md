# Perf stress

This example runs a real Rinvid application that is meant to stress the full frame loop, not
just isolated draw calls. It updates and renders randomly generated textures, primitive shapes,
text, and lights. The scene content is shared with the `FullStressFrame` Google Benchmark case,
so the interactive example and the automated benchmark exercise the same workload.

## What it does

- Starts in fullscreen at `1280x720` with the FPS cap disabled.
- Creates a maximum pool of random sprites, shapes, text objects, and lights, then changes how
  many of them are active.
- Uses randomly generated RGBA textures with dimensions between `50x50` and `500x500`.
- Starts at `50` sprites, `50` shapes, `50` text objects, and `50` lights.

## Controls

- `1`: activate `10` sprites, `10` shapes, `10` text objects, and `10` lights
- `2`: activate `25` of each category
- `3`: activate `50` of each category
- `4`: activate `100` of each category
- `Esc`: quit

## Build and run

This example is part of the normal CMake build.

```shell
cmake -S . -B build
cmake --build build --target perf_stress
./examples/perf_stress/perf_stress.exe
```

On Windows, prefer the MinGW64 MSYS2 workflow described in the [root README](../../README.md).

## Runtime asset

The example uses the font `examples/testing_grounds/resources/aquifer.ttf`. The build copies it
to `resources/aquifer.ttf` next to the executable and to the build root `resources/` directory,
which keeps the example runnable from common launch locations.

## Benchmark relation

If benchmark support is enabled with `-DRINVID_BUILD_BENCHMARKS=ON`, the `rinvid_benchmark`
executable exposes the `FullStressFrame` benchmark, which uses the same shared
`StressSceneContent` workload:

```shell
./benchmarks/rinvid_benchmark.exe --benchmark_filter=FullStressFrame
```

## Notes

- Light count is currently intended to stay at or below `100`, because the renderer currently
  uses a fixed-size light array in the shader path.
