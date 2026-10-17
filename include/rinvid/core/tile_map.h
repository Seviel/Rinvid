/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_CORE_TILE_MAP_H
#define INCLUDE_RINVID_CORE_TILE_MAP_H

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <utility>
#include <vector>

#include <rinvid/core/drawable.h>
#include <rinvid/core/texture.h>
#include <rinvid/data_types/tile_map_data.h>

namespace rinvid
{

/**************************************************************************************************
 * @brief A static Tiled map rendered with the active RenderContext and camera.
 * Owns its atlas textures and a static vertex buffer. Create and destroy it while the owning
 * OpenGL context is active, as with Sprite and Texture. Coordinates start at the map's top left.
 * 
 *************************************************************************************************/
class TileMap : public Drawable
{
  public:
    /**************************************************************************************************
     * @brief Loads a TMX map, its atlas images, and its geometry.
     * 
     * @param file_name TMX file to read; see load_tiled_map for the supported subset.
     * 
     * @throws ResourceLoadError if the map or images cannot be loaded or validated.
     * @throws GraphicsError if there is no active Rinvid render context.
     * 
     *************************************************************************************************/
    explicit TileMap(const std::filesystem::path& file_name);

    /**************************************************************************************************
     * @brief Releases the map's GPU resources.
     * 
     *************************************************************************************************/
    ~TileMap() override;

    /**************************************************************************************************
     * @brief Copy construction is disabled; the map owns GPU resources.
     * 
     *************************************************************************************************/
    TileMap(const TileMap&) = delete;

    /**************************************************************************************************
     * @brief Copy assignment is disabled.
     * 
     *************************************************************************************************/
    TileMap& operator=(const TileMap&) = delete;

    /**************************************************************************************************
     * @brief Move construction is disabled, matching Sprite's lifetime semantics.
     * 
     *************************************************************************************************/
    TileMap(TileMap&&) = delete;

    /**************************************************************************************************
     * @brief Move assignment is disabled.
     * 
     *************************************************************************************************/
    TileMap& operator=(TileMap&&) = delete;

    /**************************************************************************************************
     * @brief Draws visible layers in file order using the default texture shader.
     * 
     *************************************************************************************************/
    void draw() override;

    /**************************************************************************************************
     * @brief Draws visible layers using a shader compatible with Rinvid's texture shader.
     * 
     * @param shader Shader to use.
     * 
     *************************************************************************************************/
    void draw(const Shader shader) override;

    /**************************************************************************************************
     * @brief Draws one layer, allowing game objects to be drawn between map layers.
     * Hidden layers are skipped. Uses the default texture shader.
     * 
     * @param layer_index Zero-based index in get_data().layers_.
     * 
     * @throws std::out_of_range if layer_index is invalid.
     * 
     *************************************************************************************************/
    void draw_layer(std::size_t layer_index);

    /**************************************************************************************************
     * @brief Returns immutable map metadata and tile IDs for application queries.
     * 
     * @return Parsed map data, valid for this TileMap's lifetime.
     * 
     *************************************************************************************************/
    const TileMapData& get_data() const;

  private:
    struct Batch
    {
        std::size_t  tileset_{};
        std::int32_t first_{};
        std::int32_t count_{};
    };

    void build();
    void release_gl_resources();
    void draw_layer(std::size_t index, const Shader shader);

    TileMapData                                      data_{};
    std::vector<Texture>                             textures_{};
    std::vector<Batch>                               batches_{};
    std::vector<std::pair<std::size_t, std::size_t>> layer_batches_{};
    std::uint32_t                                    vertex_array_{};
    std::uint32_t                                    vertex_buffer_{};
};

} // namespace rinvid

#endif // INCLUDE_RINVID_CORE_TILE_MAP_H
