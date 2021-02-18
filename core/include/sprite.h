/**********************************************************************
 * Copyright (c) 2021, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_SPRITE_H
#define CORE_INCLUDE_SPRITE_H

#include <map>
#include <string>
#include <vector>
#include <initializer_list>
#include <type_traits>

#include "core/include/animation.h"
#include "core/include/texture.h"
#include "util/include/rect.h"
#include "util/include/vector2.h"

namespace rinvid
{

class Sprite
{
  public:
    /**************************************************************************************************
     * @brief Sprite constructor.
     *
     * @param texture sprite textures
     * @param width sprite width
     * @param height sprite height
     * @param top_left top left corner of sprite
     * @param texture_offset top left corner of texture region to use, (0, 0) of texture is top left
     *
     *************************************************************************************************/
    Sprite(Texture* texture, std::int32_t width, std::int32_t height, Vector2 top_left,
           Vector2 texture_offset = {0.0F, 0.0F});

    /**************************************************************************************************
     * @brief Draws the sprite.
     *
     * @param delta_time Time passed in seconds since last frame
     *
     *************************************************************************************************/
    void draw(double delta_time = 0.0);

    /**************************************************************************************************
     * @brief Moves sprite by adding move_vector to its position vector.
     *
     * @param move_vector Vector to be added to sprite's position vector
     *
     *************************************************************************************************/
    void move(const Vector2 move_vector);

    /**************************************************************************************************
     * @brief Sets sprite's position to the position of passed vector.
     *
     * @param vector A new position vector of the sprite
     *
     *************************************************************************************************/
    void set_position(const Vector2 vector);

    /**************************************************************************************************
     * @brief Returns bounding box rect of the shape
     *
     * @return Bounding rect
     *
     *************************************************************************************************/
    Rect bounding_rect() const;

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
        static_assert(std::is_integral<T>::value == true);

        std::vector<Rect> regions{};

        for (auto index : region_indices)
        {
            regions.push_back(regions_.at(index));
        }

        return regions;
    }

    /**************************************************************************************************
     * @brief Adds animation to sprite. Sprite can have multiple animations (or none). Specific
     * animation can be played by calling play() method.
     *
     * @param name Name of added animation (i.e. "walking"). This name will be used when referencing
     * animation when calling play() method
     * @param animation Animation to be added
     *
     *************************************************************************************************/
    void add_animation(std::string name, Animation animation);

    /**************************************************************************************************
     * @brief Plays animation previously added to sprite.
     *
     * @param name Name of added animation. An animation with this name must be previously added to
     * sprite by calling add_animation()
     *
     *************************************************************************************************/
    void play(std::string name);

    /**************************************************************************************************
     * @brief Checks whether animation being currently played is finished, if animation is in Normal
     * mode, otherwise returns false. Returns true if sprite is not animated
     *
     * @return true if animation is finished, false otherwise
     *
     *************************************************************************************************/
    bool is_animation_finished();

  private:
    std::map<std::string, Animation> animations_;
    std::vector<Rect>                regions_;
    Animation*                       current_animation_;
    Texture*                         texture_;
    std::int32_t                     width_;
    std::int32_t                     height_;
    Vector2                          top_left_;
    Vector2                          texture_offset_;
    bool                             is_animated_;
};

} // namespace rinvid

#endif // CORE_INCLUDE_SPRITE_H
