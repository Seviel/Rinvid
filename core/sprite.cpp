/**********************************************************************
 * Copyright (c) 2021 - 2024, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "include/sprite.h"

namespace rinvid
{

Sprite::Sprite()
    : sprite_animation_{}, texture_{nullptr}, width_{0}, height_{0}, top_left_{0.0F, 0.0F},
      texture_offset_{0.0F, 0.0F}, opacity_{1.0F}
{
}

Sprite::Sprite(Texture* texture, std::int32_t width, std::int32_t height, Vector2f top_left,
               Vector2f texture_offset)
    : sprite_animation_{}, texture_{texture}, width_{width}, height_{height}, top_left_{top_left},
      texture_offset_{texture_offset}, opacity_{1.0F}
{
    texture_->update_vertices(texture_offset_, width_, height_);
}

void Sprite::draw()
{
    draw(0.0);
}

void Sprite::draw(const Shader shader)
{
    draw(0.0, shader);
}

void Sprite::draw(double delta_time)
{
    const auto shader = RinvidGfx::get_texture_default_shader();
    draw(delta_time, shader);
}

void Sprite::draw(double delta_time, const Shader shader)
{
    origin_.x = top_left_.x + width_ / 2;
    origin_.y = top_left_.y + height_ / 2;

    if (sprite_animation_.is_active_)
    {
        sprite_animation_.current_animation_->advance(delta_time);
        Rect          texture_region = sprite_animation_.current_animation_->frame();
        Vector2f      offset{texture_offset_};
        std::uint32_t width  = texture_region.width;
        std::uint32_t height = texture_region.height;

        offset.x += texture_region.position.x;
        offset.y += texture_region.position.y;

        texture_->update_vertices(offset, width, height);
    }

    texture_->draw(get_transform(), shader, opacity_);
}

void Sprite::move(const Vector2f move_vector)
{
    top_left_.move(move_vector);
}

void Sprite::set_position(const Vector2f vector)
{
    top_left_.set(vector);
}

Rect Sprite::bounding_rect()
{
    if (is_transformed() == false)
    {
        return Rect{top_left_, width_, height_};
    }

    Rect                   rect{};
    std::vector<glm::vec4> glm_vertices{};

    const auto& transform = get_transform();

    glm::vec4 vertex{top_left_.x, top_left_.y, 1.0F, 1.0F};
    vertex = transform * vertex;
    glm_vertices.push_back(vertex);

    vertex = glm::vec4{top_left_.x + width_, top_left_.y, 1.0F, 1.0F};
    vertex = transform * vertex;
    glm_vertices.push_back(vertex);

    vertex = glm::vec4{top_left_.x + width_, top_left_.y + height_, 1.0F, 1.0F};
    vertex = transform * vertex;
    glm_vertices.push_back(vertex);

    vertex = glm::vec4{top_left_.x, top_left_.y + height_, 1.0F, 1.0F};
    vertex = transform * vertex;
    glm_vertices.push_back(vertex);

    float min_x{};
    float max_x{};
    float min_y{};
    float max_y{};

    set_min_max_coords(glm_vertices, min_x, max_x, min_y, max_y);

    rect.position.x = min_x;
    rect.position.y = min_y;
    rect.width      = max_x - min_x;
    rect.height     = max_y - min_y;

    return rect;
}

void Sprite::setup(Texture* texture, std::int32_t width, std::int32_t height, Vector2f top_left,
                   Vector2f texture_offset)
{
    texture_        = texture;
    width_          = width;
    height_         = height;
    top_left_       = top_left;
    texture_offset_ = texture_offset;

    texture_->update_vertices(texture_offset_, width_, height_);
}

void Sprite::set_opacity(float transparency)
{
    opacity_ = transparency;
}

SpriteAnimation& Sprite::get_animation()
{
    return sprite_animation_;
}

} // namespace rinvid
