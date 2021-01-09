/**********************************************************************
 * Copyright (c) 2021, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "include/sprite.h"
#include "include/texture.h"

namespace rinvid
{

Sprite::Sprite(Texture* texture, std::uint32_t width, std::uint32_t height, Vector2 top_left)
    : texture_{texture}, width_{width}, height_{height}, top_left_{top_left}
{
}

void Sprite::draw()
{
    texture_->update_vertices(top_left_, width_, height_);
    texture_->draw();
}

void Sprite::move(const Vector2 move_vector)
{
    top_left_.move(move_vector);
}

void Sprite::set_position(const Vector2 vector)
{
    top_left_.set(vector);
}

} // namespace rinvid
