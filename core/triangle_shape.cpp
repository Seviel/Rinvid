#include <algorithm>

#include "core/include/rinvid_gfx.h"
#include "include/triangle_shape.h"
#include "util/include/vector3.h"

#define GL_GLEXT_PROTOTYPES

#include <SFML/OpenGL.hpp>

namespace rinvid
{

TriangleShape::TriangleShape(Vector2 vert1, Vector2 vert2, Vector2 vert3)
    : vert1_{vert1}, vert2_{vert2}, vert3_{vert3}, vertices{}
{
    calculate_origin();

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

void TriangleShape::calculate_origin()
{
    float min_x = std::min(vert1_.x, std::min(vert2_.x, vert3_.x));
    float min_y = std::min(vert1_.y, std::min(vert2_.y, vert3_.y));

    float max_x = std::max(vert1_.x, std::max(vert2_.x, vert3_.x));
    float max_y = std::max(vert1_.y, std::max(vert2_.y, vert3_.y));

    origin_.x = (max_x + min_x) / 2.0F;
    origin_.y = (max_y + min_y) / 2.0F;
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

    calculate_origin();
}

void TriangleShape::set_position(const Vector2 vector)
{
    Vector2 move_vector{};

    move_vector.x = vector.x - origin_.x;
    move_vector.y = vector.y - origin_.y;

    vert1_.move(move_vector);
    vert2_.move(move_vector);
    vert3_.move(move_vector);

    origin_ = vector;
}

} // namespace rinvid