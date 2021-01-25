/**********************************************************************
 * Copyright (c) 2021, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <algorithm>
#include <iostream>
#include <iterator>

#define GL_GLEXT_PROTOTYPES
#include <SFML/OpenGL.hpp>

#include "core/include/rinvid_gfx.h"
#include "include/texture.h"
#include "util/include/image_loader.h"

namespace rinvid
{

Texture::Texture(const char* file_name)
{
    glGenVertexArrays(1, &vertex_array_object_);
    glGenBuffers(1, &vertex_buffer_obecjt_);
    glGenBuffers(1, &element_buffer_object_);

    glBindVertexArray(vertex_array_object_);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obecjt_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_), indices_, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    bool result = load_image(file_name, image_data_, width_, height_);
    if (result == false)
    {
        /// @todo Handle error
    }

    glGenTextures(1, &texture_id_);
    glBindTexture(GL_TEXTURE_2D, texture_id_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 image_data_.data());
}

Texture::Texture(Texture&& other)
{
    this->width_                 = other.width_;
    this->height_                = other.height_;
    this->vertex_array_object_   = other.vertex_array_object_;
    this->vertex_buffer_obecjt_  = other.vertex_buffer_obecjt_;
    this->element_buffer_object_ = other.element_buffer_object_;
    this->texture_id_            = other.texture_id_;

    std::copy(std::begin(other.vertices_), std::end(other.vertices_), std::begin(this->vertices_));

    other.vertex_array_object_   = 0;
    other.vertex_buffer_obecjt_  = 0;
    other.element_buffer_object_ = 0;
    other.texture_id_            = 0;
}

Texture& Texture::operator=(Texture&& other)
{
    this->width_                 = other.width_;
    this->height_                = other.height_;
    this->vertex_array_object_   = other.vertex_array_object_;
    this->vertex_buffer_obecjt_  = other.vertex_buffer_obecjt_;
    this->element_buffer_object_ = other.element_buffer_object_;
    this->texture_id_            = other.texture_id_;

    std::copy(std::begin(other.vertices_), std::end(other.vertices_), std::begin(this->vertices_));

    other.vertex_array_object_   = 0;
    other.vertex_buffer_obecjt_  = 0;
    other.element_buffer_object_ = 0;
    other.texture_id_            = 0;

    return *this;
}

Texture::~Texture()
{
    if (texture_id_ != 0)
    {
        glDeleteTextures(1, &texture_id_);
    }

    if (element_buffer_object_ != 0)
    {
        glDeleteBuffers(1, &element_buffer_object_);
    }

    if (vertex_buffer_obecjt_ != 0)
    {
        glDeleteBuffers(1, &vertex_buffer_obecjt_);
    }

    if (vertex_array_object_ != 0)
    {
        glDeleteVertexArrays(1, &vertex_array_object_);
    }
}

void Texture::draw()
{
    glUseProgram(RinvidGfx::get_texture_default_shader());

    glBindTexture(GL_TEXTURE_2D, texture_id_);
    glBindVertexArray(vertex_array_object_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Texture::update_vertices(Vector2 top_left, Vector2 offset, std::uint32_t width,
                              std::uint32_t height)
{
    // Top left
    vertices_[0] = RinvidGfx::get_opengl_x_coord(top_left.x);
    vertices_[1] = RinvidGfx::get_opengl_y_coord(top_left.y);
    vertices_[2] = 0.0F;
    vertices_[3] = offset.x / static_cast<float>(width_);
    vertices_[4] = offset.y / static_cast<float>(height_);

    // Top right
    vertices_[5] = RinvidGfx::get_opengl_x_coord(top_left.x + width);
    vertices_[6] = RinvidGfx::get_opengl_y_coord(top_left.y);
    vertices_[7] = 0.0F;
    vertices_[8] = static_cast<float>(width) / static_cast<float>(width_) +
                   offset.x / static_cast<float>(width_);
    vertices_[9] = offset.y / static_cast<float>(height_);

    // Bottom right
    vertices_[10] = RinvidGfx::get_opengl_x_coord(top_left.x + width);
    vertices_[11] = RinvidGfx::get_opengl_y_coord(top_left.y + height);
    vertices_[12] = 0.0F;
    vertices_[13] = static_cast<float>(width) / static_cast<float>(width_) +
                    offset.x / static_cast<float>(width_);
    vertices_[14] = static_cast<float>(height) / static_cast<float>(height_) +
                    offset.y / static_cast<float>(height_);

    // Bottom left
    vertices_[15] = RinvidGfx::get_opengl_x_coord(top_left.x);
    vertices_[16] = RinvidGfx::get_opengl_y_coord(top_left.y + height);
    vertices_[17] = 0.0F;
    vertices_[18] = offset.x / static_cast<float>(width_);
    vertices_[19] = static_cast<float>(height) / static_cast<float>(height_) +
                    offset.y / static_cast<float>(height_);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obecjt_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_STATIC_DRAW);
}

} // namespace rinvid
