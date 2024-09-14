/**********************************************************************
 * Copyright (c) 2020 - 2024, Filip Vasiljevic
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

const char* default_shape_vert =
    "#version 330 core\n\
    layout(location = 0) in vec3 position;\n\
    uniform mat4 model_view_projection;\n\
    void main()\n\
    {\n\
        gl_Position = model_view_projection * vec4(position.x, position.y, position.z, 1.0);\n\
    }\n";

const char* default_shape_frag =
    "#version 330 core\n\
    out vec4  out_color;\n\
    uniform vec4  in_color;\n\
    uniform bool  use_ambient_light = false;\n\
    uniform float ambient_strength  = 0.1;\n\
    #define NUMBER_OF_LIGHTS 100\n\
    uniform bool  light_active[NUMBER_OF_LIGHTS];\n\
    uniform vec2  light_pos[NUMBER_OF_LIGHTS];\n\
    uniform float light_intensity[NUMBER_OF_LIGHTS];\n\
    uniform float light_falloff[NUMBER_OF_LIGHTS];\n\
    vec4 apply_light(vec3 object_color, vec3 ambient, int light_number)\n\
    {\n\
        vec4 color = vec4(0.0, 0.0, 0.0, 0.0);\n\
        if (light_active[light_number])\n\
        {\n\
            vec2  pixel       = gl_FragCoord.xy;\n\
            vec2  aux         = light_pos[light_number] - pixel;\n\
            float dist        = length(aux);\n\
            dist = dist / light_falloff[light_number];\n\
            float light_attenuation =\n\
                1.0 / (0.1 + 0.1 * dist + 0.1 * dist * dist);\n\
            light_attenuation = light_attenuation * light_intensity[light_number];\n\
            color = vec4(light_attenuation, light_attenuation, light_attenuation, 1.0) *\n\
                    vec4(object_color, 1.0);\n\
        }\n\
        else\n\
        {\n\
            color = vec4(object_color, 1.0);\n\
        }\n\
        return color;\n\
    }\n\
    void main()\n\
    {\n\
        float used_ambient_strength = ambient_strength;\n\
        if (!use_ambient_light)\n\
            used_ambient_strength = 1.0;\n\
        vec3 ambient = vec3(used_ambient_strength, used_ambient_strength, \n\
    used_ambient_strength);\n\
        vec3 color   = vec3(0.0, 0.0, 0.0);\n\
        bool any_light_active = false;\n\
        for (int i = 0; i < NUMBER_OF_LIGHTS; ++i)\n\
        {\n\
            if (light_active[i])\n\
            {\n\
                color += apply_light(in_color.xyz * used_ambient_strength, ambient, i).xyz;\n\
                any_light_active = true;\n\
            }\n\
        }\n\
        if (!any_light_active)\n\
        {\n\
            color = in_color.xyz * used_ambient_strength;\n\
        }\n\
        out_color.xyz = color.xyz;\n\
        out_color.a   = 1.0;\n\
    }\n";

const char* default_texture_vert =
    "#version 330 core\n\
    layout (location = 0) in vec3 position;\n\
    layout (location = 1) in vec2 texture_coord;\n\
    out vec2 tex_coord;\n\
    uniform mat4 model_view_projection;\n\
    void main()\n\
    {\n\
       gl_Position = model_view_projection * vec4(position.x, position.y, position.z, 1.0);\n\
       tex_coord = vec2(texture_coord.x, texture_coord.y);\n\
    }\n";

const char* default_texture_frag =
    "#version 330 core\n\
    out vec4 out_color; \n\
    uniform float opacity;\n\
    uniform sampler2D the_texture;\n\
    uniform bool use_ambient_light = false;\n\
    uniform float ambient_strength = 0.1;\n\
    in vec2 tex_coord;\n\
    #define NUMBER_OF_LIGHTS 100 \n\
    uniform bool light_active[NUMBER_OF_LIGHTS];\n\
    uniform vec2 light_pos[NUMBER_OF_LIGHTS];\n\
    uniform float light_intensity[NUMBER_OF_LIGHTS];\n\
    uniform float light_falloff[NUMBER_OF_LIGHTS];\n\
    vec4 apply_light(vec3 object_color, vec3 ambient, int light_number)\n\
    {\n\
       vec4 color = vec4(0.0, 0.0, 0.0, 0.0);\n\
       if (light_active[light_number])\n\
       {\n\
          vec2 pixel = gl_FragCoord.xy;\n\
          vec2 aux = light_pos[light_number] - pixel;\n\
          float dist = length(aux);\n\
          dist = dist / light_falloff[light_number];\n\
          float light_attenuation = 1.0 / (0.1 + 0.1 * dist + 0.1 * dist * dist);\n\
          light_attenuation = light_attenuation * light_intensity[light_number];\n\
          color = vec4(light_attenuation, light_attenuation, light_attenuation, 1.0) * vec4(object_color, 1.0);\n\
       }\n\
       else \n\
       {\n\
          color = vec4(object_color, 1.0);\n\
       }\n\
       return color;\n\
    }\n\
    void main()\n\
    {\n\
       float used_ambient_strength = ambient_strength;\n\
       if (!use_ambient_light)\n\
          used_ambient_strength = 1.0;\n\
       vec4 alpha = texture(the_texture, tex_coord).aaaa;\n\
       vec3 ambient = vec3(used_ambient_strength, used_ambient_strength, used_ambient_strength);\n\
       vec3 object_color = texture(the_texture, tex_coord).rgb;\n\
       vec4 color = vec4(0.0, 0.0, 0.0, 0.0);\n\
       bool any_light_active = false;\n\
       for (int i = 0; i < NUMBER_OF_LIGHTS; ++i)\n\
       {\n\
          if (light_active[i])\n\
          {\n\
             color += apply_light(object_color * ambient, ambient, i);\n\
             any_light_active = true;\n\
          }\n\
       }\n\
       if (!any_light_active)\n\
       {\n\
          color = vec4(object_color * ambient, 1.0);\n\
       }\n\
       out_color = color;\n\
       out_color.a = alpha.a * opacity;\n\
    }\n";

const char* default_text_vert =
    "#version 330 core\n\
    layout (location = 0) in vec4 vertex;\n\
    out vec2 tex_coords;\n\
    \n\
    uniform mat4 projection;\n\
    \n\
    void main()\n\
    {\n\
        gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n\
        tex_coords = vertex.zw;\n\
    }\n";

const char* default_text_frag =
    "#version 330 core\n\
    in vec2 tex_coords;\n\
    out vec4 color;\n\
    \n\
    uniform sampler2D text;\n\
    uniform vec3 text_color;\n\
    \n\
    void main()\n\
    {    \n\
        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, tex_coords).r);\n\
        color = vec4(text_color, 1.0) * sampled;\n\
    }\n";

glm::mat4    RinvidGfx::model_view_projection_{1.0F};
glm::mat4    RinvidGfx::view_{1.0F};
glm::mat4    RinvidGfx::projection_{1.0F};
Shader       RinvidGfx::shape_default_shader_{};
Shader       RinvidGfx::texture_default_shader_{};
Shader       RinvidGfx::text_default_shader_{};
std::int32_t RinvidGfx::width_{};
std::int32_t RinvidGfx::height_{};

void RinvidGfx::init_default_shaders()
{
    shape_default_shader_   = Shader(default_shape_vert, default_shape_frag);
    texture_default_shader_ = Shader(default_texture_vert, default_texture_frag);
    text_default_shader_    = Shader(default_text_vert, default_text_frag);
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

std::uint32_t RinvidGfx::get_text_default_shader_id()
{
    return text_default_shader_.get_id();
}

const Shader RinvidGfx::get_shape_default_shader()
{
    return shape_default_shader_;
}

const Shader RinvidGfx::get_texture_default_shader()
{
    return texture_default_shader_;
}

const Shader RinvidGfx::get_text_default_shader()
{
    return text_default_shader_;
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

void RinvidGfx::use_text_default_shader()
{
    text_default_shader_.use();
}

} // namespace rinvid
