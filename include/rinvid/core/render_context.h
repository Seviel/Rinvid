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
#include <vector>

#include <glm/mat4x4.hpp>

#include <rinvid/core/shader.h>

namespace rinvid
{

class Application;
class Light;

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
     * @brief Updates default shader light data. Intended for internal Rinvid use.
     *
     * @param light_index Index of the light in the default light buffer.
     * @param position_x World-space X coordinate.
     * @param position_y World-space Y coordinate.
     * @param intensity Light intensity used by the shader.
     * @param falloff Light falloff used by the shader.
     *
     *************************************************************************************************/
    void set_default_light(std::uint32_t light_index, float position_x, float position_y,
                           float intensity, float falloff);

    /**************************************************************************************************
     * @brief Acquires a default shader light slot. Intended for internal Rinvid use.
     *
     * @param light Light that owns the slot.
     * @param active Whether the light should contribute to lighting.
     *
     * @return Acquired light slot index.
     *
     *************************************************************************************************/
    std::int32_t acquire_default_light_slot(const Light* light, bool active);

    /**************************************************************************************************
     * @brief Replaces the owner of a default shader light slot. Intended for internal Rinvid use.
     *
     * @param light_index Light slot index.
     * @param old_light Previous owner of the slot.
     * @param new_light New owner of the slot.
     *
     *************************************************************************************************/
    void replace_default_light_slot(std::int32_t light_index, const Light* old_light,
                                    const Light* new_light);

    /**************************************************************************************************
     * @brief Releases a default shader light slot. Intended for internal Rinvid use.
     *
     * @param light_index Light slot index.
     * @param light Light that owns the slot.
     *
     *************************************************************************************************/
    void release_default_light_slot(std::int32_t light_index, const Light* light);

    /**************************************************************************************************
     * @brief Updates default shader light activity. Intended for internal Rinvid use.
     *
     * @param light_index Light slot index.
     * @param light Light that owns the slot.
     * @param active Whether the light should contribute to lighting.
     *
     *************************************************************************************************/
    void set_default_light_active(std::int32_t light_index, const Light* light, bool active);

    /**************************************************************************************************
     * @brief Checks whether a default shader light slot belongs to a light.
     *
     * @param light_index Light slot index.
     * @param light Light that may own the slot.
     *
     * @return True if the slot belongs to light.
     *
     *************************************************************************************************/
    bool owns_default_light_slot(std::int32_t light_index, const Light* light) const;

    /**************************************************************************************************
     * @brief Updates number of lights read by default shaders. Intended for internal Rinvid use.
     *
     * @param light_count Number of light entries to read.
     *
     *************************************************************************************************/
    void set_default_light_count(std::uint32_t light_count);

    /**************************************************************************************************
     * @brief Updates default shader ambient lighting. Intended for internal Rinvid use.
     *
     * @param enabled Whether ambient lighting is enabled.
     * @param strength Ambient light strength.
     *
     *************************************************************************************************/
    void set_default_ambient_light(bool enabled, float strength);

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

    struct DefaultLightData
    {
        float position_x_{};
        float position_y_{};
        float intensity_{};
        float falloff_{1.0F};
    };

    struct DefaultLightSlot
    {
        const Light* owner_{nullptr};
        bool         active_{false};
    };

    void init_default_shaders();
    void init_default_light_buffer();
    void release_default_light_buffer();
    void ensure_default_light_capacity(std::uint32_t light_count);
    void mark_default_light_data_dirty(std::uint32_t light_index);
    void bind_default_light_buffer() const;
    void init_default_shader_lighting_uniforms() const;
    void update_default_lighting_uniforms() const;
    void sync_default_lighting();

    std::uint32_t get_default_light_count() const;

    glm::mat4          model_view_projection_{1.0F};
    glm::mat4          view_{1.0F};
    glm::mat4          projection_{1.0F};
    Shader             shape_default_shader_{};
    Shader             texture_default_shader_{};
    Shader             text_default_shader_{};
    std::int32_t       width_{};
    std::int32_t       height_{};
    const Application* application_{nullptr};

    std::vector<DefaultLightData> default_light_data_{};
    std::vector<DefaultLightSlot> default_light_slots_{};
    std::uint32_t                 default_light_buffer_object_{};
    std::uint32_t                 default_light_texture_object_{};
    std::uint32_t                 default_light_capacity_{};
    std::uint32_t                 default_light_count_{};
    std::uint32_t                 default_light_dirty_begin_{};
    std::uint32_t                 default_light_dirty_end_{};
    float                         default_ambient_light_strength_{0.1F};
    bool                          default_light_data_dirty_{};
    bool                          default_light_uniforms_dirty_{true};
    bool                          default_ambient_light_enabled_{};
};

} // namespace rinvid

#endif // INCLUDE_RINVID_CORE_RENDER_CONTEXT_H
