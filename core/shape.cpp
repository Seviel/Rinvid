/**********************************************************************
 * Copyright (c) 2020, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#define GL_GLEXT_PROTOTYPES
#include <SFML/OpenGL.hpp>

#include "include/shape.h"
#include "util/include/color.h"

namespace rinvid
{

Shape::Shape() : origin_{}, color_{}, vao_{}, vbo_{}
{
}

Shape::Shape(Shape&& other)
{
    this->origin_ = other.origin_;
    this->color_  = other.color_;
    this->vao_    = other.vao_;
    this->vbo_    = other.vbo_;

    other.vao_ = 0;
    other.vbo_ = 0;
}

Shape& Shape::operator=(Shape&& other)
{
    this->origin_ = other.origin_;
    this->color_  = other.color_;
    this->vao_    = other.vao_;
    this->vbo_    = other.vbo_;

    other.vao_ = 0;
    other.vbo_ = 0;

    return *this;
}

Shape::~Shape()
{
    if (vbo_ != 0)
    {
        glDeleteBuffers(1, &vbo_);
    }

    if (vao_ != 0)
    {
        glDeleteVertexArrays(1, &vao_);
    }
}

Vector2 Shape::get_origin()
{
    return origin_;
}

void Shape::set_color(const Color color)
{
    color_ = color;
}

} // namespace rinvid
