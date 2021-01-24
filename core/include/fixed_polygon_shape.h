/**********************************************************************
 * Copyright (c) 2020, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_FIXED_POLYGON_SHAPE_H
#define CORE_INCLUDE_FIXED_POLYGON_SHAPE_H

#include <vector>

#include "core/include/rinvid_gfx.h"
#include "core/include/shape.h"
#include "util/include/vector2.h"

namespace rinvid
{

/**************************************************************************************************
 * @brief A base class for shapes that are polygons with fixed number of vertices.
 *
 * @param number_of_vertices How many vertices does the shape consist of
 *
 *************************************************************************************************/
template <typename std::uint32_t number_of_vertices>
class FixedPolygonShape : public Shape
{
  public:
    /**************************************************************************************************
     * @brief FixedPolygonShape constructor.
     *
     * Initializes underlying OpenGl objects and internal vertex vector.
     *
     *************************************************************************************************/
    FixedPolygonShape();

    /**************************************************************************************************
     * @brief Copy constructor deleted.
     *
     *************************************************************************************************/
    FixedPolygonShape(const FixedPolygonShape& other) = delete;

    /**************************************************************************************************
     * @brief Copy assignement operator deleted.
     *
     *************************************************************************************************/
    FixedPolygonShape& operator=(FixedPolygonShape& other) = delete;

    /**************************************************************************************************
     * @brief Move constructor.
     *
     * @param other object being moved
     *
     *************************************************************************************************/
    FixedPolygonShape(FixedPolygonShape&& other);

    /**************************************************************************************************
     * @brief Move assignement operator.
     *
     * @param other object being moved
     *
     *************************************************************************************************/
    FixedPolygonShape& operator=(FixedPolygonShape&& other);

    /**************************************************************************************************
     * @brief FixedPolygonShape destructor.
     *
     *************************************************************************************************/
    ~FixedPolygonShape();

    /**************************************************************************************************
     * @brief Draw the shape.
     *
     * This method needs to be overridden in child classes. When overriding it, call base class
     * method first and then call draw_arrays() with appropriate argument.
     *
     *************************************************************************************************/
    virtual void draw() override;

    /**************************************************************************************************
     * @brief Move shape by adding move_vector to its position vector.
     *
     * @param move_vector Vector to be added to shape's position vector
     *
     *************************************************************************************************/
    virtual void move(const Vector2 move_vector) override;

    /**************************************************************************************************
     * @brief Sets shape's position to the position of passed vector.
     *
     * @param vector A new position vector of the shape
     *
     *************************************************************************************************/
    virtual void set_position(const Vector2 vector) override;

  protected:
    /**************************************************************************************************
     * @brief Normalizes coordinates (transforms x and y coordinates to [-1, 1] range). This is the
     * range that OpenGl operates on. This method should usually be called first within overridden
     * draw method.
     *
     *************************************************************************************************/
    virtual void normalize_coordinates() override;

    /**************************************************************************************************
     * @brief Calculates center point of a shape.
     *
     *************************************************************************************************/
    virtual void calculate_origin() override;

    /**************************************************************************************************
     * @brief Initializes OpenGl objects (vertex array object and vertex buffer object).
     *
     *************************************************************************************************/
    void init_vertex_buffer();

    /**************************************************************************************************
     * @brief A wrapper around glDrawArrays.
     *
     * @param mode A proper GLenum mode constant needs to be passed depending on what are you
     * drawing (e.g. GL_QUADS, GL_TRIANGLES...)
     *
     *************************************************************************************************/
    void draw_arrays(GLenum mode);

    const std::uint32_t number_of_vertices_;

    std::vector<Vector2> verts_;

    // Times 3 because each vertex has x, y, and z component.
    float vertices_[number_of_vertices * 3];
};

template <typename std::uint32_t number_of_vertices>
FixedPolygonShape<number_of_vertices>::FixedPolygonShape()
    : number_of_vertices_{number_of_vertices}, verts_{}, vertices_{}
{
    init_vertex_buffer();

    for (std::uint32_t i{0}; i < number_of_vertices; ++i)
    {
        verts_.emplace_back();
    }
}

template <typename std::uint32_t number_of_vertices>
FixedPolygonShape<number_of_vertices>::FixedPolygonShape(FixedPolygonShape&& other)
    : Shape(std::move(other)), number_of_vertices_{number_of_vertices}, verts_{}, vertices_{}
{
    verts_ = std::move(other.verts_);
    std::copy(std::begin(other.vertices_), std::end(other.vertices_), std::begin(this->vertices_));
}

template <typename std::uint32_t number_of_vertices>
FixedPolygonShape<number_of_vertices>& FixedPolygonShape<number_of_vertices>::
                                       operator=(FixedPolygonShape&& other)
{
    Shape::operator=(std::move(other));

    verts_ = std::move(other.verts_);
    std::copy(std::begin(other.vertices_), std::end(other.vertices_), std::begin(this->vertices_));

    return *this;
}

template <typename std::uint32_t number_of_vertices>
FixedPolygonShape<number_of_vertices>::~FixedPolygonShape()
{
}

template <typename std::uint32_t number_of_vertices>
void FixedPolygonShape<number_of_vertices>::normalize_coordinates()
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
void FixedPolygonShape<number_of_vertices>::draw_arrays(GLenum mode)
{
    glBindVertexArray(vao_);
    glDrawArrays(mode, 0, number_of_vertices_);
    glBindVertexArray(0);
}

template <typename std::uint32_t number_of_vertices>
void FixedPolygonShape<number_of_vertices>::draw()
{
    normalize_coordinates();

    glUseProgram(RinvidGfx::get_shape_default_shader());

    std::int32_t color_location =
        glGetUniformLocation(RinvidGfx::get_shape_default_shader(), "in_color");
    glUniform4f(color_location, color_.r, color_.g, color_.b, color_.a);
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
    calculate_origin();

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
