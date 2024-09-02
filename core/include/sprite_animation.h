/**********************************************************************
 * Copyright (c) 2024, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_SPRITE_ANIMATION_H
#define CORE_INCLUDE_SPRITE_ANIMATION_H

#include <map>
#include <string>
#include <vector>

#include "core/include/animation.h"

namespace gui
{
class Button;
}

namespace rinvid
{

class SpriteAnimation
{
    friend class Sprite;
    friend class Button;

  public:
    /**************************************************************************************************
     * @brief Default constructor.
     *
     *************************************************************************************************/
    SpriteAnimation()
        : regions_{}, current_animation_{nullptr}, x_offset_{0.0F}, y_offset_{0.0F},
          is_active_{false} {};

    /**************************************************************************************************
     * @brief Constructor.
     *
     * @param x_offset Offset on x axis when calculating regions.
     * @param y_offset Offset on y axis when calculating regions.
     *
     *************************************************************************************************/
    SpriteAnimation(float x_offset, float y_offset)
        : regions_{}, current_animation_{nullptr}, x_offset_{x_offset}, y_offset_{y_offset},
          is_active_{false} {};

    /**************************************************************************************************
     * @brief Creates a vector of rects representing texture regions (or animation frames)
     *
     * @param width Region width
     * @param height Region height
     * @param cols How many columns does the sprite sheet have
     * @param rows How many rows does the sprite sheet have
     *
     *************************************************************************************************/
    std::vector<Rect> split_animation_frames(std::uint32_t width, std::uint32_t height,
                                             std::uint32_t cols, std::uint32_t rows);

    /**************************************************************************************************
     * @brief Fetches a subset of regions that are created after calling split_animation_frames().
     * This may be handy when creating animations. It is users responsibility to make sure
     * split_animation_frames() is called before this function and that indices in range are
     * provided
     *
     * @param region_indices Indices of wanted regions. Indices are assinged during
     * split_animation_frames() in left to right, top to bottom fashion
     *
     * @return Vector of desired regions
     *
     *************************************************************************************************/
    template <typename T>
    std::vector<Rect> get_regions(std::initializer_list<T> region_indices)
    {
        static_assert(std::is_integral<T>::value == true, "T must be of integral type");

        std::vector<Rect> regions{};

        for (auto index : region_indices)
        {
            regions.push_back(regions_.at(index));
        }

        return regions;
    }

    /**************************************************************************************************
     * @brief Adds animation. There can be multiple animations (or none). Specific animation can be
     * played by calling play() method.
     *
     * @param name Name of added animation (i.e. "walking"). This name will be used when referencing
     * animation when calling play() method.
     * @param animation Animation to be added.
     *
     *************************************************************************************************/
    void add_animation(std::string name, Animation animation);

    /**************************************************************************************************
     * @brief Plays previously added animation.
     *
     * @param name Name of added animation. An animation with this name must be previously added by
     * calling add_animation().
     * @param reset If the requested animation is already playing, it will play from the beginning
     * if this is set to true.
     *
     *************************************************************************************************/
    void play(std::string name, bool reset = false);

    /**************************************************************************************************
     * @brief Checks whether animation being currently played is finished, if animation is in Normal
     * mode, otherwise returns false. Returns true if sprite is not animated
     *
     * @return true if animation is finished, false otherwise
     *
     *************************************************************************************************/
    bool is_animation_finished();

    /**************************************************************************************************
     * @brief Replaces existing animation of given name with new one.
     *
     * @param name Name of animation to replace.
     * @param animation New animation.
     *
     *
     *************************************************************************************************/
    void set_animation_by_name(const std::string& name, Animation& animation);

  protected:
    std::map<std::string, Animation> animations_;

  private:
    std::vector<Rect> regions_;
    Animation*        current_animation_;
    float             x_offset_;
    float             y_offset_;
    bool              is_active_;
};

} // namespace rinvid

#endif // CORE_INCLUDE_SPRITE_ANIMATION_H
