/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "include/sprite_object.h"

namespace rinvid
{

SpriteObject::SpriteObject()
{
}

SpriteObject::SpriteObject(Texture* texture, std::int32_t width, std::int32_t height,
                           Vector2f top_left, Vector2f texture_offset)
    : Sprite(texture, width, height, top_left, texture_offset)
{
}

Rect SpriteObject::bounding_rect()
{
    return Sprite::bounding_rect();
}

} // namespace rinvid
