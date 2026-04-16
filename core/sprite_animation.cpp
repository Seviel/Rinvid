/**********************************************************************
 * Copyright (c) 2024 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <cstddef>
#include <utility>

#include "include/sprite_animation.h"

namespace rinvid
{

std::vector<Rect> SpriteAnimation::split_animation_frames(std::uint32_t width, std::uint32_t height,
                                                          std::uint32_t cols, std::uint32_t rows)
{
    const auto new_region_count = static_cast<std::size_t>(rows) * static_cast<std::size_t>(cols);
    regions_.reserve(regions_.size() + new_region_count);

    for (std::uint32_t i{0}; i < rows; ++i)
    {
        for (std::uint32_t j{0}; j < cols; ++j)
        {
            Rect rect{};
            rect.position.x = x_offset_ + (j * width);
            rect.position.y = y_offset_ + (i * height);
            rect.width      = width;
            rect.height     = height;

            regions_.push_back(rect);
        }
    }

    return regions_;
}

void SpriteAnimation::add_animation(std::string name, Animation animation)
{
    animations_.emplace(std::move(name), std::move(animation));
}

void SpriteAnimation::play(const std::string& name, bool reset)
{
    auto       animation      = animations_.find(name);
    Animation* next_animation = &(animation->second);

    is_active_ = true;
    if (current_animation_ == next_animation)
    {
        if (reset)
        {
            current_animation_->reset();
        }
    }
    else
    {
        current_animation_ = next_animation;
        current_animation_->reset();
    }
}

bool SpriteAnimation::is_animation_finished()
{
    if (is_active_)
    {
        return current_animation_->is_finished();
    }
    else
    {
        return true;
    }
}

void SpriteAnimation::set_animation_by_name(const std::string& name, Animation& animation)
{
    auto anim = animations_.find(name);
    if (anim != animations_.end())
    {
        anim->second = animation;
    }
}

} // namespace rinvid
