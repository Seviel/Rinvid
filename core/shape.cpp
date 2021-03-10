/**********************************************************************
 * Copyright (c) 2020 - 2021, Filip Vasiljevic
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
    this->color_                = other.color_;
    this->vertex_array_object_  = other.vertex_array_object_;
    this->vertex_buffer_object_ = other.vertex_buffer_object_;

    other.vertex_array_object_  = 0;
    other.vertex_buffer_object_ = 0;

    return *this;
}

Shape::~Shape()
{
    if (vertex_buffer_object_ != 0)
    {
        glDeleteBuffers(1, &vertex_buffer_object_);
    }

    if (vertex_array_object_ != 0)
    {
        glDeleteVertexArrays(1, &vertex_array_object_);
    }
}

void Shape::set_color(const Color color)
{
    color_ = color;
}

} // namespace rinvid
