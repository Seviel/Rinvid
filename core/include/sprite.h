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
    Sprite(Texture* texture, std::uint32_t width, std::uint32_t height, Vector2 top_left);

    void draw();

    void move(const Vector2 move_vector);

    void set_position(const Vector2 vector);

  private:
    Texture*      texture_;
    std::uint32_t width_;
    std::uint32_t height_;
    Vector2       top_left_;
};

} // namespace rinvid

#endif // CORE_INCLUDE_SPRITE_H
