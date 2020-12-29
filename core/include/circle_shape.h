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

class CircleShape : public FixedPolygonShape<180>
{
  public:
    CircleShape(Vector2 center, float radius);

    virtual void draw() override;
};

} // namespace rinvid

#endif // CORE_INCLUDE_CIRCLE_SHAPE_H
