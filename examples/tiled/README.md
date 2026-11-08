# Tiled maps

This example loads `resources/garden.tmx`, follows its external `garden.tsx` tileset,
and renders four tile layers. Use **WASD / arrow keys** to move the yellow player
and **Escape** to exit. The camera follows the player.
The courtyard contains all eight horizontal/vertical/diagonal tile transforms.
The player starts at the `player_start` point. Red rectangles show collision objects
and block movement using a small example-only rectangle overlap check.
Faint decorative tiles scroll at half speed. Walk right past the screen center
or down past the middle of the screen to make the camera scroll and see the effect.

Build with the repository's usual CMake workflow:

```sh
cmake -S . -B build -G Ninja
cmake --build build --target tiled
./build/examples/tiled/tiled
```

On Windows, run these commands in the MSYS2 MINGW64 shell and use `tiled.exe`.
Resources are copied beside the executable. Launch using its path; the default map
is located relative to the executable, so running from the repository root works.
Pass a map path to open your own map:

```sh
./build/examples/tiled/tiled /path/to/level.tmx
```

## ImageCache

The example parses the map and preloads its atlas images before creating the
window. Decoding runs on the cache worker while the application initializes.
`TileMap` then uses the parsed data and cached pixels to create its GPU resources.

## Saving from Tiled

- Save as **TMX (XML)**, with **Orthogonal** orientation, a finite size, and
  **Right Down** render order.
- Set **Tile Layer Format** to **CSV**. Uncompressed XML `<tile gid="..."/>`
  data is also supported. Base64 and compressed data are not supported.
- Use atlas image tilesets, embedded in the map or saved as external **TSX** files.
  Tile dimensions must match the map grid. Include `tilecount`, `columns`, and image
  `width`/`height` (written by current Tiled for atlas tilesets).
- Multiple tilesets, empty cells, tile flips, atlas margin/spacing, layer names,
  visibility, opacity, and pixel offsets are supported. PNG alpha is supported.

## Parallax

Set a tile layer's **Parallax Factor X/Y** in Tiled. `1` is normal world scrolling,
`0.5` scrolls at half speed, `0` stays fixed as the camera pans, and values above
`1` scroll faster. Negative factors are also supported. The factors default to `1`.
Set the map's **Parallax Origin X/Y** to control the reference point; it defaults
to `(0, 0)`. Layers align with their ordinary positions when the view center is at
that origin. The example uses `(480, 320)`, its initial view center.

The renderer applies parallax per tile layer using the current camera view and
logical canvas dimensions, without rebuilding vertices or adding draw calls.
`draw()`, custom-shader drawing, and `draw_layer()` all apply it. Pixel offsets
remain part of the layer geometry and are not multiplied by the parallax factor.
Parsed factors are available as `TileLayer::parallax_` and the origin as
`TileMapData::parallax_origin_`.

Spawn points and collision rectangles remain in world coordinates. Object-layer
parallax is rejected; keep gameplay layers at factor `1` and use parallax on
decorative tile layers. Group layers and image layers remain unsupported.

The loader rejects infinite/non-orthogonal maps, group/image layers,
animations, image collection tilesets, per-tile image regions, nonzero tileset
offsets, color-key transparency, layer tint, and non-normal blend modes.
Properties outside object layers/objects and editor metadata are ignored. Map
background color is left to the application's `clear_screen()` call.
