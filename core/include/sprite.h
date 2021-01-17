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

#include "core/include/texture.h"
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
    Sprite(Texture* texture, std::uint32_t width, std::uint32_t height, Vector2 top_left,
           Vector2 texture_offset = {0.0F, 0.0F});

    /**************************************************************************************************
     * @brief Draws the sprite.
     *
     *************************************************************************************************/
    void draw();

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

  private:
    Texture*      texture_;
    std::uint32_t width_;
    std::uint32_t height_;
    Vector2       top_left_;
    Vector2       texture_offset_;
};

} // namespace rinvid

#endif // CORE_INCLUDE_SPRITE_H
