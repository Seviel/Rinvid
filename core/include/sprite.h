/**********************************************************************
 * Copyright (c) 2021 - 2024, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_SPRITE_H
#define CORE_INCLUDE_SPRITE_H

#include "core/include/drawable_animated.h"
#include "core/include/sprite_animation.h"
#include "core/include/texture.h"
#include "core/include/transformable.h"
#include "util/include/rect.h"
#include "util/include/vector2.h"

namespace rinvid
{

class Sprite : public Transformable, public DrawableAnimated
{
  public:
    /**************************************************************************************************
     * @brief Default constructor.
     *
     *************************************************************************************************/
    Sprite();

    /**************************************************************************************************
     * @brief Sprite constructor.
     *
     * @param texture sprite textures
     * @param width sprite width
     * @param height sprite height
     * @param top_left top left corner of sprite
     * @param texture_offset top left corner of texture region to use, since a texture can contain
     * multiuple sprites. Origin (0, 0) is top left point of the texture.
     *
     *************************************************************************************************/
    Sprite(Texture* texture, std::int32_t width, std::int32_t height, Vector2f top_left,
           Vector2f texture_offset = {0.0F, 0.0F});

    /**************************************************************************************************
     * @brief Draws the sprite. Use this function for sprites that are not animated.
     *
     *************************************************************************************************/
    void draw() override;

    /**************************************************************************************************
     * @brief Draw the sprite with shader.
     *
     * @param shader Shader to be used.
     *
     *************************************************************************************************/
    virtual void draw(const Shader shader) override;

    /**************************************************************************************************
     * @brief Draws the sprite.  Use this function for drawing animated sprites.
     *
     * @param delta_time Time passed in seconds since last frame
     *
     *************************************************************************************************/
    void draw(double delta_time) override;

    /**************************************************************************************************
     * @brief Draws the sprite with shader.  Use this function for drawing animated sprites.
     *
     * @param delta_time Time passed in seconds since last frame.
     * @param shader Shader to be used.
     *
     *************************************************************************************************/
    void draw(double delta_time, const Shader shader) override;

    /**************************************************************************************************
     * @brief Moves sprite by adding move_vector to its position vector.
     *
     * @param move_vector Vector to be added to sprite's position vector
     *
     *************************************************************************************************/
    void move(const Vector2f move_vector);

    /**************************************************************************************************
     * @brief Sets sprite's position to the position of passed vector.
     *
     * @param vector A new position vector of the sprite
     *
     *************************************************************************************************/
    void set_position(const Vector2f vector);

    /**************************************************************************************************
     * @brief Returns bounding box rect of the shape
     *
     * @return Bounding rect
     *
     *************************************************************************************************/
    Rect bounding_rect();

    /**************************************************************************************************
     * @brief Sets texture, position and size of sprite.
     *
     * @param texture sprite texture
     * @param width sprite width
     * @param height sprite height
     * @param top_left top left corner of sprite
     * @param texture_offset top left corner of texture region to use, (0, 0) of texture is top left
     *
     *************************************************************************************************/
    void setup(Texture* texture, std::int32_t width, std::int32_t height, Vector2f top_left,
               Vector2f texture_offset = {0.0F, 0.0F});

    /**************************************************************************************************
     * @brief Sets transparency level of sprite.
     *
     * @param opacity transparency level in range 0.0 - 1.0, 0.0 meaning sprite will be
     * completely invisible
     *
     *************************************************************************************************/
    void set_opacity(float opacity);

    /**************************************************************************************************
     * @brief Returns SpriteAnimation object.
     *
     * @return SpriteAnimation object.
     *
     *************************************************************************************************/
    SpriteAnimation& get_animation();

  protected:
    SpriteAnimation sprite_animation_;

  private:
    Texture*     texture_;
    std::int32_t width_;
    std::int32_t height_;
    Vector2f     top_left_;
    Vector2f     texture_offset_;
    float        opacity_;
};

} // namespace rinvid

#endif // CORE_INCLUDE_SPRITE_H
