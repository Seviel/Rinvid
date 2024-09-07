/**********************************************************************
 * Copyright (c) 2021 - 2024, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_TEXTURE_H
#define CORE_INCLUDE_TEXTURE_H

#include <cstdint>
#include <vector>

#include "core/include/rinvid_gfx.h"
#include "extern/glm/glm/mat4x4.hpp"
#include "util/include/color.h"
#include "util/include/vector2.h"

namespace rinvid
{

class Texture
{
  public:
    /**************************************************************************************************
     * @brief Texture constructor.
     *
     * @param file_name Path to texture image file
     *
     *************************************************************************************************/
    Texture(const char* file_name);

    /**************************************************************************************************
     * @brief Copy constructor deleted.
     *
     *************************************************************************************************/
    Texture(const Texture& other) = delete;

    /**************************************************************************************************
     * @brief Copy assignement operator deleted.
     *
     *************************************************************************************************/
    Texture& operator=(Texture& other) = delete;

    /**************************************************************************************************
     * @brief Move constructor.
     *
     * @param other object being moved
     *
     *************************************************************************************************/
    Texture(Texture&& other);

    /**************************************************************************************************
     * @brief Move assignement operator.
     *
     * @param other object being moved
     *
     *************************************************************************************************/
    Texture& operator=(Texture&& other);

    /**************************************************************************************************
     * @brief Destructor.
     *
     *************************************************************************************************/
    ~Texture();

  private:
    friend class Sprite;

    /**************************************************************************************************
     * @brief Internal function called by sprite. Draws part of texture specified by sprite calling
     * it.
     *
     * @param transform Transformation to apply (model matrix)
     * @param shader Shader to be used.
     * @param opacity Transparency level (0.0 being invisible, 1.0 being fully visible).
     *
     *************************************************************************************************/
    void draw(const glm::mat4& transform, const Shader shader, float opacity = 1.0F);

    /**************************************************************************************************
     * @brief Internal function called by sprite. Adjusts internal texture vertices based on
     * arguments passed by sprite.
     *
     *************************************************************************************************/
    void update_vertices(Vector2<float> offset, std::uint32_t width, std::uint32_t height);

    std::int32_t width_{};
    std::int32_t height_{};

    // OpenGl object id's
    std::uint32_t vertex_array_object_{};
    std::uint32_t vertex_buffer_obecjt_{};
    std::uint32_t element_buffer_object_{};
    std::uint32_t texture_id_{};

    // There are four vertices with 5 elements each, elements are: x, y, z coordinate and x and y
    // texture cooridnate, hence 4 * 5
    float               gl_vertices_[4 * 5]{};
    const std::uint32_t indices_[6] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    std::vector<std::uint8_t> image_data_{};
};

} // namespace rinvid

#endif // CORE_INCLUDE_TEXTURE_H
