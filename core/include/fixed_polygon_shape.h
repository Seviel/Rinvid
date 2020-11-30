#ifndef CORE_INCLUDE_FIXED_POLYGON_SHAPE_H
#define CORE_INCLUDE_FIXED_POLYGON_SHAPE_H

#include <vector>

#include "core/include/rinvid_gfx.h"
#include "core/include/shape.h"
#include "util/include/vector2.h"

namespace rinvid
{

template <typename std::uint32_t number_of_vertices>
class FixedPolygonShape : public Shape
{
  public:
    FixedPolygonShape();
    virtual void draw() = 0;
    virtual void move(const Vector2 move_vector) override;
    virtual void set_position(const Vector2 vector) override;

  protected:
    virtual void convert_to_opengl_coordinates() override;
    virtual void calculate_origin() override;
    void         init_vertex_buffer();

    std::vector<Vector2> verts_;
    float                vertices_[number_of_vertices * 3];
};

template <typename std::uint32_t number_of_vertices>
FixedPolygonShape<number_of_vertices>::FixedPolygonShape() : verts_{}, vertices_{}
{
    for (std::uint32_t i{0}; i < number_of_vertices; ++i)
    {
        verts_.emplace_back();
    }
}

template <typename std::uint32_t number_of_vertices>
void FixedPolygonShape<number_of_vertices>::convert_to_opengl_coordinates()
{
    for (std::uint32_t i{0}; i < number_of_vertices; ++i)
    {
        vertices_[i * 3]     = RinvidGfx::get_opengl_x_coord(verts_.at(i).x);
        vertices_[i * 3 + 1] = RinvidGfx::get_opengl_y_coord(verts_.at(i).y);
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_DYNAMIC_DRAW);
}

template <typename std::uint32_t number_of_vertices>
void FixedPolygonShape<number_of_vertices>::calculate_origin()
{
    auto min_x_vector =
        std::min_element(verts_.begin(), verts_.end(),
                         [](Vector2 first, Vector2 second) { return first.x < second.x; });
    auto max_x_vector =
        std::max_element(verts_.begin(), verts_.end(),
                         [](Vector2 first, Vector2 second) { return first.x < second.x; });

    auto min_y_vector =
        std::min_element(verts_.begin(), verts_.end(),
                         [](Vector2 first, Vector2 second) { return first.y < second.y; });
    auto max_y_vector =
        std::max_element(verts_.begin(), verts_.end(),
                         [](Vector2 first, Vector2 second) { return first.y < second.y; });

    float min_x = min_x_vector->x;
    float max_x = max_x_vector->x;
    float min_y = min_y_vector->y;
    float max_y = max_y_vector->y;

    origin_.x = (max_x + min_x) / 2.0F;
    origin_.y = (max_y + min_y) / 2.0F;
}

template <typename std::uint32_t number_of_vertices>
void FixedPolygonShape<number_of_vertices>::init_vertex_buffer()
{
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

template <typename std::uint32_t number_of_vertices>
void FixedPolygonShape<number_of_vertices>::move(const Vector2 move_vector)
{
    for (std::uint32_t i{0}; i < number_of_vertices; ++i)
    {
        verts_.at(i).move(move_vector);
    }

    calculate_origin();
}

template <typename std::uint32_t number_of_vertices>
void FixedPolygonShape<number_of_vertices>::set_position(const Vector2 vector)
{
    Vector2 move_vector{};

    move_vector.x = vector.x - origin_.x;
    move_vector.y = vector.y - origin_.y;

    for (std::uint32_t i{0}; i < number_of_vertices; ++i)
    {
        verts_.at(i).move(move_vector);
    }

    origin_ = vector;
}

} // namespace rinvid

#endif // CORE_INCLUDE_FIXED_POLYGON_SHAPE_H
