/**********************************************************************
 * Copyright (c) 2020 - 2023, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_RINFID_GFX_H
#define CORE_INCLUDE_RINFID_GFX_H

#include <cstdint>
#include <memory>

#include "core/include/rinvid_gl.h"
#include "core/include/shader.h"
#include "extern/glm/glm/mat4x4.hpp"

namespace rinvid
{

/**************************************************************************************************
 * @brief A class that holds global objects and functions.
 *
 * All functions and members are static.
 *
 *************************************************************************************************/
class RinvidGfx
{
  public:
    /**************************************************************************************************
     * @brief Default constructor
     *
     *************************************************************************************************/
    RinvidGfx()
    {
    }

    /**************************************************************************************************
     * @brief Initializes Rinvid internal state.
     *
     * Should be called once, before any other Rinvid graphics related functions.
     *
     *************************************************************************************************/
    static void init();

    /**************************************************************************************************
     * @brief Sets viewport position and size.
     *
     * Basically a wrapper around glViewport.
     *
     *************************************************************************************************/
    static void set_viewport(std::int32_t x, std::int32_t y, std::int32_t width,
                             std::int32_t heigth);

    /**************************************************************************************************
     * @brief Clears screen
     *
     * @param r red component of the color with which to clear screen
     * @param g green component of the color with which to clear screen
     * @param b blue component of the color with which to clear screen
     * @param a alpha component of the color with which to clear screen
     *
     *************************************************************************************************/
    static void clear_screen(float r, float g, float b, float a);

    /**************************************************************************************************
     * @brief Returns default shape shader handle. Should only be used internally in Rinvid.
     *
     * @return An OpenGl handle to the shader
     *
     *************************************************************************************************/
    static std::uint32_t get_shape_default_shader_id();

    /**************************************************************************************************
     * @brief Returns default texture shader handle. Should only be used internally in Rinvid.
     *
     * @return An OpenGl handle to the shader
     *
     *************************************************************************************************/
    static std::uint32_t get_texture_default_shader_id();

    /**************************************************************************************************
     * @brief Returns default text shader handle. Should only be used internally in Rinvid.
     *
     * @return An OpenGl handle to the shader
     *
     *************************************************************************************************/
    static std::uint32_t get_text_default_shader_id();

    /**************************************************************************************************
     * @brief Returns default shape shader object.
     *
     * @return A shared ptr to default shape Shader object.
     *
     *************************************************************************************************/
    static const Shader get_shape_default_shader();

    /**************************************************************************************************
     * @brief Returns default texture shader object.
     *
     * @return A shared ptr to default texture Shader object.
     *
     *************************************************************************************************/
    static const Shader get_texture_default_shader();

    /**************************************************************************************************
     * @brief Returns default text shader object.
     *
     * @return A default text Shader object.
     *
     *************************************************************************************************/
    static const Shader get_text_default_shader();

    /**************************************************************************************************
     * @brief Returns screen width.
     *
     * @return Screen width
     *
     *************************************************************************************************/
    static std::int32_t get_width();

    /**************************************************************************************************
     * @brief Returns screen height.
     *
     * @return Screen height
     *
     *************************************************************************************************/
    static std::int32_t get_height();

    /**************************************************************************************************
     * @brief Returns normalized x coordinate. Intended for internal Rinvid use.
     *
     * @param absolute_coord an absolute x axis coordinate in pixels (origin assumed to be in top
     * left corner)
     *
     * @return Normalized x coordinate ([-1, 1] range)
     *
     *************************************************************************************************/
    static float get_opengl_x_coord(float absolute_coord)
    {
        return ((absolute_coord / width_) * 2.0F) - 1.0F;
    }

    /**************************************************************************************************
     * @brief Returns normalized y coordinate. Intended for internal Rinvid use.
     *
     * @param absolute_coord an absolute y axis coordinate in pixels (origin assumed to be in top
     * left corner)
     *
     * @return Normalized y coordinate ([-1, 1] range)
     *
     *************************************************************************************************/
    static float get_opengl_y_coord(float absolute_coord)
    {
        return -1.0F * (((absolute_coord / height_) * 2.0F) - 1.0F);
    }

    /**************************************************************************************************
     * @brief Updates model view projection matrix.
     *
     * @param model A model matrix to apply
     *
     *************************************************************************************************/
    static void update_mvp_matrix(const glm::mat4& model, std::uint32_t shader_id);

    /**************************************************************************************************
     * @brief Updates view matrix.
     *
     * @param view A view matrix to apply
     *
     *************************************************************************************************/
    static void update_view(const glm::mat4& model);

    /**************************************************************************************************
     * @brief Returns the view matrix.
     *
     * @return View matrix
     *
     *************************************************************************************************/
    static const glm::mat4& get_view();

    /**************************************************************************************************
     * @brief Use the shape default shader.
     *
     *************************************************************************************************/
    static void use_shape_default_shader();

    /**************************************************************************************************
     * @brief Use the texture default shader.
     *
     *************************************************************************************************/
    static void use_texture_default_shader();

    /**************************************************************************************************
     * @brief Use the texture default shader.
     *
     *************************************************************************************************/
    static void use_text_default_shader();

  private:
    static void init_default_shaders();

    static glm::mat4    model_view_projection_;
    static glm::mat4    view_;
    static glm::mat4    projection_;
    static Shader       shape_default_shader_;
    static Shader       texture_default_shader_;
    static Shader       text_default_shader_;
    static std::int32_t width_;
    static std::int32_t height_;
};

} // namespace rinvid

#endif // CORE_INCLUDE_RINFID_GFX_H
