/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_CORE_RENDER_CONTEXT_H
#define INCLUDE_RINVID_CORE_RENDER_CONTEXT_H

#include <cstdint>

#include <glm/mat4x4.hpp>

#include <rinvid/core/shader.h>

namespace rinvid
{

class Application;

/**************************************************************************************************
 * @brief Owns render state for one application context.
 *
 *************************************************************************************************/
class RenderContext
{
  public:
    /**************************************************************************************************
     * @brief Default constructor.
     *
     *************************************************************************************************/
    RenderContext() = default;

    /**************************************************************************************************
     * @brief Copy constructor deleted.
     *
     *************************************************************************************************/
    RenderContext(const RenderContext& other) = delete;

    /**************************************************************************************************
     * @brief Copy assignment operator deleted.
     *
     *************************************************************************************************/
    RenderContext& operator=(const RenderContext& other) = delete;

    /**************************************************************************************************
     * @brief Move constructor deleted.
     *
     *************************************************************************************************/
    RenderContext(RenderContext&& other) = delete;

    /**************************************************************************************************
     * @brief Move assignment operator deleted.
     *
     *************************************************************************************************/
    RenderContext& operator=(RenderContext&& other) = delete;

    /**************************************************************************************************
     * @brief Destructor.
     *
     *************************************************************************************************/
    ~RenderContext() = default;

    /**************************************************************************************************
     * @brief Returns currently active render context.
     *
     * @return Active render context.
     *
     *************************************************************************************************/
    static RenderContext* get_active_context();

    /**************************************************************************************************
     * @brief Initializes render state.
     *
     * @param application Application that owns this render context.
     *
     *************************************************************************************************/
    void init(const Application* application);

    /**************************************************************************************************
     * @brief Releases render resources owned by this context.
     *
     *************************************************************************************************/
    void shutdown();

    /**************************************************************************************************
     * @brief Sets viewport position and size.
     *
     * Basically a wrapper around glViewport.
     *
     *************************************************************************************************/
    void set_viewport(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t heigth);

    /**************************************************************************************************
     * @brief Clears screen.
     *
     * @param r red component of the color with which to clear screen
     * @param g green component of the color with which to clear screen
     * @param b blue component of the color with which to clear screen
     * @param a alpha component of the color with which to clear screen
     *
     *************************************************************************************************/
    void clear_screen(float r, float g, float b, float a);

    /**************************************************************************************************
     * @brief Returns default shape shader handle. Should only be used internally in Rinvid.
     *
     * @return An OpenGl handle to the shader.
     *
     *************************************************************************************************/
    std::uint32_t get_shape_default_shader_id() const;

    /**************************************************************************************************
     * @brief Returns default texture shader handle. Should only be used internally in Rinvid.
     *
     * @return An OpenGl handle to the shader.
     *
     *************************************************************************************************/
    std::uint32_t get_texture_default_shader_id() const;

    /**************************************************************************************************
     * @brief Returns default text shader handle. Should only be used internally in Rinvid.
     *
     * @return An OpenGl handle to the shader.
     *
     *************************************************************************************************/
    std::uint32_t get_text_default_shader_id() const;

    /**************************************************************************************************
     * @brief Returns default shape shader object.
     *
     * @return Default shape Shader object.
     *
     *************************************************************************************************/
    const Shader get_shape_default_shader() const;

    /**************************************************************************************************
     * @brief Returns default texture shader object.
     *
     * @return Default texture Shader object.
     *
     *************************************************************************************************/
    const Shader get_texture_default_shader() const;

    /**************************************************************************************************
     * @brief Returns default text shader object.
     *
     * @return Default text Shader object.
     *
     *************************************************************************************************/
    const Shader get_text_default_shader() const;

    /**************************************************************************************************
     * @brief Returns screen width.
     *
     * @return Screen width.
     *
     *************************************************************************************************/
    std::int32_t get_width() const;

    /**************************************************************************************************
     * @brief Returns screen height.
     *
     * @return Screen height.
     *
     *************************************************************************************************/
    std::int32_t get_height() const;

    /**************************************************************************************************
     * @brief Returns normalized x coordinate. Intended for internal Rinvid use.
     *
     * @param absolute_coord an absolute x axis coordinate in pixels (origin assumed to be in top
     * left corner)
     *
     * @return Normalized x coordinate ([-1, 1] range).
     *
     *************************************************************************************************/
    float get_opengl_x_coord(float absolute_coord) const;

    /**************************************************************************************************
     * @brief Returns normalized y coordinate. Intended for internal Rinvid use.
     *
     * @param absolute_coord an absolute y axis coordinate in pixels (origin assumed to be in top
     * left corner)
     *
     * @return Normalized y coordinate ([-1, 1] range).
     *
     *************************************************************************************************/
    float get_opengl_y_coord(float absolute_coord) const;

    /**************************************************************************************************
     * @brief Updates model view projection matrix.
     *
     * @param model A model matrix to apply.
     * @param shader_id Shader program id to receive the updated matrix.
     *
     *************************************************************************************************/
    void update_mvp_matrix(const glm::mat4& model, std::uint32_t shader_id);

    /**************************************************************************************************
     * @brief Updates view matrix.
     *
     * @param view A view matrix to apply.
     *
     *************************************************************************************************/
    void update_view(const glm::mat4& view);

    /**************************************************************************************************
     * @brief Returns the view matrix.
     *
     * @return View matrix.
     *
     *************************************************************************************************/
    const glm::mat4& get_view() const;

    /**************************************************************************************************
     * @brief Use the shape default shader.
     *
     *************************************************************************************************/
    void use_shape_default_shader() const;

    /**************************************************************************************************
     * @brief Use the texture default shader.
     *
     *************************************************************************************************/
    void use_texture_default_shader() const;

    /**************************************************************************************************
     * @brief Use the text default shader.
     *
     *************************************************************************************************/
    void use_text_default_shader() const;

    /**************************************************************************************************
     * @brief Returns application that owns this render context.
     *
     * @return Application.
     *
     *************************************************************************************************/
    const Application* get_application() const;

  private:
    static RenderContext* active_context_;

    void init_default_shaders();

    glm::mat4          model_view_projection_{1.0F};
    glm::mat4          view_{1.0F};
    glm::mat4          projection_{1.0F};
    Shader             shape_default_shader_{};
    Shader             texture_default_shader_{};
    Shader             text_default_shader_{};
    std::int32_t       width_{};
    std::int32_t       height_{};
    const Application* application_{nullptr};
};

} // namespace rinvid

#endif // INCLUDE_RINVID_CORE_RENDER_CONTEXT_H
