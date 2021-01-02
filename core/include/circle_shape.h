/**********************************************************************
 * Copyright (c) 2020, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_CIRCLE_SHAPE_H
#define CORE_INCLUDE_CIRCLE_SHAPE_H

#include "core/include/fixed_polygon_shape.h"
#include "core/include/shape.h"
#include "util/include/vector2.h"

namespace rinvid
{

/**************************************************************************************************
 * @brief A drawable circle shape.
 *
 *************************************************************************************************/
class CircleShape : public FixedPolygonShape<180>
{
  public:
    /**************************************************************************************************
     * @brief CircleShape constructor.
     *
     * @param center center of the circle
     * @param radius radius of the circle
     *
     *************************************************************************************************/
    CircleShape(Vector2 center, float radius);

    /**************************************************************************************************
     * @brief Draws the circle.
     *
     *************************************************************************************************/
    virtual void draw() override;
};

} // namespace rinvid

#endif // CORE_INCLUDE_CIRCLE_SHAPE_H
