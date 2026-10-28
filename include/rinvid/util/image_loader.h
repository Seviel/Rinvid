/**********************************************************************
 * Copyright (c) 2021 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_UTIL_IMAGE_LOADER_H
#define INCLUDE_RINVID_UTIL_IMAGE_LOADER_H

#include <cstdint>
#include <filesystem>
#include <vector>

#include <rinvid/util/error.h>

namespace rinvid
{

/**************************************************************************************************
 * @brief Owned CPU image data in RGBA8 format, with rows in file order.
 *
 *************************************************************************************************/
struct ImageData
{
    std::vector<std::uint8_t> pixels;   ///< Four bytes per pixel: red, green, blue, alpha.
    std::int32_t              width{};  ///< Width in pixels.
    std::int32_t              height{}; ///< Height in pixels.
};

/**************************************************************************************************
 * @brief Decodes an image without requiring a graphics context.
 *
 * @param file_name Image path.
 *
 * @return Owned RGBA8 pixels and positive dimensions.
 *
 * @throws ResourceLoadError if the image cannot be opened or decoded.
 *
 *************************************************************************************************/
ImageData load_image(const std::filesystem::path& file_name);

/**************************************************************************************************
 * @brief Loads image from filesystem.
 *
 * @param file_name path to image file to load
 * @param image_data vector of bytes to store image data
 * @param width width of the image
 * @param height height of the image
 *
 * @return true if successfully loaded the image, false otherwise
 *
 *************************************************************************************************/
bool load_image(const char* file_name, std::vector<std::uint8_t>& image_data, std::int32_t& width,
                std::int32_t& height);

} // namespace rinvid

#endif // INCLUDE_RINVID_UTIL_IMAGE_LOADER_H
