/**********************************************************************
 * Copyright (c) 2022 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <string>
#include <vector>
#include <unordered_map>

#include <rinvid/core/shader.h>
#include <rinvid/util/error.h>

namespace rinvid
{

namespace
{

const char* get_shader_type_name(GLenum shader_type)
{
    switch (shader_type)
    {
        case GL_VERTEX_SHADER:
            return "vertex";
        case GL_FRAGMENT_SHADER:
            return "fragment";
        default:
            return "unknown";
    }
}

std::string get_shader_info_log(std::uint32_t shader_id)
{
    GLint info_log_length{};
    GL_CALL(glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length));
    if (info_log_length <= 0)
    {
        return "No shader info log available";
    }

    std::vector<char> info_log(static_cast<std::size_t>(info_log_length));
    GL_CALL(glGetShaderInfoLog(shader_id, info_log_length, NULL, info_log.data()));

    return std::string{info_log.data()};
}

std::string get_program_info_log(std::uint32_t program_id)
{
    GLint info_log_length{};
    GL_CALL(glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length));
    if (info_log_length <= 0)
    {
        return "No program info log available";
    }

    std::vector<char> info_log(static_cast<std::size_t>(info_log_length));
    GL_CALL(glGetProgramInfoLog(program_id, info_log_length, NULL, info_log.data()));

    return std::string{info_log.data()};
}

void validate_shader_compile_status(std::uint32_t shader_id, GLenum shader_type)
{
    GLint success{};
    GL_CALL(glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success));
    if (success == GL_TRUE)
    {
        return;
    }

    throw GraphicsError{"Shader: " + std::string{get_shader_type_name(shader_type)} +
                        " shader compilation failed: " + get_shader_info_log(shader_id)};
}

void validate_program_link_status(std::uint32_t program_id)
{
    GLint success{};
    GL_CALL(glGetProgramiv(program_id, GL_LINK_STATUS, &success));
    if (success == GL_TRUE)
    {
        return;
    }

    throw GraphicsError{"Shader: program linking failed: " + get_program_info_log(program_id)};
}

std::uint32_t create_shader(GLenum shader_type, const char* shader_code)
{
    if (shader_code == nullptr)
    {
        throw GraphicsError{"Shader: " + std::string{get_shader_type_name(shader_type)} +
                            " shader source is null"};
    }

    const auto shader_id = glCreateShader(shader_type);
    if (shader_id == 0U)
    {
        throw GraphicsError{"Shader: could not create " +
                            std::string{get_shader_type_name(shader_type)} + " shader"};
    }

    try
    {
        GL_CALL(glShaderSource(shader_id, 1, &shader_code, NULL));
        GL_CALL(glCompileShader(shader_id));
        validate_shader_compile_status(shader_id, shader_type);
    }
    catch (...)
    {
        GL_CALL(glDeleteShader(shader_id));
        throw;
    }

    return shader_id;
}

void put_invalid_uniform_error_to_log(const std::string& name)
{
    rinvid::errors::put_error_to_log("glGetUniformLocation error: invalid uniform name '" + name +
                                     "'");
}

} // namespace

struct Shader::ProgramHandle
{
    ~ProgramHandle()
    {
        if (id_ != 0U)
        {
            glDeleteProgram(id_);
        }
    }

    std::uint32_t                                         id_{};
    mutable std::unordered_map<std::string, std::int32_t> uniform_locations_{};
};

Shader::Shader(const char* vert_code, const char* frag_code)
    : program_handle_{std::make_shared<ProgramHandle>()}
{
    std::uint32_t vert_handle{};
    std::uint32_t frag_handle{};

    try
    {
        vert_handle = create_shader(GL_VERTEX_SHADER, vert_code);
        frag_handle = create_shader(GL_FRAGMENT_SHADER, frag_code);
    }
    catch (...)
    {
        if (vert_handle != 0U)
        {
            GL_CALL(glDeleteShader(vert_handle));
        }

        if (frag_handle != 0U)
        {
            GL_CALL(glDeleteShader(frag_handle));
        }

        throw;
    }

    program_handle_->id_ = glCreateProgram();
    if (program_handle_->id_ == 0U)
    {
        GL_CALL(glDeleteShader(vert_handle));
        GL_CALL(glDeleteShader(frag_handle));
        throw GraphicsError{"Shader: could not create shader program"};
    }

    GL_CALL(glAttachShader(program_handle_->id_, vert_handle));
    GL_CALL(glAttachShader(program_handle_->id_, frag_handle));
    GL_CALL(glLinkProgram(program_handle_->id_));
    try
    {
        validate_program_link_status(program_handle_->id_);
    }
    catch (...)
    {
        GL_CALL(glDeleteShader(vert_handle));
        GL_CALL(glDeleteShader(frag_handle));
        throw;
    }

    GL_CALL(glDeleteShader(vert_handle));
    GL_CALL(glDeleteShader(frag_handle));
}

void Shader::use() const
{
    glUseProgram(get_id());
}

void Shader::set_bool(const std::string& name, bool value) const
{
    std::int32_t location = get_uniform_location(name);
    if (location == -1)
    {
        put_invalid_uniform_error_to_log(name);
        return;
    }
    GL_CALL(glUniform1i(location, static_cast<std::int32_t>(value)));
}

void Shader::set_int(const std::string& name, std::int32_t value) const
{
    std::int32_t location = get_uniform_location(name);
    if (location == -1)
    {
        put_invalid_uniform_error_to_log(name);
        return;
    }
    GL_CALL(glUniform1i(location, value));
}

void Shader::set_float(const std::string& name, float value) const
{
    std::int32_t location = get_uniform_location(name);
    if (location == -1)
    {
        put_invalid_uniform_error_to_log(name);
        return;
    }
    GL_CALL(glUniform1f(location, value));
}

void Shader::set_float2(const std::string& name, float value1, float value2) const
{
    std::int32_t location = get_uniform_location(name);
    if (location == -1)
    {
        put_invalid_uniform_error_to_log(name);
        return;
    }
    GL_CALL(glUniform2f(location, value1, value2));
}

void Shader::set_float4(const std::string& name, float value1, float value2, float value3,
                        float value4) const
{
    std::int32_t location = get_uniform_location(name);
    if (location == -1)
    {
        put_invalid_uniform_error_to_log(name);
        return;
    }
    GL_CALL(glUniform4f(location, value1, value2, value3, value4));
}

std::int32_t Shader::get_uniform_location(const std::string& name) const
{
    if (!program_handle_)
    {
        return -1;
    }

    auto location = program_handle_->uniform_locations_.find(name);
    if (location != program_handle_->uniform_locations_.end())
    {
        return location->second;
    }

    const std::int32_t resolved_location = glGetUniformLocation(get_id(), name.c_str());
    program_handle_->uniform_locations_.emplace(name, resolved_location);
    return resolved_location;
}

std::uint32_t Shader::get_id() const
{
    if (!program_handle_)
    {
        return 0U;
    }

    return program_handle_->id_;
}

} // namespace rinvid
