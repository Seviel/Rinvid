#include <algorithm>

#include "core/include/rinvid_gfx.h"
#include "include/rectangle_shape.h"
#include "util/include/vector3.h"

#define GL_GLEXT_PROTOTYPES

#include <SFML/OpenGL.hpp>

namespace rinvid
{

RectangleShape::RectangleShape(Vector2 vert1, Vector2 vert2, Vector2 vert3, Vector2 vert4)
{
    verts_.at(0) = vert1;
    verts_.at(1) = vert2;
    verts_.at(2) = vert3;
    verts_.at(3) = vert4;

    calculate_origin();

    init_vertex_buffer();
}

void RectangleShape::draw()
{
    convert_to_opengl_coordinates();
    std::int32_t color_location = glGetUniformLocation(RinvidGfx::get_default_shader(), "in_color");
    glUniform4f(color_location, color_.r, color_.g, color_.b, color_.a);
    glBindVertexArray(vao_);
    glDrawArrays(GL_QUADS, 0, 4);
    glBindVertexArray(0);
}

} // namespace rinvid
