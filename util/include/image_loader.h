/**********************************************************************
 * Copyright (c) 2021, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef UTIL_IMAGE_LOADER_H
#define UTIL_IMAGE_LOADER_H

#include <cstdint>
#include <vector>

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

#endif // UTIL_IMAGE_LOADER_H
