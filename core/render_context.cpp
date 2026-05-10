/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <algorithm>
#include <cstddef>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <rinvid/core/render_context.h>
#include <rinvid/util/error_handler.h>

namespace rinvid
{

namespace
{

constexpr std::uint32_t LIGHT_TEXTURE_UNIT{1U};

const char* default_shape_vert =
    "#version 330 core\n\
    layout(location = 0) in vec3 position;\n\
    out vec2 world_pos;\n\
    uniform mat4 model;\n\
    uniform mat4 model_view_projection;\n\
    void main()\n\
    {\n\
        vec4 world_position = model * vec4(position.x, position.y, position.z, 1.0);\n\
        world_pos = world_position.xy;\n\
        gl_Position = model_view_projection * vec4(position.x, position.y, position.z, 1.0);\n\
    }\n";

const char* default_shape_frag =
    "#version 330 core\n\
    out vec4  out_color;\n\
    uniform vec4  in_color;\n\
    uniform bool  use_ambient_light = false;\n\
    uniform float ambient_strength  = 0.1;\n\
    uniform samplerBuffer light_data;\n\
    uniform int light_count = 0;\n\
    in vec2 world_pos;\n\
    float get_light_amount(vec4 light)\n\
    {\n\
        vec2  aux         = light.xy - world_pos;\n\
        float dist        = length(aux);\n\
        dist = dist / light.w;\n\
        float light_attenuation = 1.0 / (0.1 + 0.1 * dist + 0.1 * dist * dist);\n\
        return light_attenuation * light.z;\n\
    }\n\
    vec3 apply_lighting(vec3 object_color)\n\
    {\n\
        float ambient = use_ambient_light ? ambient_strength : 0.0;\n\
        float direct_light = 0.0;\n\
        bool any_light_active = false;\n\
        for (int i = 0; i < light_count; ++i)\n\
        {\n\
            vec4 light = texelFetch(light_data, i);\n\
            if (light.z > 0.0)\n\
            {\n\
                direct_light += get_light_amount(light);\n\
                any_light_active = true;\n\
            }\n\
        }\n\
        if (!any_light_active)\n\
        {\n\
            return object_color * (use_ambient_light ? ambient_strength : 1.0);\n\
        }\n\
        return clamp(object_color * (ambient + direct_light), 0.0, 1.0);\n\
    }\n\
    void main()\n\
    {\n\
        out_color.xyz = apply_lighting(in_color.xyz);\n\
        out_color.a   = in_color.a;\n\
    }\n";

const char* default_texture_vert =
    "#version 330 core\n\
    layout (location = 0) in vec3 position;\n\
    layout (location = 1) in vec2 texture_coord;\n\
    out vec2 tex_coord;\n\
    out vec2 world_pos;\n\
    uniform mat4 model;\n\
    uniform mat4 model_view_projection;\n\
    void main()\n\
    {\n\
       vec4 world_position = model * vec4(position.x, position.y, position.z, 1.0);\n\
       world_pos = world_position.xy;\n\
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
    in vec2 world_pos;\n\
    uniform samplerBuffer light_data;\n\
    uniform int light_count = 0;\n\
    float get_light_amount(vec4 light)\n\
    {\n\
       vec2 aux = light.xy - world_pos;\n\
       float dist = length(aux);\n\
       dist = dist / light.w;\n\
       float light_attenuation = 1.0 / (0.1 + 0.1 * dist + 0.1 * dist * dist);\n\
       return light_attenuation * light.z;\n\
    }\n\
    vec3 apply_lighting(vec3 object_color)\n\
    {\n\
       float ambient = use_ambient_light ? ambient_strength : 0.0;\n\
       float direct_light = 0.0;\n\
       bool any_light_active = false;\n\
       for (int i = 0; i < light_count; ++i)\n\
       {\n\
          vec4 light = texelFetch(light_data, i);\n\
          if (light.z > 0.0)\n\
          {\n\
             direct_light += get_light_amount(light);\n\
             any_light_active = true;\n\
          }\n\
       }\n\
       if (!any_light_active)\n\
       {\n\
          return object_color * (use_ambient_light ? ambient_strength : 1.0);\n\
       }\n\
       return clamp(object_color * (ambient + direct_light), 0.0, 1.0);\n\
    }\n\
    void main()\n\
    {\n\
       vec4 texture_color = texture(the_texture, tex_coord);\n\
       out_color.xyz = apply_lighting(texture_color.rgb);\n\
       out_color.a = texture_color.a * opacity;\n\
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

} // namespace

RenderContext* RenderContext::active_context_{nullptr};

RenderContext* RenderContext::get_active_context()
{
    return active_context_;
}

void RenderContext::init_default_shaders()
{
    shape_default_shader_   = Shader(default_shape_vert, default_shape_frag);
    texture_default_shader_ = Shader(default_texture_vert, default_texture_frag);
    text_default_shader_    = Shader(default_text_vert, default_text_frag);

    init_default_light_buffer();
    init_default_shader_lighting_uniforms();
}

void RenderContext::init_default_light_buffer()
{
    release_default_light_buffer();

    GL_CALL(glGenBuffers(1, &default_light_buffer_object_));
    GL_CALL(glGenTextures(1, &default_light_texture_object_));

    default_light_capacity_ = 1U;
    DefaultLightData empty_light{};

    GL_CALL(glBindBuffer(GL_TEXTURE_BUFFER, default_light_buffer_object_));
    GL_CALL(
        glBufferData(GL_TEXTURE_BUFFER, sizeof(DefaultLightData), &empty_light, GL_DYNAMIC_DRAW));

    GL_CALL(glActiveTexture(GL_TEXTURE1));
    GL_CALL(glBindTexture(GL_TEXTURE_BUFFER, default_light_texture_object_));
    GL_CALL(glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, default_light_buffer_object_));
    GL_CALL(glActiveTexture(GL_TEXTURE0));
}

void RenderContext::release_default_light_buffer()
{
    if (default_light_texture_object_ != 0U)
    {
        GL_CALL(glDeleteTextures(1, &default_light_texture_object_));
        default_light_texture_object_ = 0U;
    }

    if (default_light_buffer_object_ != 0U)
    {
        GL_CALL(glDeleteBuffers(1, &default_light_buffer_object_));
        default_light_buffer_object_ = 0U;
    }

    default_light_data_.clear();
    default_light_slots_.clear();
    default_light_capacity_         = 0U;
    default_light_count_            = 0U;
    default_light_dirty_begin_      = 0U;
    default_light_dirty_end_        = 0U;
    default_ambient_light_strength_ = 0.1F;
    default_light_data_dirty_       = false;
    default_light_uniforms_dirty_   = true;
    default_ambient_light_enabled_  = false;
}

void RenderContext::ensure_default_light_capacity(std::uint32_t light_count)
{
    if (light_count <= default_light_capacity_)
    {
        return;
    }

    std::uint32_t new_capacity = default_light_capacity_;
    if (new_capacity == 0U)
    {
        new_capacity = 1U;
    }

    while (new_capacity < light_count)
    {
        new_capacity *= 2U;
    }

    GL_CALL(glBindBuffer(GL_TEXTURE_BUFFER, default_light_buffer_object_));
    GL_CALL(glBufferData(GL_TEXTURE_BUFFER,
                         static_cast<GLsizeiptr>(new_capacity * sizeof(DefaultLightData)), nullptr,
                         GL_DYNAMIC_DRAW));

    if (!default_light_data_.empty())
    {
        GL_CALL(glBufferSubData(
            GL_TEXTURE_BUFFER, 0,
            static_cast<GLsizeiptr>(default_light_data_.size() * sizeof(DefaultLightData)),
            default_light_data_.data()));
    }

    GL_CALL(glActiveTexture(GL_TEXTURE1));
    GL_CALL(glBindTexture(GL_TEXTURE_BUFFER, default_light_texture_object_));
    GL_CALL(glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, default_light_buffer_object_));
    GL_CALL(glActiveTexture(GL_TEXTURE0));

    default_light_capacity_ = new_capacity;
}

void RenderContext::mark_default_light_data_dirty(std::uint32_t light_index)
{
    if (!default_light_data_dirty_)
    {
        default_light_dirty_begin_ = light_index;
        default_light_dirty_end_   = light_index + 1U;
        default_light_data_dirty_  = true;
        return;
    }

    default_light_dirty_begin_ = std::min(default_light_dirty_begin_, light_index);
    default_light_dirty_end_   = std::max(default_light_dirty_end_, light_index + 1U);
}

void RenderContext::bind_default_light_buffer() const
{
    if (default_light_texture_object_ == 0U)
    {
        return;
    }

    GL_CALL(glActiveTexture(GL_TEXTURE1));
    GL_CALL(glBindTexture(GL_TEXTURE_BUFFER, default_light_texture_object_));
    GL_CALL(glActiveTexture(GL_TEXTURE0));
}

void RenderContext::init_default_shader_lighting_uniforms() const
{
    bind_default_light_buffer();

    const auto shape_shader = get_shape_default_shader();
    shape_shader.use();
    shape_shader.set_int("light_data", static_cast<std::int32_t>(LIGHT_TEXTURE_UNIT));

    const auto texture_shader = get_texture_default_shader();
    texture_shader.use();
    texture_shader.set_int("the_texture", 0);
    texture_shader.set_int("light_data", static_cast<std::int32_t>(LIGHT_TEXTURE_UNIT));

    update_default_lighting_uniforms();
}

void RenderContext::update_default_lighting_uniforms() const
{
    const auto shape_shader = get_shape_default_shader();
    shape_shader.use();
    shape_shader.set_int("light_count", static_cast<std::int32_t>(default_light_count_));
    shape_shader.set_bool("use_ambient_light", default_ambient_light_enabled_);
    shape_shader.set_float("ambient_strength", default_ambient_light_strength_);

    const auto texture_shader = get_texture_default_shader();
    texture_shader.use();
    texture_shader.set_int("light_count", static_cast<std::int32_t>(default_light_count_));
    texture_shader.set_bool("use_ambient_light", default_ambient_light_enabled_);
    texture_shader.set_float("ambient_strength", default_ambient_light_strength_);
}

void RenderContext::sync_default_lighting()
{
    bind_default_light_buffer();

    if (default_light_data_dirty_)
    {
        const std::uint32_t dirty_count = default_light_dirty_end_ - default_light_dirty_begin_;
        GL_CALL(glBindBuffer(GL_TEXTURE_BUFFER, default_light_buffer_object_));
        GL_CALL(glBufferSubData(
            GL_TEXTURE_BUFFER,
            static_cast<GLintptr>(default_light_dirty_begin_ * sizeof(DefaultLightData)),
            static_cast<GLsizeiptr>(dirty_count * sizeof(DefaultLightData)),
            default_light_data_.data() + default_light_dirty_begin_));

        default_light_dirty_begin_ = 0U;
        default_light_dirty_end_   = 0U;
        default_light_data_dirty_  = false;
    }

    if (default_light_uniforms_dirty_)
    {
        update_default_lighting_uniforms();
        default_light_uniforms_dirty_ = false;
    }
}

std::uint32_t RenderContext::get_default_light_count() const
{
    std::uint32_t light_count{0U};

    for (std::size_t i{0U}; i < default_light_slots_.size(); ++i)
    {
        if (default_light_slots_.at(i).owner_ != nullptr && default_light_slots_.at(i).active_)
        {
            light_count = static_cast<std::uint32_t>(i + 1U);
        }
    }

    return light_count;
}

void RenderContext::init(const Application* application)
{
    active_context_ = this;

    init_default_shaders();
    GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GL_CALL(glEnable(GL_BLEND));
    projection_ = glm::ortho(0.0F, static_cast<float>(get_width()),
                             static_cast<float>(get_height()), 0.0F, -1.0F, 1.0F);

    model_view_projection_ = projection_ * view_;
    application_           = application;
}

void RenderContext::shutdown()
{
    release_default_light_buffer();

    shape_default_shader_   = Shader{};
    texture_default_shader_ = Shader{};
    text_default_shader_    = Shader{};
    application_            = nullptr;

    if (active_context_ == this)
    {
        active_context_ = nullptr;
    }
}

void RenderContext::set_viewport(std::int32_t x, std::int32_t y, std::int32_t width,
                                 std::int32_t heigth)
{
    width_  = width;
    height_ = heigth;
    GL_CALL(glViewport(x, y, width, heigth));
}

void RenderContext::clear_screen(float r, float g, float b, float a)
{
    GL_CALL(glClearColor(r, g, b, a));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

std::uint32_t RenderContext::get_shape_default_shader_id() const
{
    return shape_default_shader_.get_id();
}

std::uint32_t RenderContext::get_texture_default_shader_id() const
{
    return texture_default_shader_.get_id();
}

std::uint32_t RenderContext::get_text_default_shader_id() const
{
    return text_default_shader_.get_id();
}

const Shader RenderContext::get_shape_default_shader() const
{
    return shape_default_shader_;
}

const Shader RenderContext::get_texture_default_shader() const
{
    return texture_default_shader_;
}

const Shader RenderContext::get_text_default_shader() const
{
    return text_default_shader_;
}

void RenderContext::set_default_light(std::uint32_t light_index, float position_x, float position_y,
                                      float intensity, float falloff)
{
    const auto required_light_count = light_index + 1U;
    if (required_light_count > default_light_data_.size())
    {
        default_light_data_.resize(required_light_count);
    }

    ensure_default_light_capacity(required_light_count);

    default_light_data_.at(light_index) =
        DefaultLightData{position_x, position_y, intensity, falloff};

    mark_default_light_data_dirty(light_index);
}

std::int32_t RenderContext::acquire_default_light_slot(const Light* light, bool active)
{
    for (std::size_t i{0U}; i < default_light_slots_.size(); ++i)
    {
        if (default_light_slots_.at(i).owner_ == nullptr)
        {
            default_light_slots_.at(i).owner_  = light;
            default_light_slots_.at(i).active_ = active;
            set_default_light_count(get_default_light_count());
            return static_cast<std::int32_t>(i);
        }
    }

    default_light_slots_.push_back(DefaultLightSlot{light, active});
    set_default_light_count(get_default_light_count());
    return static_cast<std::int32_t>(default_light_slots_.size() - 1U);
}

void RenderContext::replace_default_light_slot(std::int32_t light_index, const Light* old_light,
                                               const Light* new_light)
{
    if (!owns_default_light_slot(light_index, old_light))
    {
        return;
    }

    default_light_slots_.at(static_cast<std::size_t>(light_index)).owner_ = new_light;
}

void RenderContext::release_default_light_slot(std::int32_t light_index, const Light* light)
{
    if (!owns_default_light_slot(light_index, light))
    {
        return;
    }

    const auto slot_index                       = static_cast<std::size_t>(light_index);
    default_light_slots_.at(slot_index).owner_  = nullptr;
    default_light_slots_.at(slot_index).active_ = false;

    if (static_cast<std::uint32_t>(light_index) < default_light_data_.size())
    {
        set_default_light(static_cast<std::uint32_t>(light_index), 0.0F, 0.0F, 0.0F, 1.0F);
    }

    set_default_light_count(get_default_light_count());
}

void RenderContext::set_default_light_active(std::int32_t light_index, const Light* light,
                                             bool active)
{
    if (!owns_default_light_slot(light_index, light))
    {
        return;
    }

    default_light_slots_.at(static_cast<std::size_t>(light_index)).active_ = active;
    set_default_light_count(get_default_light_count());
}

bool RenderContext::owns_default_light_slot(std::int32_t light_index, const Light* light) const
{
    if (light_index < 0 || static_cast<std::size_t>(light_index) >= default_light_slots_.size())
    {
        return false;
    }

    return default_light_slots_.at(static_cast<std::size_t>(light_index)).owner_ == light;
}

void RenderContext::set_default_light_count(std::uint32_t light_count)
{
    if (default_light_count_ == light_count)
    {
        return;
    }

    default_light_count_          = light_count;
    default_light_uniforms_dirty_ = true;
}

void RenderContext::set_default_ambient_light(bool enabled, float strength)
{
    strength = std::clamp(strength, 0.0F, 1.0F);

    if (default_ambient_light_enabled_ == enabled && default_ambient_light_strength_ == strength)
    {
        return;
    }

    default_ambient_light_enabled_  = enabled;
    default_ambient_light_strength_ = strength;
    default_light_uniforms_dirty_   = true;
}

std::int32_t RenderContext::get_width() const
{
    return width_;
}

std::int32_t RenderContext::get_height() const
{
    return height_;
}

float RenderContext::get_opengl_x_coord(float absolute_coord) const
{
    return ((absolute_coord / width_) * 2.0F) - 1.0F;
}

float RenderContext::get_opengl_y_coord(float absolute_coord) const
{
    return -1.0F * (((absolute_coord / height_) * 2.0F) - 1.0F);
}

void RenderContext::update_mvp_matrix(const glm::mat4& model, std::uint32_t shader_id)
{
    sync_default_lighting();
    GL_CALL(glUseProgram(shader_id));

    projection_ = glm::ortho(0.0F, static_cast<float>(get_width()),
                             static_cast<float>(get_height()), 0.0F, -1.0F, 1.0F);

    model_view_projection_      = projection_ * view_ * model;
    std::int32_t model_location = glGetUniformLocation(shader_id, "model");
    rinvid::errors::handle_gl_errors(__FILE__, __LINE__);
    if (model_location != -1)
    {
        GL_CALL(glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model)));
    }

    std::int32_t mvp_location = glGetUniformLocation(shader_id, "model_view_projection");
    rinvid::errors::handle_gl_errors(__FILE__, __LINE__);
    if (mvp_location == -1)
    {
        rinvid::errors::put_error_to_log("glGetUniformLocation error: invalid uniform name");
        return;
    }
    GL_CALL(glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(model_view_projection_)));
}

void RenderContext::update_view(const glm::mat4& view)
{
    view_ = view;
}

const glm::mat4& RenderContext::get_view() const
{
    return view_;
}

void RenderContext::use_shape_default_shader() const
{
    shape_default_shader_.use();
}

void RenderContext::use_texture_default_shader() const
{
    texture_default_shader_.use();
}

void RenderContext::use_text_default_shader() const
{
    text_default_shader_.use();
}

const Application* RenderContext::get_application() const
{
    return application_;
}

} // namespace rinvid
