# Tests

Contains all the tests. Uses GoogleTest built from source as part of the CMake build.

To skip building tests entirely, configure with:

```shell
-DRINVID_BUILD_TESTS=OFF
```

The audio tests can be built and run independently of the graphics tests:

```shell
cmake --build build --target rinvid_sound_test
ctest --test-dir build -R '^rinvid_sound_test$' --output-on-failure
```

They select SFML's null playback device, so no physical audio device is required and no
sound is emitted. The target copies its own resources. `resources/valid_sound.wav` and
`resources/short_sound.wav` are generated silent PCM fixtures (mono, 16-bit, 8000 Hz), with
durations of 2 seconds and 0.25 seconds respectively.
