/**********************************************************************
 * Copyright (c) 2024, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "include/sprite_animation.h"

namespace rinvid
{

std::vector<Rect> SpriteAnimation::split_animation_frames(std::uint32_t width, std::uint32_t height,
                                                          std::uint32_t cols, std::uint32_t rows)
{
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
    animations_.insert(std::pair<std::string, Animation>(name, animation));
}

void SpriteAnimation::play(std::string name, bool reset)
{
    is_active_ = true;
    if (current_animation_ == &(animations_.find(name)->second))
    {
        if (reset)
        {
            current_animation_->reset();
        }
    }
    else
    {
        current_animation_ = &(animations_.find(name)->second);
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
