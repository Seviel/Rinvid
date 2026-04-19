/**********************************************************************
 * Copyright (c) 2020 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "include/shape.h"
#include "core/include/rinvid_gl.h"
#include "util/include/color.h"
#include "util/include/error_handler.h"

namespace rinvid
{

void Shape::release_gl_resources()
{
    if (vertex_buffer_object_ != 0)
    {
        GL_CALL(glDeleteBuffers(1, &vertex_buffer_object_));
        vertex_buffer_object_ = 0;
    }

    if (vertex_array_object_ != 0)
    {
        GL_CALL(glDeleteVertexArrays(1, &vertex_array_object_));
        vertex_array_object_ = 0;
    }
}

Shape::Shape() : color_{}, vertex_array_object_{}, vertex_buffer_object_{}
{
}

Shape::Shape(Shape&& other)
{
    this->color_                = other.color_;
    this->vertex_array_object_  = other.vertex_array_object_;
    this->vertex_buffer_object_ = other.vertex_buffer_object_;

    other.vertex_array_object_  = 0;
    other.vertex_buffer_object_ = 0;
}

Shape& Shape::operator=(Shape&& other)
{
    if (this == &other)
    {
        return *this;
    }

    release_gl_resources();

    this->color_                = other.color_;
    this->vertex_array_object_  = other.vertex_array_object_;
    this->vertex_buffer_object_ = other.vertex_buffer_object_;

    other.vertex_array_object_  = 0;
    other.vertex_buffer_object_ = 0;

    return *this;
}

Shape::~Shape()
{
    release_gl_resources();
}

void Shape::set_color(const Color color)
{
    color_ = color;
}

} // namespace rinvid
