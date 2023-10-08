/**********************************************************************
 * Copyright (c) 2023, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <unordered_map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "core/include/drawable.h"
#include "util/include/color.h"
#include "util/include/vector2.h"

namespace rinvid
{

class Text : public Drawable
{
  public:
    /**************************************************************************************************
     * @brief Constructor.
     *
     * @param text The contents.
     * @param font_path Path to font on the filesystem.
     * @param position Position where to draw the text.
     * @param color Color of the text.
     * @param size Font size.
     *
     *************************************************************************************************/
    Text(std::string text, std::string font_path, Vector2<float> position, Color color,
         std::uint32_t size);

    /**************************************************************************************************
     * @brief Draws the text.
     *
     *************************************************************************************************/
    virtual void draw() override;

  private:
    struct Character
    {
        std::uint32_t texture_id;
        glm::ivec2    size;
        glm::ivec2    bearing;
        std::uint32_t advance;
    };

    FT_Library     ft_lib_;
    FT_Face        ft_face_;
    std::uint32_t  vertex_array_object_;
    std::uint32_t  vertex_buffer_object_;
    std::string    text_;
    Vector2<float> position_;
    Color          color_;

    std::unordered_map<char, Character> characters_;
};

} // namespace rinvid
