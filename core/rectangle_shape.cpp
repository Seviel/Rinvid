#include <algorithm>

#include "core/include/rinvid_gfx.h"
#include "include/rectangle_shape.h"
#include "util/include/vector3.h"

#define GL_GLEXT_PROTOTYPES

#include <SFML/OpenGL.hpp>

namespace rinvid
{

RectangleShape::RectangleShape(Vector2 vert1, Vector2 vert2, Vector2 vert3, Vector2 vert4)
    : vert1_{vert1}, vert2_{vert2}, vert3_{vert3}, vert4_{vert4}, vertices_{}
{
    calculate_origin();

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_.data, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void RectangleShape::convert_to_opengl_coordinates()
{
    vertices_.data[0] = RinvidGfx::get_opengl_x_coord(vert1_.x);
    vertices_.data[1] = RinvidGfx::get_opengl_y_coord(vert1_.y);

    vertices_.data[3] = RinvidGfx::get_opengl_x_coord(vert2_.x);
    vertices_.data[4] = RinvidGfx::get_opengl_y_coord(vert2_.y);

    vertices_.data[6] = RinvidGfx::get_opengl_x_coord(vert3_.x);
    vertices_.data[7] = RinvidGfx::get_opengl_y_coord(vert3_.y);

    vertices_.data[9]  = RinvidGfx::get_opengl_x_coord(vert4_.x);
    vertices_.data[10] = RinvidGfx::get_opengl_y_coord(vert4_.y);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_.data, GL_DYNAMIC_DRAW);
}

void RectangleShape::calculate_origin()
{
    float min_x = std::min(std::min(vert1_.x, vert2_.x), std::min(vert3_.x, vert4_.x));
    float min_y = std::min(std::min(vert1_.y, vert2_.y), std::min(vert3_.y, vert4_.y));

    float max_x = std::max(std::max(vert1_.x, vert2_.x), std::max(vert3_.x, vert4_.x));
    float max_y = std::max(std::max(vert1_.y, vert2_.y), std::max(vert3_.y, vert4_.y));

    origin_.x = (max_x + min_x) / 2.0F;
    origin_.y = (max_y + min_y) / 2.0F;
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

void RectangleShape::move(const Vector2 move_vector)
{
    vert1_.move(move_vector);
    vert2_.move(move_vector);
    vert3_.move(move_vector);
    vert4_.move(move_vector);

    calculate_origin();
}

void RectangleShape::set_position(const Vector2 vector)
{
    Vector2 move_vector{};

    move_vector.x = vector.x - origin_.x;
    move_vector.y = vector.y - origin_.y;

    vert1_.move(move_vector);
    vert2_.move(move_vector);
    vert3_.move(move_vector);
    vert4_.move(move_vector);

    origin_ = vector;
}

} // namespace rinvid