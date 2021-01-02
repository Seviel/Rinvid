/**********************************************************************
 * Copyright (c) 2020, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_TRIANGLE_SHAPE_H
#define CORE_INCLUDE_TRIANGLE_SHAPE_H

#include "core/include/fixed_polygon_shape.h"
#include "core/include/shape.h"
#include "util/include/vector2.h"

namespace rinvid
{

/**************************************************************************************************
 * @brief A drawable triangle shape.
 *
 *************************************************************************************************/
class TriangleShape : public FixedPolygonShape<3>
{
  public:
    /**************************************************************************************************
     * @brief TriangleShape constructor.
     *
     * @param vert1 vertex one
     * @param vert2 vertex two
     * @param vert3 vertex three
     *
     *************************************************************************************************/
    TriangleShape(Vector2 vert1, Vector2 vert2, Vector2 vert3);

    /**************************************************************************************************
     * @brief Draws the triangle.
     *
     *************************************************************************************************/
    virtual void draw() override;
};

} // namespace rinvid

#endif // CORE_INCLUDE_TRIANGLE_SHAPE_H
