/**********************************************************************
 * Copyright (c) 2022 - 2023, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <fstream>
#include <iostream>
#include <sstream>

#include "core/include/shader.h"

Shader::Shader(const char* vert_path, const char* frag_path)
{
    std::string   vert_code;
    std::string   frag_code;
    std::ifstream vert_shader_file;
    std::ifstream frag_shader_file;
    vert_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    frag_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vert_shader_file.open(vert_path);
        frag_shader_file.open(frag_path);
        std::stringstream vert_stream, frag_stream;
        vert_stream << vert_shader_file.rdbuf();
        frag_stream << frag_shader_file.rdbuf();
        vert_shader_file.close();
        frag_shader_file.close();
        vert_code = vert_stream.str();
        frag_code = frag_stream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
    }

    const char*  vert_code_raw = vert_code.c_str();
    const char*  frag_code_raw = frag_code.c_str();
    unsigned int vert_handle, frag_handle;
    vert_handle = glCreateShader(GL_VERTEX_SHADER);
    GL_CALL(glShaderSource(vert_handle, 1, &vert_code_raw, NULL));
    GL_CALL(glCompileShader(vert_handle));
    frag_handle = glCreateShader(GL_FRAGMENT_SHADER);
    GL_CALL(glShaderSource(frag_handle, 1, &frag_code_raw, NULL));
    glCompileShader(frag_handle);
    id_ = glCreateProgram();
    GL_CALL(glAttachShader(id_, vert_handle));
    GL_CALL(glAttachShader(id_, frag_handle));
    GL_CALL(glLinkProgram(id_));
    GL_CALL(glDeleteShader(vert_handle));
    GL_CALL(glDeleteShader(frag_handle));
}

void Shader::use()
{
    glUseProgram(id_);
}

void Shader::set_bool(const std::string& name, bool value) const
{
    std::int32_t location = glGetUniformLocation(id_, name.c_str());
    if (location == -1)
    {
        rinvid::errors::put_error_to_log("glGetUniformLocation error: invalid uniform name");
        return;
    }
    GL_CALL(glUniform1i(location, static_cast<std::int32_t>(value)));
}

void Shader::set_int(const std::string& name, std::int32_t value) const
{
    std::int32_t location = glGetUniformLocation(id_, name.c_str());
    if (location == -1)
    {
        rinvid::errors::put_error_to_log("glGetUniformLocation error: invalid uniform name");
        return;
    }
    GL_CALL(glUniform1i(location, value));
}

void Shader::set_float(const std::string& name, float value) const
{
    std::int32_t location = glGetUniformLocation(id_, name.c_str());
    if (location == -1)
    {
        rinvid::errors::put_error_to_log("glGetUniformLocation error: invalid uniform name");
        return;
    }
    GL_CALL(glUniform1f(location, value));
}

void Shader::set_float2(const std::string& name, float value1, float value2) const
{
    std::int32_t location = glGetUniformLocation(id_, name.c_str());
    if (location == -1)
    {
        rinvid::errors::put_error_to_log("glGetUniformLocation error: invalid uniform name");
        return;
    }
    GL_CALL(glUniform2f(location, value1, value2));
}

void Shader::set_float4(const std::string& name, float value1, float value2, float value3,
                        float value4) const
{
    std::int32_t location = glGetUniformLocation(id_, name.c_str());
    if (location == -1)
    {
        rinvid::errors::put_error_to_log("glGetUniformLocation error: invalid uniform name");
        return;
    }
    GL_CALL(glUniform4f(location, value1, value2, value3, value4));
}

std::uint32_t Shader::get_id() const
{
    return id_;
}
