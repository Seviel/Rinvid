/**********************************************************************
 * Copyright (c) 2021, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_ANIMATION_H
#define CORE_INCLUDE_ANIMATION_H

#include <string>
#include <vector>

#include "util/include/quad.h"

namespace rinvid
{

enum class AnimationMode
{
    Normal = 0U,
    Looping
};

class Animation
{
  public:
    /**************************************************************************************************
     * @brief Animation constructor.
     *
     * @param framerate Speed of animation in frames per second
     * @param regions Vector of quads where each quad represents a region of texture that represents
     * a single animation frame
     * @param mode Which mode should animation play in (normal or looping)
     *
     *************************************************************************************************/
    Animation(double framerate, std::vector<Quad> regions,
              AnimationMode mode = AnimationMode::Normal);

    /**************************************************************************************************
     * @brief Returns which region (frame) should currently be displayed based on time passed
     *
     * @param delta_time Time passed since last frame in seconds
     *
     *************************************************************************************************/
    Quad current_frame(double delta_time);

    /**************************************************************************************************
     * @brief Resets animation back to first frame
     *
     *************************************************************************************************/
    void reset();

  private:
    std::vector<Quad> regions_;
    double            frame_time_;
    double            time_passed_;
    AnimationMode     mode_;
};

} // namespace rinvid

#endif // CORE_INCLUDE_ANIMATION_H
