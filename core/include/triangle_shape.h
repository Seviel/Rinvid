#ifndef CORE_INCLUDE_TRIANGLE_SHAPE_H
#define CORE_INCLUDE_TRIANGLE_SHAPE_H

#include "core/include/shape.h"
#include "util/include/vector3.h"

namespace rinvid
{

struct _triangle_vertices_t
{
    float data[9];
};

class TriangleShape : public Shape
{
  public:
    TriangleShape(Vector3 vert1, Vector3 vert2, Vector3 vert3);

    virtual void draw() override;

  private:
    _triangle_vertices_t vertices;
};

} // namespace rinvid

#endif // CORE_INCLUDE_TRIANGLE_SHAPE_H