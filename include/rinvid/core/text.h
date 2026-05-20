/**********************************************************************
 * Copyright (c) 2023 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_CORE_TEXT_H
#define INCLUDE_RINVID_CORE_TEXT_H

#include <string>
#include <unordered_map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>

#include <rinvid/core/drawable.h>
#include <rinvid/util/color.h>
#include <rinvid/util/error.h>
#include <rinvid/util/vector2.h>

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
     * @throws ResourceLoadError if the font face or glyph resources cannot be loaded.
     *
     *************************************************************************************************/
    Text(std::string text, const std::string& font_path, Vector2f position, Color color,
         std::uint32_t size);

    Text(const Text& other) = delete;

    Text& operator=(const Text& other) = delete;

    Text(Text&& other) = delete;

    Text& operator=(Text&& other) = delete;

    /**************************************************************************************************
     * @brief Destructor. Releases font related resources.
     *
     *************************************************************************************************/
    ~Text();

    /**************************************************************************************************
     * @brief Draws the text.
     *
     *************************************************************************************************/
    virtual void draw() override;

    /**************************************************************************************************
     * @brief Draws the text with Shader.
     *
     * @param shader Shader to be used.
     *
     *************************************************************************************************/
    virtual void draw(const Shader shader) override;

    /**************************************************************************************************
     * @brief Moves the text.
     *
     * @param move_vector Vector to be added to position vector.
     *
     *************************************************************************************************/
    void move(const Vector2f move_vector);

    /**************************************************************************************************
     * @brief Sets position of the text.
     *
     * @param position Vector New position.
     *
     *************************************************************************************************/
    void set_position(const Vector2f position);

    /**************************************************************************************************
     * @brief Sets the font size.
     *
     * @param new_size New size.
     *
     *************************************************************************************************/
    void set_size(const std::uint32_t new_size);

    /**************************************************************************************************
     * @brief Sets the text color.
     *
     * @param color New color.
     *
     *************************************************************************************************/
    void set_color(const Color color);

    /**************************************************************************************************
     * @brief Sets the text color.
     *
     * @param color New color.
     *
     *************************************************************************************************/
    void set_text(const std::string& text);

    /**************************************************************************************************
     * @brief Sets max width of text.
     *
     * @param max_width
     *
     *************************************************************************************************/
    void set_max_width(float max_width);

  private:
    struct Character
    {
        std::uint32_t texture_id;
        glm::ivec2    size;
        glm::ivec2    bearing;
        std::uint32_t advance;
    };

    /**************************************************************************************************
     * @brief Generates and prepares textures of characters for rendering.
     *
     *************************************************************************************************/
    void generate_character_textures();

    void release_character_textures();

    void release_vertex_buffer();

    const Character* find_character(char character_key) const;

    float get_character_advance(char character_key) const;

    float get_text_width(std::string::const_iterator begin, std::string::const_iterator end) const;

    float get_line_height() const;

    void move_to_next_line(float& x, float& y, float start_x) const;

    void draw_character(char character_key, float& x, float y) const;

    void draw_text_range(std::string::const_iterator begin, std::string::const_iterator end,
                         float& x, float y) const;

    void draw_text_range_wrapped_by_character(std::string::const_iterator begin,
                                              std::string::const_iterator end, float& x, float& y,
                                              float start_x, bool& line_has_content) const;

    FT_Face       ft_face_{nullptr};
    std::uint32_t vertex_array_object_{};
    std::uint32_t vertex_buffer_object_{};
    std::uint32_t size_{};
    std::string   text_;
    Vector2f      position_;
    Color         color_;
    float         max_width_;

    std::unordered_map<char, Character> characters_;
};

} // namespace rinvid

#endif // INCLUDE_RINVID_CORE_TEXT_H
