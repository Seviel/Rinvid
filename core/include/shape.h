/**********************************************************************
 * Copyright (c) 2020, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_SHAPE_H
#define CORE_INCLUDE_SHAPE_H

#include <cstdint>

#include "util/include/color.h"
#include "util/include/vector2.h"

namespace rinvid
{

/**************************************************************************************************
 * @brief A base class for drawable shapes.
 *
 *************************************************************************************************/
class Shape
{
  public:
    /**************************************************************************************************
     * @brief Shape constructor.
     *
     *************************************************************************************************/
    Shape();

    /**************************************************************************************************
     * @brief Copy constructor deleted.
     *
     *************************************************************************************************/
    Shape(const Shape& other) = delete;

    /**************************************************************************************************
     * @brief Copy assignement operator deleted.
     *
     *************************************************************************************************/
    Shape& operator=(Shape& other) = delete;

    /**************************************************************************************************
     * @brief Move constructor.
     *
     * @param other object being moved
     *
     *************************************************************************************************/
    Shape(Shape&& other);

    /**************************************************************************************************
     * @brief Move assignement operator.
     *
     * @param other object being moved
     *
     *************************************************************************************************/
    Shape& operator=(Shape&& other);

    /**************************************************************************************************
     * @brief Shape destructor.
     *
     *************************************************************************************************/
    ~Shape();

    /**************************************************************************************************
     * @brief Draws the shape.
     *
     *************************************************************************************************/
    virtual void draw() = 0;

    /**************************************************************************************************
     * @brief Move shape by adding move_vector to its position vector.
     *
     * @param move_vector Vector to be added to shape's position vector
     *
     *************************************************************************************************/
    virtual void move(const Vector2 move_vector) = 0;

    /**************************************************************************************************
     * @brief Sets shape's position to the position of passed vector.
     *
     * @param vector A new position vector of the shape
     *
     *************************************************************************************************/
    virtual void set_position(const Vector2 vector) = 0;

    /**************************************************************************************************
     * @brief Returns center point of shape.
     *
     * @return Center point of shape
     *
     *************************************************************************************************/
    Vector2 get_origin();

    /**************************************************************************************************
     * @brief Sets shape's color.
     *
     * @param color A new color of the shape
     *
     *************************************************************************************************/
    void set_color(const Color color);

  protected:
    Vector2       origin_;
    Color         color_;
    std::uint32_t vao_;
    std::uint32_t vbo_;

  private:
    /**************************************************************************************************
     * @brief Normalizes coordinates (transforms x and y coordinates to [-1, 1] range). This is the
     * range that OpenGl operates on. This method should usually be called first within overridden
     * draw method.
     *
     *************************************************************************************************/
    virtual void normalize_coordinates() = 0;

    /**************************************************************************************************
     * @brief Calculates center point of a shape.
     *
     *************************************************************************************************/
    virtual void calculate_origin() = 0;
};

} // namespace rinvid

#endif // CORE_INCLUDE_SHAPE_H
