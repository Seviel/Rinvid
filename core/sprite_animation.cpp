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

#include <rinvid/core/sprite_animation.h>
#include <rinvid/util/error_handler.h>

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

Animation* SpriteAnimation::get_animation_by_name(const std::string& name)
{
    auto animation = animations_.find(name);
    if (animation == animations_.end())
    {
        return nullptr;
    }

    return &animation->second;
}

const Animation* SpriteAnimation::get_animation_by_name(const std::string& name) const
{
    auto animation = animations_.find(name);
    if (animation == animations_.end())
    {
        return nullptr;
    }

    return &animation->second;
}

Animation* SpriteAnimation::get_current_animation()
{
    if (current_animation_name_.empty())
    {
        return nullptr;
    }

    return get_animation_by_name(current_animation_name_);
}

const Animation* SpriteAnimation::get_current_animation() const
{
    if (current_animation_name_.empty())
    {
        return nullptr;
    }

    return get_animation_by_name(current_animation_name_);
}

void SpriteAnimation::play(const std::string& name, bool reset)
{
    Animation* next_animation = get_animation_by_name(name);
    if (next_animation == nullptr)
    {
        errors::put_error_to_log("SpriteAnimation::play error: animation '" + name +
                                 "' does not exist");
        is_active_ = false;
        current_animation_name_.clear();
        return;
    }

    is_active_ = true;
    if (current_animation_name_ == name)
    {
        if (reset)
        {
            next_animation->reset();
        }
    }
    else
    {
        current_animation_name_ = name;
        next_animation->reset();
    }
}

bool SpriteAnimation::is_animation_finished()
{
    if (is_active_)
    {
        Animation* current_animation = get_current_animation();
        if (current_animation == nullptr)
        {
            is_active_ = false;
            current_animation_name_.clear();
            return true;
        }

        return current_animation->is_finished();
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
        return;
    }

    errors::put_error_to_log("SpriteAnimation::set_animation_by_name error: animation '" + name +
                             "' does not exist");
}

} // namespace rinvid
