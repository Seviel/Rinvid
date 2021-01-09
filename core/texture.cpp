/**********************************************************************
 * Copyright (c) 2021, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <iostream>

#define GL_GLEXT_PROTOTYPES
#include <SFML/OpenGL.hpp>

#include "include/texture.h"
#include "util/include/image_loader.h"

namespace rinvid
{

Texture::Texture(const char* file_name)
{
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_), indices_, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    bool res = load_image(file_name, image_data_, width_, height_);

    glGenTextures(1, &texture_id_);
    glBindTexture(GL_TEXTURE_2D, texture_id_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 image_data_.data());
}

void Texture::draw()
{
    glBindTexture(GL_TEXTURE_2D, texture_id_);
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Texture::update_vertices(Vector2 top_left, std::uint32_t width, std::uint32_t height)
{
    RinvidGfx::get_opengl_x_coord(100.0F), RinvidGfx::get_opengl_y_coord(100.0F);

    vertices_[0] = RinvidGfx::get_opengl_x_coord(top_left.x);
    vertices_[1] = RinvidGfx::get_opengl_y_coord(top_left.y);
    vertices_[2] = 0.0F;
    vertices_[3] = 0.0F;
    vertices_[4] = 0.0F;

    vertices_[5] = RinvidGfx::get_opengl_x_coord(top_left.x + width);
    vertices_[6] = RinvidGfx::get_opengl_y_coord(top_left.y);
    vertices_[7] = 0.0F;
    vertices_[8] = 1.0F;
    vertices_[9] = 0.0F;

    vertices_[10] = RinvidGfx::get_opengl_x_coord(top_left.x + width);
    vertices_[11] = RinvidGfx::get_opengl_y_coord(top_left.y + height);
    vertices_[12] = 0.0F;
    vertices_[13] = 1.0F;
    vertices_[14] = 1.0F;

    vertices_[15] = RinvidGfx::get_opengl_x_coord(top_left.x);
    vertices_[16] = RinvidGfx::get_opengl_y_coord(top_left.y + height);
    vertices_[17] = 0.0F;
    vertices_[18] = 0.0F;
    vertices_[19] = 1.0F;

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_STATIC_DRAW);
}

} // namespace rinvid
