#include "include/shape.h"
#include "util/include/color.h"

// #define GL_GLEXT_PROTOTYPES

// #include <SFML/OpenGL.hpp>

namespace rinvid
{

Vector2 Shape::get_origin()
{
    return origin_;
}

void Shape::set_color(const Color&& color)
{
    color_ = color;
}

void Shape::set_color(const Color& color)
{
    color_ = color;
}

} // namespace rinvid