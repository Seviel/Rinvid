# Tiled maps

This example loads `resources/garden.tmx`, follows its external `garden.tsx` tileset,
and renders three tile layers. Use **WASD / arrow keys** to move the yellow player
and **Escape** to exit. The camera follows the player.
The courtyard contains all eight horizontal/vertical/diagonal tile transforms.
The player starts at the `player_start` point. Red rectangles show collision objects
and block movement using a small example-only rectangle overlap check.

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

The loader rejects infinite/non-orthogonal maps, group/object/image layers,
animations, image collection tilesets, per-tile image regions, nonzero tileset
offsets, color-key transparency, layer tint, non-normal blend modes, and parallax.
Properties outside object layers/objects and editor metadata are ignored. Map
background color is left to the application's `clear_screen()` call.
