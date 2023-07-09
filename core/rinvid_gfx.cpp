/**********************************************************************
 * Copyright (c) 2020 - 2023, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "include/rinvid_gfx.h"
#include "extern/glm/glm/gtc/type_ptr.hpp"
#include "extern/glm/glm/gtx/transform.hpp"
#include "util/include/error_handler.h"

namespace rinvid
{

glm::mat4    RinvidGfx::model_view_projection_{1.0F};
glm::mat4    RinvidGfx::view_{1.0F};
glm::mat4    RinvidGfx::projection_{1.0F};
Shader       RinvidGfx::shape_default_shader_{};
Shader       RinvidGfx::texture_default_shader_{};
std::int32_t RinvidGfx::width_{};
std::int32_t RinvidGfx::height_{};

void RinvidGfx::init_default_shaders()
{
    shape_default_shader_ =
        Shader("core/shaders/default_shape.vert", "core/shaders/default_shape.frag");
    texture_default_shader_ =
        Shader("core/shaders/default_texture.vert", "core/shaders/default_texture.frag");
}

void RinvidGfx::init()
{
    RinvidGfx::init_default_shaders();
    GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GL_CALL(glEnable(GL_BLEND));
    projection_            = glm::ortho(0.0F, static_cast<float>(RinvidGfx::get_width()),
                             static_cast<float>(RinvidGfx::get_height()), 0.0F, -1.0f, 1.0f);
    model_view_projection_ = projection_ * view_;
}

void RinvidGfx::set_viewport(std::int32_t x, std::int32_t y, std::int32_t width,
                             std::int32_t heigth)
{
    RinvidGfx::width_  = width;
    RinvidGfx::height_ = heigth;
    GL_CALL(glViewport(x, y, width, heigth));
}

void RinvidGfx::clear_screen(float r, float g, float b, float a)
{
    GL_CALL(glClearColor(r, g, b, a));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

std::uint32_t RinvidGfx::get_shape_default_shader_id()
{
    return shape_default_shader_.get_id();
}

std::uint32_t RinvidGfx::get_texture_default_shader_id()
{
    return texture_default_shader_.get_id();
}

const Shader RinvidGfx::get_shape_default_shader()
{
    return shape_default_shader_;
}

const Shader RinvidGfx::get_texture_default_shader()
{
    return texture_default_shader_;
}

std::int32_t RinvidGfx::get_width()
{
    return width_;
}

std::int32_t RinvidGfx::get_height()
{
    return height_;
}

void RinvidGfx::update_mvp_matrix(const glm::mat4& model, std::uint32_t shader_id)
{
    projection_               = glm::ortho(0.0F, static_cast<float>(RinvidGfx::get_width()),
                             static_cast<float>(RinvidGfx::get_height()), 0.0F, -1.0f, 1.0f);
    model_view_projection_    = projection_ * view_ * model;
    std::int32_t mvp_location = glGetUniformLocation(shader_id, "model_view_projection");
    rinvid::errors::handle_gl_errors(__FILE__, __LINE__);
    if (mvp_location == -1)
    {
        rinvid::errors::put_error_to_log("glGetUniformLocation error: invalid uniform name");
        return;
    }
    GL_CALL(glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(model_view_projection_)));
}

void RinvidGfx::update_view(const glm::mat4& view)
{
    view_ = view;
}

const glm::mat4& RinvidGfx::get_view()
{
    return view_;
}

void RinvidGfx::use_shape_default_shader()
{
    shape_default_shader_.use();
}

void RinvidGfx::use_texture_default_shader()
{
    texture_default_shader_.use();
}

} // namespace rinvid
