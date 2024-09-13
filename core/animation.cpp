/**********************************************************************
 * Copyright (c) 2021 - 2022, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <limits>

#include "include/animation.h"

namespace rinvid
{

Animation::Animation()
    : regions_{}, frame_time_{1.0 / 60}, time_passed_{0.0}, mode_{AnimationMode::Normal}
{
}

Animation::Animation(double framerate, std::vector<Rect> regions, AnimationMode mode)
    : regions_{regions},
      frame_time_{framerate != 0.0 ? (1.0 / framerate) : std::numeric_limits<double>::max()},
      time_passed_{0.0}, mode_{mode}
{
}

void Animation::setup(double framerate, const std::vector<Rect>& regions, AnimationMode mode)
{
    frame_time_ = framerate != 0.0 ? (1.0 / framerate) : std::numeric_limits<double>::max();
    regions_    = regions;
    mode_       = mode;
}

void Animation::advance(double delta_time)
{
    time_passed_ += delta_time;
}

Rect Animation::frame(double delta_time) const
{
    if (!regions_.empty())
    {
        return regions_.at(frame_index(delta_time));
    }
    else
    {
        return Rect{};
    }
}

std::uint32_t Animation::frame_index(double delta_time) const
{
    double        time_passed       = time_passed_ + delta_time;
    double        index             = time_passed / frame_time_;
    std::uint32_t number_of_regions = regions_.size();

    if (number_of_regions == 0)
    {
        return 0;
    }

    switch (mode_)
    {
        case AnimationMode::Looping:
            return static_cast<std::uint32_t>(index) % number_of_regions;
            break;
        case AnimationMode::Normal:
        default:
            std::uint32_t frame_index{};
            static_cast<std::uint32_t>(index) > number_of_regions - 1
                ? frame_index = number_of_regions - 1
                : frame_index = static_cast<std::uint32_t>(index);
            return frame_index;
            break;
    }
}

bool Animation::is_finished()
{
    switch (mode_)
    {
        case AnimationMode::Looping:
            return false;
            break;
        case AnimationMode::Normal:
        default:
            return frame_index() == (regions_.size() - 1);
            break;
    }
}

void Animation::reset()
{
    time_passed_ = 0.0;
}

} // namespace rinvid
