/**********************************************************************
 * Copyright (c) 2020, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_SHAPE_H
#define CORE_INCLUDE_SHAPE_H

#include <cstdint>

#include "util/include/color.h"
#include "util/include/vector2.h"

namespace rinvid
{

class Shape
{
  public:
    virtual void draw()                             = 0;
    virtual void move(const Vector2 move_vector)    = 0;
    virtual void set_position(const Vector2 vector) = 0;

    Vector2 get_origin();

    void set_color(const Color&& color);
    void set_color(const Color& color);

  protected:
    Vector2       origin_;
    Color         color_;
    std::uint32_t vao_;
    std::uint32_t vbo_;

  private:
    virtual void normalize_coordinates() = 0;
    virtual void calculate_origin()      = 0;
};

} // namespace rinvid

#endif // CORE_INCLUDE_SHAPE_H
