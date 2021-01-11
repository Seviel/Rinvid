/**********************************************************************
 * Copyright (c) 2021, Filip Vasiljevic
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
    Texture& operator=(Texture other) = delete;

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
     *************************************************************************************************/
    void draw();

    /**************************************************************************************************
     * @brief Internal function called by sprite. Adjusts internal texture vertices based on
     * arguments passed by sprite.
     *
     *************************************************************************************************/
    void update_vertices(Vector2 top_left, std::uint32_t width, std::uint32_t height);

    std::int32_t width_{};
    std::int32_t height_{};

    // OpenGl object id's
    std::uint32_t vao_{};
    std::uint32_t vbo_{};
    std::uint32_t ebo_{};
    std::uint32_t texture_id_{};

    // There are four vertices with 5 elements each, elements are: x, y, z coordinate and x and y
    // texture cooridnate, hence 4 * 5
    float               vertices_[4 * 5]{};
    const std::uint32_t indices_[6] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    std::vector<std::uint8_t> image_data_{};
};

} // namespace rinvid

#endif // CORE_INCLUDE_TEXTURE_H
