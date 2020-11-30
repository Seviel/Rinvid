#ifndef CORE_INCLUDE_TRIANGLE_SHAPE_H
#define CORE_INCLUDE_TRIANGLE_SHAPE_H

#include "core/include/fixed_polygon_shape.h"
#include "core/include/shape.h"
#include "util/include/vector2.h"

namespace rinvid
{

class TriangleShape : public FixedPolygonShape<3>
{
  public:
    TriangleShape(Vector2 vert1, Vector2 vert2, Vector2 vert3);

    virtual void draw() override;
};

} // namespace rinvid

#endif // CORE_INCLUDE_TRIANGLE_SHAPE_H
