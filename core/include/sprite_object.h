/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_SPRITE_OBJECT_H
#define CORE_INCLUDE_SPRITE_OBJECT_H

#include "core/include/object.h"
#include "core/include/sprite.h"
#include "core/include/texture.h"
#include "util/include/vector2.h"

namespace rinvid
{

/// @brief Class that combines sprite (drawable and transformable) and object (has physics
/// properties), used to represent physical entities in the world.
class SpriteObject : public Sprite, public Object
{
  public:
    /**************************************************************************************************
     * @brief Default constructor.
     *
     *************************************************************************************************/
    SpriteObject();

    virtual ~SpriteObject()
    {
    }

    /**************************************************************************************************
     * @brief SpriteObject constructor.
     *
     * @param texture sprite textures
     * @param width sprite width
     * @param height sprite height
     * @param top_left top left corner of sprite
     * @param texture_offset top left corner of texture region to use, since a texture can contain
     * multiuple sprites. Origin (0, 0) is top left point of the texture.
     *
     *************************************************************************************************/
    SpriteObject(Texture* texture, std::int32_t width, std::int32_t height, Vector2f top_left,
                 Vector2f texture_offset = {0.0F, 0.0F});

    /**************************************************************************************************
     * @brief Returns bounding box rect of the object.
     *
     * @return Bounding rect
     *
     *************************************************************************************************/
    Rect bounding_rect() override;
};

} // namespace rinvid

#endif // CORE_INCLUDE_SPRITE_OBJECT_H
