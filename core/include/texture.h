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
    Texture(const char* file_name);

  private:
    friend class Sprite;

    void draw();

    void update_vertices(Vector2 top_left, std::uint32_t width, std::uint32_t height);

    std::int32_t        width_{};
    std::int32_t        height_{};
    std::uint32_t       vao_{};
    std::uint32_t       vbo_{};
    std::uint32_t       ebo_{};
    std::uint32_t       texture_id_{};
    float               vertices_[4 * 5]{};
    const std::uint32_t indices_[6] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    std::vector<std::uint8_t> image_data_{};
};

} // namespace rinvid

#endif // CORE_INCLUDE_TEXTURE_H
