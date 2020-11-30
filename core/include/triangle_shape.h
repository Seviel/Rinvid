#ifndef CORE_INCLUDE_TRIANGLE_SHAPE_H
#define CORE_INCLUDE_TRIANGLE_SHAPE_H

#include "core/include/shape.h"
#include "util/include/vector2.h"

namespace rinvid
{

struct _triangle_vertices_t
{
    float data[9];
};

class TriangleShape : public Shape
{
  public:
    TriangleShape(Vector2 vert1, Vector2 vert2, Vector2 vert3);

    virtual void draw() override;
    virtual void move(const Vector2 move_vector) override;
    virtual void set_position(const Vector2 vector) override;

  private:
    virtual void convert_to_opengl_coordinates() override;
    virtual void calculate_origin() override;

    Vector2              vert1_;
    Vector2              vert2_;
    Vector2              vert3_;
    _triangle_vertices_t vertices;
};

} // namespace rinvid

#endif // CORE_INCLUDE_TRIANGLE_SHAPE_H
