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

Animation::Animation(double framerate, std::vector<Quad> regions)
    : regions_{regions}, frame_time_{1.0 / framerate}, time_passed_{0.0}
{
}

Quad Animation::current_frame(double delta_time)
{
    if (!regions_.empty())
    {
        time_passed_ += delta_time;

        double frame_index{};
        frame_index = time_passed_ / frame_time_;

        return regions_.at(static_cast<std::uint32_t>(frame_index) % regions_.size());
    }
    else
    {
        return Quad{};
    }
}

} // namespace rinvid
