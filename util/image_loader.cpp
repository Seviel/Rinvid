/**********************************************************************
 * Copyright (c) 2021 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <limits>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <stb_image.h>

#include <rinvid/util/error_handler.h>
#include <rinvid/util/image_loader.h>

namespace rinvid
{

ImageData load_image(const std::filesystem::path& file_name)
{
    if (file_name.empty() || file_name.native().find(std::filesystem::path::value_type{}) !=
                                 std::filesystem::path::string_type::npos)
    {
        throw ResourceLoadError{"Image: Empty path or embedded null character"};
    }

#ifdef _WIN32
    std::FILE* raw_file{_wfopen(file_name.c_str(), L"rb")};
#else
    std::FILE* raw_file{std::fopen(file_name.c_str(), "rb")};
#endif
    const auto close_file = [](std::FILE* file) { std::fclose(file); };
    const std::unique_ptr<std::FILE, decltype(close_file)> file{raw_file, close_file};

    if (!file)
    {
        throw ResourceLoadError{"Image: Could not open '" + file_name.u8string() + "'"};
    }

    ImageData                                                  image;
    std::int32_t                                               width{}, height{}, channels{};
    const std::unique_ptr<stbi_uc, decltype(&stbi_image_free)> data{
        stbi_load_from_file(file.get(), &width, &height, &channels, STBI_rgb_alpha),
        &stbi_image_free};

    if (!data)
    {
        const char* reason{stbi_failure_reason()};
        throw ResourceLoadError{"Image: Could not decode '" + file_name.u8string() +
                                "': " + (reason ? reason : "unknown image error")};
    }

    if (width <= 0 || height <= 0 ||
        static_cast<std::size_t>(width) >
            std::numeric_limits<std::size_t>::max() / 4U / static_cast<std::size_t>(height))
    {
        throw ResourceLoadError{"Image: Invalid dimensions in '" + file_name.u8string() + "'"};
    }

    const auto size{static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * 4U};
    image.pixels.assign(data.get(), data.get() + size);
    image.width  = width;
    image.height = height;

    return image;
}

bool load_image(const char* file_name, std::vector<std::uint8_t>& image_data, std::int32_t& width,
                std::int32_t& height)
{
    try
    {
        if (file_name == nullptr)
        {
            throw ResourceLoadError{"Image: Null file path"};
        }
        auto image = load_image(std::filesystem::path{file_name});
        image_data = std::move(image.pixels);
        width      = image.width;
        height     = image.height;
        return true;
    }
    catch (const ResourceLoadError& error)
    {
        errors::put_error_to_log(error.what());
        return false;
    }
}

} // namespace rinvid
