/**********************************************************************
 * Copyright (c) 2021 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <string>
#include <vector>

#include <rinvid/core/rinvid_gl.h>
#include <rinvid/core/texture.h>
#include <rinvid/util/error_handler.h>
#include <rinvid/util/image_loader.h>

namespace rinvid
{

void Texture::release_gl_resources()
{
    if (texture_id_ != 0)
    {
        GL_CALL(glDeleteTextures(1, &texture_id_));
        texture_id_ = 0;
    }
}

Texture::Texture(const char* file_name)
{
    std::vector<std::uint8_t> image_data{};
    bool                      result = load_image(file_name, image_data, width_, height_);
    if (result == false)
    {
        errors::put_error_to_log(std::string{file_name} +
                                 " image loading failed during texture creation");
    }

    GL_CALL(glGenTextures(1, &texture_id_));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, texture_id_));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                         image_data.data()));
}

Texture::Texture(Texture&& other)
{
    width_      = other.width_;
    height_     = other.height_;
    texture_id_ = other.texture_id_;

    other.texture_id_ = 0;
    other.width_      = 0;
    other.height_     = 0;
}

Texture& Texture::operator=(Texture&& other)
{
    if (this == &other)
    {
        return *this;
    }

    release_gl_resources();

    width_      = other.width_;
    height_     = other.height_;
    texture_id_ = other.texture_id_;

    other.texture_id_ = 0;
    other.width_      = 0;
    other.height_     = 0;

    return *this;
}

Texture::~Texture()
{
    release_gl_resources();
}

void Texture::bind() const
{
    GL_CALL(glBindTexture(GL_TEXTURE_2D, texture_id_));
}

} // namespace rinvid
