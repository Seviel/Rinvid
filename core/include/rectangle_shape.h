/**********************************************************************
 * Copyright (c) 2020 - 2023, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_RECTANGLE_SHAPE_H
#define CORE_INCLUDE_RECTANGLE_SHAPE_H

#include <rinvid/core/fixed_polygon_shape.h>
#include <rinvid/core/shape.h>
#include <rinvid/util/vector2.h>

namespace rinvid
{

/**************************************************************************************************
 * @brief A drawable rectangle shape.
 *
 *************************************************************************************************/
class RectangleShape final : public FixedPolygonShape<4, GL_QUADS>
{
  public:
    /**************************************************************************************************
     * @brief RectangleShape constructor.
     *
     * @param center center point of rectangle
     * @param width rectangle width
     * @param height rectangle height
     *
     *************************************************************************************************/
    RectangleShape(Vector2f center, float width, float height);
};

} // namespace rinvid

#endif // CORE_INCLUDE_RECTANGLE_SHAPE_H
