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

Shader::Shader(const char* vert_code, const char* frag_code)
{
    unsigned int vert_handle, frag_handle;
    vert_handle = glCreateShader(GL_VERTEX_SHADER);
    GL_CALL(glShaderSource(vert_handle, 1, &vert_code, NULL));
    GL_CALL(glCompileShader(vert_handle));
    frag_handle = glCreateShader(GL_FRAGMENT_SHADER);
    GL_CALL(glShaderSource(frag_handle, 1, &frag_code, NULL));
    glCompileShader(frag_handle);
    id_ = glCreateProgram();
    GL_CALL(glAttachShader(id_, vert_handle));
    GL_CALL(glAttachShader(id_, frag_handle));
    GL_CALL(glLinkProgram(id_));
    GL_CALL(glDeleteShader(vert_handle));
    GL_CALL(glDeleteShader(frag_handle));
}

void Shader::use() const
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
