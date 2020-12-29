#ifndef CORE_INCLUDE_QUAD_SHAPE_H
#define CORE_INCLUDE_QUAD_SHAPE_H

#include "core/include/fixed_polygon_shape.h"
#include "core/include/shape.h"
#include "util/include/vector2.h"

namespace rinvid
{

class QuadShape : public FixedPolygonShape<4>
{
  public:
    QuadShape(Vector2 top_left, Vector2 top_right, Vector2 bottom_right, Vector2 bottom_left);

    virtual void draw() override;
};

} // namespace rinvid

#endif // CORE_INCLUDE_QUAD_SHAPE_H
