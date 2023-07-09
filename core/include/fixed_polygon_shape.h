/**********************************************************************
 * Copyright (c) 2020 - 2023, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_FIXED_POLYGON_SHAPE_H
#define CORE_INCLUDE_FIXED_POLYGON_SHAPE_H

#include <vector>

#include "core/include/drawable.h"
#include "core/include/rinvid_gfx.h"
#include "core/include/shape.h"
#include "util/include/error_handler.h"
#include "util/include/rect.h"
#include "util/include/vector2.h"

namespace rinvid
{

/**************************************************************************************************
 * @brief A base class for shapes that are polygons with fixed number of vertices.
 *
 * @param number_of_vertices How many vertices does the shape consist of
 *
 *************************************************************************************************/
template <typename std::uint32_t number_of_vertices, GLenum draw_mode = GL_POLYGON>
class FixedPolygonShape : public Shape, public Transformable, public Drawable
{
  public:
    /**************************************************************************************************
     * @brief FixedPolygonShape constructor.
     *
     * Initializes underlying OpenGl objects and internal vector of vertices. Use this constructor
     * for initializing derived objects.
     *
     *************************************************************************************************/
    FixedPolygonShape();

    /**************************************************************************************************
     * @brief FixedPolygonShape constructor.
     *
     * Initializes underlying OpenGl objects. Use this constructor when creating arbitrary polygon.
     *
     * @param vertices Vertices of the polygon
     *
     *************************************************************************************************/
    FixedPolygonShape(std::vector<Vector2<float>> vertices);

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
     * @brief FixedPolygonShape virtual destructor.
     *
     *************************************************************************************************/
    virtual ~FixedPolygonShape();

    /**************************************************************************************************
     * @brief Draw the shape.
     *
     *************************************************************************************************/
    virtual void draw() override;

    /**************************************************************************************************
     * @brief Move shape by adding move_vector to its position vector.
     *
     * @param move_vector Vector to be added to shape's position vector
     *
     *************************************************************************************************/
    virtual void move(const Vector2<float> move_vector) override;

    /**************************************************************************************************
     * @brief Sets shape's position to the position of passed vector.
     *
     * @param vector A new position vector of the shape
     *
     *************************************************************************************************/
    virtual void set_position(const Vector2<float> vector) override;

    /**************************************************************************************************
     * @brief Returns bounding box rect of the shape
     *
     * @return Bounding rect
     *
     *************************************************************************************************/
    Rect bounding_rect();

  protected:
    /**************************************************************************************************
     * @brief Updates OpenGl vertex buffer data
     *
     *************************************************************************************************/
    void update_gl_buffer_data();

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

    const std::uint32_t         number_of_vertices_;
    std::vector<Vector2<float>> vertices_;

    // Times 3 because each vertex has x, y, and z component.
    float gl_vertices_[number_of_vertices * 3];

  private:
    /**************************************************************************************************
     * @brief Returns bounding box rect of the shape not accounting transformation. Used as helper
     * when calculating origin
     *
     * @return Bounding rect as if no transformations are applied to object
     *
     *************************************************************************************************/
    Rect non_transformed_bounding_rect();
};

template <typename std::uint32_t number_of_vertices, GLenum draw_mode>
FixedPolygonShape<number_of_vertices, draw_mode>::FixedPolygonShape(
    std::vector<Vector2<float>> vertices)
    : number_of_vertices_{number_of_vertices}, vertices_{vertices}, gl_vertices_{}
{
    init_vertex_buffer();
    calculate_origin();
    update_gl_buffer_data();
}

template <typename std::uint32_t number_of_vertices, GLenum draw_mode>
FixedPolygonShape<number_of_vertices, draw_mode>::FixedPolygonShape()
    : number_of_vertices_{number_of_vertices}, vertices_{}, gl_vertices_{}
{
    init_vertex_buffer();

    for (std::uint32_t i{0}; i < number_of_vertices; ++i)
    {
        vertices_.emplace_back();
    }
}

template <typename std::uint32_t number_of_vertices, GLenum draw_mode>
FixedPolygonShape<number_of_vertices, draw_mode>::FixedPolygonShape(FixedPolygonShape&& other)
    : Shape(std::move(other)), number_of_vertices_{number_of_vertices}, vertices_{}, gl_vertices_{}
{
    vertices_ = std::move(other.vertices_);
    std::copy(std::begin(other.gl_vertices_), std::end(other.gl_vertices_),
              std::begin(this->gl_vertices_));
}

template <typename std::uint32_t number_of_vertices, GLenum draw_mode>
FixedPolygonShape<number_of_vertices, draw_mode>& FixedPolygonShape<number_of_vertices, draw_mode>::
                                                  operator=(FixedPolygonShape&& other)
{
    Shape::operator=(std::move(other));

    vertices_ = std::move(other.vertices_);
    std::copy(std::begin(other.gl_vertices_), std::end(other.gl_vertices_),
              std::begin(this->gl_vertices_));

    return *this;
}

template <typename std::uint32_t number_of_vertices, GLenum draw_mode>
FixedPolygonShape<number_of_vertices, draw_mode>::~FixedPolygonShape()
{
}

template <typename std::uint32_t number_of_vertices, GLenum draw_mode>
void FixedPolygonShape<number_of_vertices, draw_mode>::update_gl_buffer_data()
{
    for (std::uint32_t i{0}; i < number_of_vertices; ++i)
    {
        gl_vertices_[i * 3]     = vertices_.at(i).x - origin_.x;
        gl_vertices_[i * 3 + 1] = vertices_.at(i).y - origin_.y;
    }

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(gl_vertices_), gl_vertices_, GL_DYNAMIC_DRAW));
}

template <typename std::uint32_t number_of_vertices, GLenum draw_mode>
void FixedPolygonShape<number_of_vertices, draw_mode>::calculate_origin()
{
    Rect rect = non_transformed_bounding_rect();

    origin_.x = rect.position.x + (rect.width / 2);
    origin_.y = rect.position.y + (rect.height / 2);
}

template <typename std::uint32_t number_of_vertices, GLenum draw_mode>
void FixedPolygonShape<number_of_vertices, draw_mode>::init_vertex_buffer()
{
    GL_CALL(glGenVertexArrays(1, &vertex_array_object_));
    GL_CALL(glBindVertexArray(vertex_array_object_));

    GL_CALL(glGenBuffers(1, &vertex_buffer_object_));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(gl_vertices_), gl_vertices_, GL_DYNAMIC_DRAW));

    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
    GL_CALL(glEnableVertexAttribArray(0));

    GL_CALL(glBindVertexArray(0));
}

template <typename std::uint32_t number_of_vertices, GLenum draw_mode>
void FixedPolygonShape<number_of_vertices, draw_mode>::draw()
{
    RinvidGfx::use_shape_default_shader();
    RinvidGfx::update_mvp_matrix(get_transform(), RinvidGfx::get_shape_default_shader_id());
    const auto shader = RinvidGfx::get_shape_default_shader();
    shader.set_float4("in_color", color_.r, color_.g, color_.b, color_.a);

    GL_CALL(glBindVertexArray(vertex_array_object_));
    GL_CALL(glDrawArrays(draw_mode, 0, number_of_vertices_));
    GL_CALL(glBindVertexArray(0));
}

template <typename std::uint32_t number_of_vertices, GLenum draw_mode>
void FixedPolygonShape<number_of_vertices, draw_mode>::move(const Vector2<float> move_vector)
{
    for (std::uint32_t i{0}; i < number_of_vertices; ++i)
    {
        vertices_.at(i).move(move_vector);
    }

    calculate_origin();
}

template <typename std::uint32_t number_of_vertices, GLenum draw_mode>
void FixedPolygonShape<number_of_vertices, draw_mode>::set_position(const Vector2<float> vector)
{
    calculate_origin();

    Vector2<float> move_vector{};

    move_vector.x = vector.x - origin_.x;
    move_vector.y = vector.y - origin_.y;

    for (std::uint32_t i{0}; i < number_of_vertices; ++i)
    {
        vertices_.at(i).move(move_vector);
    }

    origin_ = vector;
}

template <typename std::uint32_t number_of_vertices, GLenum draw_mode>
Rect FixedPolygonShape<number_of_vertices, draw_mode>::bounding_rect()
{
    if (is_transformed() == false)
    {
        return non_transformed_bounding_rect();
    }

    Rect                   rect{};
    std::vector<glm::vec4> glm_vertices{};

    const auto& transform = get_transform();

    for (auto vec : vertices_)
    {
        glm::vec4 vert{vec.x, vec.y, 1.0F, 1.0F};
        vert = transform * vert;
        glm_vertices.push_back(vert);
    }

    float min_x{};
    float max_x{};
    float min_y{};
    float max_y{};

    set_min_max_coords(glm_vertices, min_x, max_x, min_y, max_y);

    rect.position.x = min_x;
    rect.position.y = min_y;
    rect.width      = max_x - min_x;
    rect.height     = max_y - min_y;

    return rect;
}

template <typename std::uint32_t number_of_vertices, GLenum draw_mode>
Rect FixedPolygonShape<number_of_vertices, draw_mode>::non_transformed_bounding_rect()
{
    Rect rect{};

    float min_x{};
    float max_x{};
    float min_y{};
    float max_y{};

    set_min_max_coords(vertices_, min_x, max_x, min_y, max_y);

    rect.position.x = min_x;
    rect.position.y = min_y;
    rect.width      = max_x - min_x;
    rect.height     = max_y - min_y;

    return rect;
}

} // namespace rinvid

#endif // CORE_INCLUDE_FIXED_POLYGON_SHAPE_H
