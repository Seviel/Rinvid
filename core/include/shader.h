/**********************************************************************
 * Copyright (c) 2022 - 2023, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_SHADER_H
#define CORE_INCLUDE_SHADER_H

#include <string>

#include "core/include/rinvid_gl.h"
#include "util/include/error_handler.h"

class Shader
{
  public:
    /**************************************************************************************************
     * @brief Default shader constructor.
     *
     *************************************************************************************************/
    Shader()
    {
    }

    /**************************************************************************************************
     * @brief Shader constructor.
     *
     * @param vert_path Path to vertex shader source file.
     * @param frag_path Path to fragment shader source file.
     *
     *************************************************************************************************/
    Shader(const char* vert_path, const char* frag_path);

    /**************************************************************************************************
     * @brief Use this shader program.
     *
     *************************************************************************************************/
    void use() const;

    /**************************************************************************************************
     * @brief Sets bool uniform.
     *
     * @param name Name of the uniform to set.
     * @param value Value to be set.
     *
     *************************************************************************************************/
    void set_bool(const std::string& name, bool value) const;

    /**************************************************************************************************
     * @brief Sets int uniform.
     *
     * @param name Name of the uniform to set.
     * @param value Value to be set.
     *
     *************************************************************************************************/
    void set_int(const std::string& name, std::int32_t value) const;

    /**************************************************************************************************
     * @brief Sets float uniform.
     *
     * @param name Name of the uniform to set.
     * @param value Value to be set.
     *
     *************************************************************************************************/
    void set_float(const std::string& name, float value) const;

    /**************************************************************************************************
     * @brief Sets float2 uniform.
     *
     * @param name Name of the uniform to set.
     * @param value1 First value to be set.
     * @param value2 Second value to be set.
     *
     *************************************************************************************************/
    void set_float2(const std::string& name, float value1, float value2) const;

    /**************************************************************************************************
     * @brief Sets float4 uniform.
     *
     * @param name Name of the uniform to set.
     * @param value1 First value to be set.
     * @param value2 Second value to be set.
     * @param value3 Third value to be set.
     * @param value4 Fourth value to be set.
     *
     *************************************************************************************************/
    void set_float4(const std::string& name, float value1, float value2, float value3,
                    float value4) const;

    std::uint32_t get_id() const;

  private:
    std::uint32_t id_;
};

#endif // CORE_INCLUDE_SHADER_H
