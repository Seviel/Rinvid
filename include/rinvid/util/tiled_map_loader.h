/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_UTIL_TILED_MAP_LOADER_H
#define INCLUDE_RINVID_UTIL_TILED_MAP_LOADER_H

#include <filesystem>

#include <rinvid/data_types/tile_map_data.h>

namespace rinvid
{

/**************************************************************************************************
 * @brief Loads a finite, orthogonal TMX map without creating GPU resources.
 * Supports right-down tile layers, uncompressed CSV or XML tile data, and embedded or external
 * TSX atlas tilesets. Tiles must match the map grid size. Atlas dimensions must be declared.
 * Image paths are resolved relative to their owning TMX/TSX file. Image loading is deferred to
 * TileMap. Custom properties and editor metadata are ignored; unsupported visual features fail.
 *
 * @param file_name TMX file to read.
 * @return Validated map data with absolute image paths.
 * @throws ResourceLoadError on malformed input, missing map/tileset files, or unsupported features.
 *************************************************************************************************/
TileMapData load_tiled_map(const std::filesystem::path& file_name);

} // namespace rinvid

#endif // INCLUDE_RINVID_UTIL_TILED_MAP_LOADER_H
