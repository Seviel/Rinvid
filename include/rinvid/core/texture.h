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

#include <rinvid/util/error.h>

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
     * @throws ResourceLoadError if the texture image cannot be loaded.
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

    /**************************************************************************************************
     * @brief Selects linear filtering or nearest-neighbor filtering for this texture.
     * Applies to every sprite or tile map sharing this texture. Requires an active OpenGL context.
     *
     * @param smooth True for linear filtering, false for nearest-neighbor filtering.
     *
     *************************************************************************************************/
    void set_smooth(bool smooth);

    /**************************************************************************************************
     * @brief Sets the process-wide filtering default for newly constructed textures.
     * Initially true (linear). Existing textures, per-texture overrides, and TrueType fonts
     * are unaffected. No OpenGL context is required. Set before loading textures; do not call
     * concurrently with texture construction or another call to this function.
     *
     * @param smooth True for linear filtering, false for nearest-neighbor filtering.
     *
     *************************************************************************************************/
    static void set_default_smooth(bool smooth);

  private:
    friend class Sprite;
    friend class TileMap;

    void release_gl_resources();
    void bind() const;
    void init_gl_texture(const std::uint8_t* image_data);

    static bool default_smooth_;

    std::int32_t  width_{};
    std::int32_t  height_{};
    std::uint32_t texture_id_{};
};

} // namespace rinvid

#endif // INCLUDE_RINVID_CORE_TEXTURE_H
