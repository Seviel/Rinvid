#include "include/triangle_shape.h"
#include "core/include/rinvid_gfx.h"
#include "util/include/vector3.h"

#define GL_GLEXT_PROTOTYPES

#include <SFML/OpenGL.hpp>

namespace rinvid
{

TriangleShape::TriangleShape(Vector2 vert1, Vector2 vert2, Vector2 vert3)
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
    vertices.data[0] = RinvidGfx::get_opengl_x_coord(vert1_.x);
    vertices.data[1] = RinvidGfx::get_opengl_y_coord(vert1_.y);

    vertices.data[3] = RinvidGfx::get_opengl_x_coord(vert2_.x);
    vertices.data[4] = RinvidGfx::get_opengl_y_coord(vert2_.y);

    vertices.data[6] = RinvidGfx::get_opengl_x_coord(vert3_.x);
    vertices.data[7] = RinvidGfx::get_opengl_y_coord(vert3_.y);

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

void TriangleShape::move(const Vector2 move_vector)
{
    vert1_.move(move_vector);
    vert2_.move(move_vector);
    vert3_.move(move_vector);
}

} // namespace rinvid