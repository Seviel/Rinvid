#ifndef CORE_INCLUDE_SHAPE_H
#define CORE_INCLUDE_SHAPE_H

#include <cstdint>

#include "util/include/color.h"

namespace rinvid
{

class Shape
{
  public:
    virtual void draw() = 0;
    void         set_color(const Color&& color);
    void         set_color(const Color& color);

  protected:
    Color         color_;
    std::uint32_t vao_;
    std::uint32_t vbo_;

  private:
    virtual void convert_to_opengl_coordinates() = 0;
};

} // namespace rinvid

#endif // CORE_INCLUDE_SHAPE_H