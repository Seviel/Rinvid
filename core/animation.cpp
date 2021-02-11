/**********************************************************************
 * Copyright (c) 2021, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "include/animation.h"

namespace rinvid
{

Animation::Animation(double framerate, std::vector<Quad> regions, AnimationMode mode)
    : regions_{regions}, frame_time_{1.0 / framerate}, time_passed_{0.0}, mode_{mode}
{
}

Quad Animation::current_frame(double delta_time)
{
    if (!regions_.empty())
    {
        time_passed_ += delta_time;

        double frame_index{};
        frame_index = time_passed_ / frame_time_;

        switch (mode_)
        {
            case AnimationMode::Looping:
                return regions_.at(static_cast<std::uint32_t>(frame_index) % regions_.size());
                break;
            case AnimationMode::Normal:
            default:
                std::uint32_t number_of_regions = regions_.size();
                Quad          region{};
                static_cast<std::uint32_t>(frame_index) > number_of_regions - 1
                    ? region = regions_.at(number_of_regions - 1)
                    : region = regions_.at(static_cast<std::uint32_t>(frame_index));
                return region;
                break;
        }
    }
    else
    {
        return Quad{};
    }
}

void Animation::reset()
{
    time_passed_ = 0.0;
}

} // namespace rinvid
