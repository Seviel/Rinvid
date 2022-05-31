/**********************************************************************
 * Copyright (c) 2021 - 2022, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "include/sprite.h"
#include "include/texture.h"

namespace rinvid
{

Sprite::Sprite()
    : animations_{}, regions_{}, current_animation_{nullptr}, texture_{nullptr}, width_{},
      height_{}, top_left_{}, texture_offset_{}, is_animated_{false}
{
}

Sprite::Sprite(Texture* texture, std::int32_t width, std::int32_t height, Vector2<float> top_left,
               Vector2<float> texture_offset)
    : animations_{}, regions_{}, current_animation_{nullptr}, texture_{texture}, width_{width},
      height_{height}, top_left_{top_left}, texture_offset_{texture_offset}, is_animated_{false}
{
}

void Sprite::draw()
{
    draw(0.0);
}

void Sprite::draw(double delta_time)
{
    origin_.x = top_left_.x + width_ / 2;
    origin_.y = top_left_.y + height_ / 2;

    if (is_animated_)
    {
        current_animation_->advance(delta_time);
        Rect           texture_region = current_animation_->frame();
        Vector2<float> offset{texture_offset_};
        std::uint32_t  width  = texture_region.width;
        std::uint32_t  height = texture_region.height;

        offset.x += texture_region.position.x;
        offset.y += texture_region.position.y;

        texture_->update_vertices(top_left_, offset, width, height);
    }
    else
    {
        texture_->update_vertices(top_left_, texture_offset_, width_, height_);
    }

    texture_->draw(get_transform());
}

void Sprite::move(const Vector2<float> move_vector)
{
    top_left_.move(move_vector);
}

void Sprite::set_position(const Vector2<float> vector)
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

std::vector<Rect> Sprite::split_animation_frames(std::uint32_t width, std::uint32_t height,
                                                 std::uint32_t cols, std::uint32_t rows)
{
    for (std::uint32_t i{0}; i < rows; ++i)
    {
        for (std::uint32_t j{0}; j < cols; ++j)
        {
            Rect rect{};
            rect.position.x = texture_offset_.x + (j * width);
            rect.position.y = texture_offset_.y + (i * height);
            rect.width      = width;
            rect.height     = height;

            regions_.push_back(rect);
        }
    }

    return regions_;
}

void Sprite::add_animation(std::string name, Animation animation)
{
    animations_.insert(std::pair<std::string, Animation>(name, animation));
}

void Sprite::play(std::string name)
{
    is_animated_       = true;
    current_animation_ = &(animations_.find(name)->second);
    current_animation_->reset();
}

bool Sprite::is_animation_finished()
{
    if (is_animated_)
    {
        return current_animation_->is_finished();
    }
    else
    {
        return true;
    }
}

void Sprite::setup(Texture* texture, std::int32_t width, std::int32_t height,
                   Vector2<float> top_left, Vector2<float> texture_offset)
{
    texture_        = texture;
    width_          = width;
    height_         = height;
    top_left_       = top_left;
    texture_offset_ = texture_offset;
}

} // namespace rinvid
