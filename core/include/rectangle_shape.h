#ifndef CORE_INCLUDE_RECTANGLE_SHAPE_H
#define CORE_INCLUDE_RECTANGLE_SHAPE_H

#include "core/include/shape.h"
#include "util/include/vector2.h"

namespace rinvid
{

struct _rectangle_vertices_t
{
    float data[12];
};

class RectangleShape : public Shape
{
  public:
    RectangleShape(Vector2 vert1, Vector2 vert2, Vector2 vert3, Vector2 vert4);

    virtual void draw() override;
    virtual void move(const Vector2 move_vector) override;
    virtual void set_position(const Vector2 vector) override;

  private:
    virtual void convert_to_opengl_coordinates() override;
    virtual void calculate_origin() override;

    Vector2               vert1_;
    Vector2               vert2_;
    Vector2               vert3_;
    Vector2               vert4_;
    _rectangle_vertices_t vertices_;
};

} // namespace rinvid

#endif // CORE_INCLUDE_RECTANGLE_SHAPE_H