/**********************************************************************
 * Copyright (c) 2021 - 2022, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <iostream>
#include <iterator>

#include "core/include/rinvid_gfx.h"
#include "core/include/rinvid_gl.h"
#include "extern/glm/glm/gtc/type_ptr.hpp"
#include "include/texture.h"
#include "util/include/error_handler.h"
#include "util/include/image_loader.h"

namespace rinvid
{

Texture::Texture(const char* file_name)
{
    GL_CALL(glGenVertexArrays(1, &vertex_array_object_));
    GL_CALL(glGenBuffers(1, &vertex_buffer_obecjt_));
    GL_CALL(glGenBuffers(1, &element_buffer_object_));

    GL_CALL(glBindVertexArray(vertex_array_object_));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obecjt_));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(gl_vertices_), gl_vertices_, GL_STATIC_DRAW));

    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object_));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_), indices_, GL_STATIC_DRAW));

    // Position attribute
    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
    GL_CALL(glEnableVertexAttribArray(0));

    // Texture coordinate attribute
    GL_CALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                                  (void*)(3 * sizeof(float))));
    GL_CALL(glEnableVertexAttribArray(1));

    bool result = load_image(file_name, image_data_, width_, height_);
    if (result == false)
    {
        errors::put_error_to_log("Image loading failed during texture creation");
    }

    GL_CALL(glGenTextures(1, &texture_id_));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, texture_id_));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                         image_data_.data()));
}

Texture::Texture(Texture&& other)
{
    this->width_                 = other.width_;
    this->height_                = other.height_;
    this->vertex_array_object_   = other.vertex_array_object_;
    this->vertex_buffer_obecjt_  = other.vertex_buffer_obecjt_;
    this->element_buffer_object_ = other.element_buffer_object_;
    this->texture_id_            = other.texture_id_;

    std::copy(std::begin(other.gl_vertices_), std::end(other.gl_vertices_),
              std::begin(this->gl_vertices_));

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

    std::copy(std::begin(other.gl_vertices_), std::end(other.gl_vertices_),
              std::begin(this->gl_vertices_));

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
        GL_CALL(glDeleteTextures(1, &texture_id_));
    }

    if (element_buffer_object_ != 0)
    {
        GL_CALL(glDeleteBuffers(1, &element_buffer_object_));
    }

    if (vertex_buffer_obecjt_ != 0)
    {
        GL_CALL(glDeleteBuffers(1, &vertex_buffer_obecjt_));
    }

    if (vertex_array_object_ != 0)
    {
        GL_CALL(glDeleteVertexArrays(1, &vertex_array_object_));
    }
}

void Texture::draw(const glm::mat4& transform, float opacity)
{
    GL_CALL(glUseProgram(RinvidGfx::get_texture_default_shader()));

    RinvidGfx::update_mvp_matrix(transform, RinvidGfx::get_texture_default_shader());

    std::int32_t opacity_location =
        glGetUniformLocation(RinvidGfx::get_texture_default_shader(), "opacity");
    rinvid::errors::handle_gl_errors(__FILE__, __LINE__);
    if (opacity_location == -1)
    {
        rinvid::errors::put_error_to_log("glGetUniformLocation error: invalid uniform name");
        return;
    }
    GL_CALL(glUniform1f(opacity_location, opacity));

    GL_CALL(glBindTexture(GL_TEXTURE_2D, texture_id_));
    GL_CALL(glBindVertexArray(vertex_array_object_));
    GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
}

void Texture::update_vertices(Vector2<float> offset, std::uint32_t width, std::uint32_t height)
{
    // Make center of texture (0, 0) for simplicity
    Vector2<float> top_left{};
    top_left.x = 0.0F - (width / 2.0F);
    top_left.y = 0.0F - (height / 2.0F);

    // Top left
    gl_vertices_[0] = top_left.x;
    gl_vertices_[1] = top_left.y;
    gl_vertices_[2] = 0.0F;
    gl_vertices_[3] = offset.x / static_cast<float>(width_);
    gl_vertices_[4] = offset.y / static_cast<float>(height_);

    // Top right
    gl_vertices_[5] = top_left.x + width;
    gl_vertices_[6] = top_left.y;
    gl_vertices_[7] = 0.0F;
    gl_vertices_[8] = static_cast<float>(width) / static_cast<float>(width_) +
                      offset.x / static_cast<float>(width_);
    gl_vertices_[9] = offset.y / static_cast<float>(height_);

    // Bottom right
    gl_vertices_[10] = top_left.x + width;
    gl_vertices_[11] = top_left.y + height;
    gl_vertices_[12] = 0.0F;
    gl_vertices_[13] = static_cast<float>(width) / static_cast<float>(width_) +
                       offset.x / static_cast<float>(width_);
    gl_vertices_[14] = static_cast<float>(height) / static_cast<float>(height_) +
                       offset.y / static_cast<float>(height_);

    // Bottom left
    gl_vertices_[15] = top_left.x;
    gl_vertices_[16] = top_left.y + height;
    gl_vertices_[17] = 0.0F;
    gl_vertices_[18] = offset.x / static_cast<float>(width_);
    gl_vertices_[19] = static_cast<float>(height) / static_cast<float>(height_) +
                       offset.y / static_cast<float>(height_);

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obecjt_));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(gl_vertices_), gl_vertices_, GL_STATIC_DRAW));
}

} // namespace rinvid
