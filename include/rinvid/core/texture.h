/**********************************************************************
 * Copyright (c) 2021 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_CORE_TEXTURE_H
#define INCLUDE_RINVID_CORE_TEXTURE_H

#include <cstdint>
#include <vector>

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
     * @brief Texture constructor from raw RGBA pixels.
     *
     * @param image_data Image data in RGBA8 format.
     * @param width Texture width in pixels.
     * @param height Texture height in pixels.
     *
     *************************************************************************************************/
    Texture(const std::vector<std::uint8_t>& image_data, std::int32_t width, std::int32_t height);

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

    void release_gl_resources();
    void bind() const;
    void init_gl_texture(const std::uint8_t* image_data);

    std::int32_t  width_{};
    std::int32_t  height_{};
    std::uint32_t texture_id_{};
};

} // namespace rinvid

#endif // INCLUDE_RINVID_CORE_TEXTURE_H
