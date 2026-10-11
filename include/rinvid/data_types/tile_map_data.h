/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_DATA_TYPES_TILE_MAP_DATA_H
#define INCLUDE_RINVID_DATA_TYPES_TILE_MAP_DATA_H

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

#include <rinvid/util/vector2.h>

namespace rinvid
{

/**************************************************************************************************
 * @brief An atlas tileset. Tile dimensions equal the owning map's grid dimensions.
 *************************************************************************************************/
struct Tileset
{
    std::string           name_{};         /// Name stored in Tiled.
    std::filesystem::path image_{};        /// Absolute, normalized image path.
    std::uint32_t         first_gid_{};    /// First global tile ID, without flip flags.
    std::uint32_t         tile_count_{};   /// Number of tiles in the atlas.
    std::uint32_t         columns_{};      /// Number of atlas columns.
    std::uint32_t         margin_{};       /// Outer atlas margin in pixels.
    std::uint32_t         spacing_{};      /// Space between atlas tiles in pixels.
    std::uint32_t         image_width_{};  /// Declared image width in pixels.
    std::uint32_t         image_height_{}; /// Declared image height in pixels.
};

/**************************************************************************************************
 * @brief A finite tile layer, in map row order (left to right, top to bottom).
 *************************************************************************************************/
struct TileLayer
{
    std::string                name_{};   /// Name stored in Tiled.
    std::vector<std::uint32_t> tiles_{};  /// Raw Tiled GIDs including flip flags; zero is empty.
    Vector2f                   offset_{}; /// Layer offset in pixels.
    float                      opacity_{1.0F}; /// Layer opacity in [0, 1].
    bool                       visible_{true}; /// Whether the layer is drawn.
};

/**************************************************************************************************
 * @brief CPU-only map data, independent of textures and an OpenGL context.
 *************************************************************************************************/
struct TileMapData
{
    std::uint32_t          width_{};       /// Map width in tiles.
    std::uint32_t          height_{};      /// Map height in tiles.
    std::uint32_t          tile_width_{};  /// Grid width in pixels.
    std::uint32_t          tile_height_{}; /// Grid height in pixels.
    std::vector<Tileset>   tilesets_{};    /// Tilesets in ascending first_gid_ order.
    std::vector<TileLayer> layers_{};      /// Layers in painter's order.
};

} // namespace rinvid

#endif // INCLUDE_RINVID_DATA_TYPES_TILE_MAP_DATA_H
