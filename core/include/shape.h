#ifndef CORE_INCLUDE_SHAPE_H
#define CORE_INCLUDE_SHAPE_H

#include <cstdint>

namespace rinvid
{

class Shape
{
  public:
    virtual void draw() = 0;

  protected:
    std::uint32_t vao_;
    std::uint32_t vbo_;
};

} // namespace rinvid

#endif // CORE_INCLUDE_SHAPE_H