#include "include/triangle_shape.h"
#include "core/include/rinvid_gfx.h"
#include "util/include/vector3.h"

#define GL_GLEXT_PROTOTYPES

#include <SFML/OpenGL.hpp>

namespace rinvid
{

TriangleShape::TriangleShape(Vector3 vert1, Vector3 vert2, Vector3 vert3)
    : vert1_{vert1}, vert2_{vert2}, vert3_{vert3}, vertices{}
{
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void TriangleShape::convert_to_opengl_coordinates()
{
    std::uint32_t width  = RinvidGfx::get_width();
    std::uint32_t heigth = RinvidGfx::get_height();

    vertices.data[0] = ((vert1_.x / width) * 2.0F) - 1.0F;
    vertices.data[1] = ((vert1_.y / heigth) * 2.0F) - 1.0F;

    vertices.data[3] = ((vert2_.x / width) * 2.0F) - 1.0F;
    vertices.data[4] = ((vert2_.y / heigth) * 2.0F) - 1.0F;

    vertices.data[6] = ((vert3_.x / width) * 2.0F) - 1.0F;
    vertices.data[7] = ((vert3_.y / heigth) * 2.0F) - 1.0F;

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data, GL_DYNAMIC_DRAW);
}

void TriangleShape::draw()
{
    convert_to_opengl_coordinates();

    std::int32_t color_location = glGetUniformLocation(RinvidGfx::get_default_shader(), "in_color");
    glUniform4f(color_location, color_.r, color_.g, color_.b, color_.a);
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

} // namespace rinvid