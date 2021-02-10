/**********************************************************************
 * Copyright (c) 2021, Filip Vasiljevic
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

Sprite::Sprite(Texture* texture, std::uint32_t width, std::uint32_t height, Vector2 top_left,
               Vector2 texture_offset)
    : animations_{}, regions_{}, current_animation_{nullptr}, texture_{texture}, width_{width},
      height_{height}, top_left_{top_left}, texture_offset_{texture_offset}, is_animated_{false}
{
}

void Sprite::draw(double delta_time)
{
    if (is_animated_)
    {
        Quad          texture_region = current_animation_->current_frame(delta_time);
        Vector2       offset{texture_offset_};
        std::uint32_t width  = texture_region.width;
        std::uint32_t height = texture_region.height;

        offset.x += texture_region.x;
        offset.y += texture_region.y;

        texture_->update_vertices(top_left_, offset, width, height);
    }
    else
    {
        texture_->update_vertices(top_left_, texture_offset_, width_, height_);
    }

    texture_->draw();
}

void Sprite::move(const Vector2 move_vector)
{
    top_left_.move(move_vector);
}

void Sprite::set_position(const Vector2 vector)
{
    top_left_.set(vector);
}

std::vector<Quad> Sprite::split_animation_frames(std::uint32_t width, std::uint32_t height,
                                                 std::uint32_t cols, std::uint32_t rows)
{
    for (std::uint32_t i{0}; i < cols; ++i)
    {
        for (std::uint32_t j{0}; j < rows; ++j)
        {
            Quad quad{};
            quad.x      = texture_offset_.x + (i * width);
            quad.y      = texture_offset_.y + (j * height);
            quad.width  = width;
            quad.height = height;

            regions_.push_back(quad);
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
}

} // namespace rinvid
