/**********************************************************************
 * Copyright (c) 2020 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_CORE_CIRCLE_SHAPE_H
#define INCLUDE_RINVID_CORE_CIRCLE_SHAPE_H

#include <rinvid/core/fixed_polygon_shape.h>
#include <rinvid/core/shape.h>
#include <rinvid/util/vector2.h>

namespace rinvid
{

/**************************************************************************************************
 * @brief A drawable circle shape.
 *
 *************************************************************************************************/
class CircleShape final : public FixedPolygonShape<180>
{
  public:
    /**************************************************************************************************
     * @brief CircleShape constructor.
     *
     * @param center center of the circle
     * @param radius radius of the circle
     *
     *************************************************************************************************/
    CircleShape(Vector2f center, float radius);
};

} // namespace rinvid

#endif // INCLUDE_RINVID_CORE_CIRCLE_SHAPE_H
