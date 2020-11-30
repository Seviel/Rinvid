#ifndef CORE_INCLUDE_RECTANGLE_SHAPE_H
#define CORE_INCLUDE_RECTANGLE_SHAPE_H

#include "core/include/fixed_polygon_shape.h"
#include "core/include/shape.h"
#include "util/include/vector2.h"

namespace rinvid
{

class RectangleShape : public FixedPolygonShape<4>
{
  public:
    RectangleShape(Vector2 vert1, Vector2 vert2, Vector2 vert3, Vector2 vert4);

    virtual void draw() override;
};

} // namespace rinvid

#endif // CORE_INCLUDE_RECTANGLE_SHAPE_H
