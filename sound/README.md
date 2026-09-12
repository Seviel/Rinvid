# Sound

`rinvid::sound::Sound` loads an audio file into memory and owns both its samples and playback.

## Playback controls

- `play()` starts or resumes playback; calling it on a playing sound restarts it.
- `pause()` preserves the position. Pausing a stopped sound has no effect.
- `stop()` stops and rewinds, including when paused or already stopped.
- `get_status()` returns `PlaybackStatus::Stopped`, `Paused`, or `Playing`.
- `get_duration()` returns the original audio duration in seconds, unaffected by pitch or looping.
- `get_playing_offset()` and `set_playing_offset(double seconds)` inspect and seek within the
  audio. Seeking preserves playback state and rounds to an audio sample boundary. Valid
  positions include zero and the duration. Seeking to the end of active playback lets the
  backend finish or loop on its next audio update.
- `set_looping(bool)` and `is_looping()` control repetition; the default is false.
- `set_volume(float)` and `get_volume()` use the range 0–100, with a default of 100. Finite
  values outside the range are clamped.
- `set_pitch(float)` and `get_pitch()` control pitch and speed together; the default is 1.
  For example, 2 plays twice as fast and at a higher pitch.

Failed file loads, non-finite parameters, non-positive pitch factors, and seek positions
outside `[0, get_duration()]` throw `rinvid::AudioError`. Failed parameter validation leaves
the previous setting unchanged. No SFML types are needed in application code.

## Music

`rinvid::sound::Music` streams a track from disk and loops it by default. It owns the file
stream and playback; keep the file accessible until switching tracks or destroying the music.
No per-frame update is needed.

For a small game, keep one `Music` object alive across your menu and levels:

`play(file_path)` opens the new track, stops the previous one, and starts the new track at
zero. Volume, pitch, and looping carry over. A failed open throws `rinvid::AudioError` with
the file path and leaves the previous track and its playback state unchanged. Call it once
when entering a menu or level; passing the same path again restarts that track.

The remaining controls use the same names and units as `Sound`: `play()`, `pause()`, `stop()`,
status, duration, playing offset, volume, pitch, and looping. Use `set_looping(false)` for a
track that should finish once. `Music` is move-only; moves preserve playback and destruction
stops it and closes the file.

The `testing_grounds` example demonstrates switching between menu and level loops. Both
sound and music tests run in `rinvid_sound_test`, using SFML's silent playback device.
