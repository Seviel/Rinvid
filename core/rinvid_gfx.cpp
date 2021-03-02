/**********************************************************************
 * Copyright (c) 2020 - 2021, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include "include/rinvid_gfx.h"
#include "extern/glm/gtc/type_ptr.hpp"
#include "extern/glm/gtx/transform.hpp"

namespace
{

static const char* texture_vert_shader_source =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTexCoord;\n"
    "out vec2 TexCoord;\n"
    "uniform mat4 model_view_projection;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = model_view_projection * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
    "}\0";

static const char* texture_frag_shader_source =
    "#version 330 core\n"
    "out vec4 out_color;\n "
    "uniform vec4 in_color;\n"
    "uniform sampler2D texture1;\n"
    "in vec2 TexCoord;\n"
    "void main()\n"
    "{\n"
    "   out_color = texture(texture1, TexCoord);\n"
    "}\n";

static const char* shape_vert_shader_source =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "uniform mat4 model_view_projection;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = model_view_projection * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

static const char* shape_frag_shader_source =
    "#version 330 core\n"
    "out vec4 out_color;\n "
    "uniform vec4 in_color;\n"
    "void main()\n"
    "{\n"
    "   out_color = in_color;\n"
    "}\n";

static void init_default_shaders(std::uint32_t& shape_default_shader_handle,
                                 std::uint32_t& texture_default_shader_handle)
{
    std::uint32_t vert_shader;
    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &shape_vert_shader_source, nullptr);
    glCompileShader(vert_shader);

    std::uint32_t frag_shader;
    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &shape_frag_shader_source, nullptr);
    glCompileShader(frag_shader);

    shape_default_shader_handle = glCreateProgram();

    glAttachShader(shape_default_shader_handle, vert_shader);
    glAttachShader(shape_default_shader_handle, frag_shader);
    glLinkProgram(shape_default_shader_handle);

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &texture_vert_shader_source, nullptr);
    glCompileShader(vert_shader);

    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &texture_frag_shader_source, nullptr);
    glCompileShader(frag_shader);

    texture_default_shader_handle = glCreateProgram();

    glAttachShader(texture_default_shader_handle, vert_shader);
    glAttachShader(texture_default_shader_handle, frag_shader);
    glLinkProgram(texture_default_shader_handle);

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
}

} // namespace

namespace rinvid
{

glm::mat4     RinvidGfx::model_view_projection_{1.0F};
glm::mat4     RinvidGfx::view_{1.0F};
glm::mat4     RinvidGfx::projection_{1.0F};
std::uint32_t RinvidGfx::shape_default_shader_{};
std::uint32_t RinvidGfx::texture_default_shader_{};
std::int32_t  RinvidGfx::width_{};
std::int32_t  RinvidGfx::height_{};

void RinvidGfx::init()
{
    init_default_shaders(RinvidGfx::shape_default_shader_, RinvidGfx::texture_default_shader_);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    projection_            = glm::ortho(0.0F, static_cast<float>(RinvidGfx::get_width()),
                             static_cast<float>(RinvidGfx::get_height()), 0.0F, -1.0f, 1.0f);
    model_view_projection_ = projection_ * view_;
}

void RinvidGfx::set_viewport(std::int32_t x, std::int32_t y, std::int32_t width,
                             std::int32_t heigth)
{
    RinvidGfx::width_  = width;
    RinvidGfx::height_ = heigth;
    glViewport(x, y, width, heigth);
}

void RinvidGfx::clear_screen(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

std::uint32_t RinvidGfx::get_shape_default_shader()
{
    return shape_default_shader_;
}

std::uint32_t RinvidGfx::get_texture_default_shader()
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
    projection_                = glm::ortho(0.0F, static_cast<float>(RinvidGfx::get_width()),
                             static_cast<float>(RinvidGfx::get_height()), 0.0F, -1.0f, 1.0f);
    model_view_projection_     = projection_ * view_ * model;
    std::uint32_t mvp_location = glGetUniformLocation(shader_id, "model_view_projection");
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(model_view_projection_));
}

} // namespace rinvid
