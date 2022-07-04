/**********************************************************************
 * Copyright (c) 2021 - 2022, Filip Vasiljevic
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

#include "util/include/rect.h"

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
     * @brief Default constructor.
     *
     *************************************************************************************************/
    Animation();

    /**************************************************************************************************
     * @brief Animation constructor.
     *
     * @param framerate Speed of animation in frames per second
     * @param regions Vector of rects where each rect represents a region of texture that represents
     * a single animation frame
     * @param mode Which mode should animation play in (normal or looping)
     *
     *************************************************************************************************/
    Animation(double framerate, std::vector<Rect> regions,
              AnimationMode mode = AnimationMode::Normal);

    /**************************************************************************************************
     * @brief Sets up animation framerate, regions and mode.
     *
     * @param framerate Speed of animation in frames per second
     * @param regions Vector of rects where each rect represents a region of texture that represents
     * a single animation frame
     * @param mode Which mode should animation play in (normal or looping)
     *
     *************************************************************************************************/
    void setup(double framerate, const std::vector<Rect>& regions,
               AnimationMode mode = AnimationMode::Normal);

    /**************************************************************************************************
     * @brief Causes animation to advance based on time. It should be called once each frame
     *
     * @param delta_time Time passed since last frame in seconds
     *
     *************************************************************************************************/
    void advance(double delta_time);

    /**************************************************************************************************
     * @brief Returns which region (frame) should currently be displayed based on time passed
     *
     * @param delta_time Leave default argument if you want to get current frame, pass 'delta_time'
     * if you want to get frame which should be displayed as if 'delta_time' passed
     *
     *************************************************************************************************/
    Rect frame(double delta_time = 0.0) const;

    /**************************************************************************************************
     * @brief Returns index of current region (frame) based on time passed
     *
     * @param delta_time Leave default argument if you want to get current frame index, pass
     * 'delta_time' if you want to get frame index as if 'delta_time' passed
     *
     *************************************************************************************************/
    std::uint32_t frame_index(double delta_time = 0.0) const;

    /**************************************************************************************************
     * @brief Checks whether animation is finished if animation is in Normal mode, otherwise returns
     * false
     *
     * @return true if animation is finished, false otherwise
     *
     *************************************************************************************************/
    bool is_finished();

    /**************************************************************************************************
     * @brief Resets animation back to first frame
     *
     *************************************************************************************************/
    void reset();

  private:
    std::vector<Rect> regions_;
    double            frame_time_;
    double            time_passed_;
    AnimationMode     mode_;
};

} // namespace rinvid

#endif // CORE_INCLUDE_ANIMATION_H
