/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <algorithm>
#include <array>
#include <limits>
#include <utility>
#include <vector>

#include <rinvid/core/render_context.h>
#include <rinvid/core/rinvid_gl.h>
#include <rinvid/core/texture.h>
#include <rinvid/core/tile_map.h>
#include <rinvid/util/error.h>
#include <rinvid/util/tiled_map_loader.h>

namespace rinvid
{

void TileMap::release_gl_resources()
{
    if (vertex_buffer_ != 0U)
    {
        GL_CALL(glDeleteBuffers(1, &vertex_buffer_));
        vertex_buffer_ = 0U;
    }
    if (vertex_array_ != 0U)
    {
        GL_CALL(glDeleteVertexArrays(1, &vertex_array_));
        vertex_array_ = 0U;
    }
}

void TileMap::build()
{
    textures_.reserve(data_.tilesets_.size());
    for (const auto& tileset : data_.tilesets_)
    {
        textures_.emplace_back(tileset.image_.string().c_str());
        const auto& texture = textures_.back();
        if (texture.width_ != static_cast<std::int32_t>(tileset.image_width_) ||
            texture.height_ != static_cast<std::int32_t>(tileset.image_height_))
        {
            throw ResourceLoadError{"Tiled: image dimensions differ from the atlas declaration: '" +
                                    tileset.image_.string() + "'"};
        }
        texture.bind();
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    }

    // Six interleaved position/UV vertices per tile, uploaded only once.
    using Vertex = std::array<float, 5>;
    std::vector<Vertex>                           vertices{};
    constexpr std::array<std::uint32_t, 6>        corners{0U, 1U, 2U, 0U, 2U, 3U};
    constexpr std::array<std::array<float, 2>, 4> positions{
        {{0.0F, 0.0F}, {1.0F, 0.0F}, {1.0F, 1.0F}, {0.0F, 1.0F}}};
    for (const auto& layer : data_.layers_)
    {
        const auto first_batch = batches_.size();
        for (std::size_t index{0U}; index < layer.tiles_.size(); ++index)
        {
            const auto raw_gid = layer.tiles_[index];
            const auto gid     = raw_gid & 0x0FFFFFFFU;
            if (gid == 0U)
            {
                continue;
            }
            if (vertices.size() >
                    static_cast<std::size_t>(std::numeric_limits<std::int32_t>::max()) - 6U ||
                vertices.size() + 6U >
                    static_cast<std::size_t>(std::numeric_limits<GLsizeiptr>::max()) /
                        sizeof(Vertex))
            {
                throw ResourceLoadError{"Tiled: map geometry exceeds the OpenGL buffer limits"};
            }
            const auto next = std::upper_bound(data_.tilesets_.begin(), data_.tilesets_.end(), gid,
                                               [](std::uint32_t id, const Tileset& tileset)
                                               { return id < tileset.first_gid_; });
            const auto tileset_index = static_cast<std::size_t>(next - data_.tilesets_.begin() - 1);
            const auto& tileset      = data_.tilesets_[tileset_index];
            const auto  local_id     = gid - tileset.first_gid_;
            const auto  atlas_x =
                tileset.margin_ +
                (local_id % tileset.columns_) *
                    (static_cast<std::uint64_t>(data_.tile_width_) + tileset.spacing_);
            const auto atlas_y =
                tileset.margin_ +
                (local_id / tileset.columns_) *
                    (static_cast<std::uint64_t>(data_.tile_height_) + tileset.spacing_);
            const float x =
                static_cast<float>((index % data_.width_) * data_.tile_width_) + layer.offset_.x;
            float y =
                static_cast<float>((index / data_.width_) * data_.tile_height_) + layer.offset_.y;
            float width  = static_cast<float>(data_.tile_width_);
            float height = static_cast<float>(data_.tile_height_);
            if ((raw_gid & 0x20000000U) != 0U)
            {
                // Tiled swaps rectangular tile dimensions around the bottom-left anchor.
                std::swap(width, height);
                y += static_cast<float>(data_.tile_height_) - height;
            }

            // Batch only consecutive tiles so transparent tiles retain painter's order.
            if (batches_.size() == first_batch || batches_.back().tileset_ != tileset_index)
            {
                batches_.push_back({tileset_index, static_cast<std::int32_t>(vertices.size()), 0});
            }
            batches_.back().count_ += 6;
            for (const auto corner : corners)
            {
                const auto& position = positions[corner];
                float       u        = position[0];
                float       v        = position[1];
                // Inverse mapping: undo H/V, then the diagonal swap. This renders the
                // forward Tiled transform (diagonal first, followed by H/V).
                if ((raw_gid & 0x80000000U) != 0U)
                {
                    u = 1.0F - u;
                }
                if ((raw_gid & 0x40000000U) != 0U)
                {
                    v = 1.0F - v;
                }
                if ((raw_gid & 0x20000000U) != 0U)
                {
                    std::swap(u, v);
                }
                vertices.push_back(
                    {x + position[0] * width, y + position[1] * height, 0.0F,
                     (static_cast<float>(atlas_x) + u * data_.tile_width_) / tileset.image_width_,
                     (static_cast<float>(atlas_y) + v * data_.tile_height_) /
                         tileset.image_height_});
            }
        }
        layer_batches_.emplace_back(first_batch, batches_.size());
    }
    if (vertices.empty())
    {
        return;
    }
    GL_CALL(glGenVertexArrays(1, &vertex_array_));
    GL_CALL(glGenBuffers(1, &vertex_buffer_));
    GL_CALL(glBindVertexArray(vertex_array_));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)),
                         vertices.data(), GL_STATIC_DRAW));
    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr));
    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                  reinterpret_cast<const void*>(3U * sizeof(float))));
    GL_CALL(glEnableVertexAttribArray(1));
}

void TileMap::draw_layer(std::size_t index, const Shader shader)
{
    const auto& layer   = data_.layers_.at(index);
    auto*       context = RenderContext::get_active_context();
    if (context == nullptr || !layer.visible_ || layer.opacity_ == 0.0F)
    {
        return;
    }
    const auto range = layer_batches_[index];
    if (range.first == range.second)
    {
        return;
    }
    shader.use();
    context->update_mvp_matrix(glm::mat4{1.0F}, shader.get_id());
    shader.set_float("opacity", layer.opacity_);
    shader.set_int("the_texture", 0);
    GL_CALL(glBindVertexArray(vertex_array_));
    for (auto batch_index = range.first; batch_index < range.second; ++batch_index)
    {
        const auto& batch = batches_[batch_index];
        textures_[batch.tileset_].bind();
        GL_CALL(glDrawArrays(GL_TRIANGLES, batch.first_, batch.count_));
    }
}

TileMap::TileMap(const std::filesystem::path& file_name)
{
    if (RenderContext::get_active_context() == nullptr)
    {
        throw GraphicsError{"TileMap: an active render context is required"};
    }
    data_ = load_tiled_map(file_name);
    try
    {
        build();
    }
    catch (...)
    {
        release_gl_resources();
        throw;
    }
}

TileMap::~TileMap()
{
    release_gl_resources();
}

void TileMap::draw()
{
    if (auto* context = RenderContext::get_active_context())
    {
        draw(context->get_texture_default_shader());
    }
}

void TileMap::draw(const Shader shader)
{
    for (std::size_t index{0U}; index < data_.layers_.size(); ++index)
    {
        draw_layer(index, shader);
    }
}

void TileMap::draw_layer(std::size_t layer_index)
{
    static_cast<void>(data_.layers_.at(layer_index));
    if (auto* context = RenderContext::get_active_context())
    {
        draw_layer(layer_index, context->get_texture_default_shader());
    }
}

const TileMapData& TileMap::get_data() const
{
    return data_;
}

} // namespace rinvid
