/**********************************************************************
 * Copyright (c) 2023 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <cctype>
#include <cmath>
#include <iostream>
#include <utility>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <rinvid/core/render_context.h>
#include <rinvid/core/rinvid_gl.h>
#include <rinvid/core/text.h>
#include <rinvid/core/ttf_lib.h>
#include <rinvid/util/error.h>

namespace rinvid
{

namespace
{

constexpr float LINE_SPACING = 1.08F;

bool is_line_break(char character)
{
    return character == '\n' || character == '\r';
}

bool is_wrapping_whitespace(char character)
{
    return !is_line_break(character) && std::isspace(static_cast<unsigned char>(character)) != 0;
}

std::string::const_iterator consume_line_break(std::string::const_iterator current,
                                               std::string::const_iterator end)
{
    if (*current == '\r')
    {
        ++current;
        if (current != end && *current == '\n')
        {
            ++current;
        }

        return current;
    }

    return ++current;
}

std::string::const_iterator find_whitespace_end(std::string::const_iterator current,
                                                std::string::const_iterator end)
{
    while (current != end && is_wrapping_whitespace(*current))
    {
        ++current;
    }

    return current;
}

std::string::const_iterator find_word_end(std::string::const_iterator current,
                                          std::string::const_iterator end)
{
    while (current != end && !is_line_break(*current) && !is_wrapping_whitespace(*current))
    {
        ++current;
    }

    return current;
}

} // namespace

void Text::release_character_textures()
{
    for (auto& [character_key, character] : characters_)
    {
        (void)character_key;
        if (character.texture_id != 0U)
        {
            GL_CALL(glDeleteTextures(1, &character.texture_id));
            character.texture_id = 0U;
        }
    }

    characters_.clear();
}

void Text::release_vertex_buffer()
{
    if (vertex_buffer_object_ != 0U)
    {
        GL_CALL(glDeleteBuffers(1, &vertex_buffer_object_));
        vertex_buffer_object_ = 0U;
    }

    if (vertex_array_object_ != 0U)
    {
        GL_CALL(glDeleteVertexArrays(1, &vertex_array_object_));
        vertex_array_object_ = 0U;
    }
}

Text::Text(std::string text, const std::string& font_path, Vector2f position, Color color,
           std::uint32_t size)
    : size_{size}, text_{std::move(text)}, position_{position}, color_{color}, max_width_{0.0F}
{
    const auto* ft_lib = TTFLib::get_instance();

    auto error = FT_New_Face(*ft_lib, font_path.c_str(), 0, &ft_face_);
    if (error)
    {
        TTFLib::release();
        throw ResourceLoadError{"Freetype: Could not load font face '" + font_path +
                                "' with error code " + std::to_string(error)};
    }

    try
    {
        generate_character_textures();
    }
    catch (...)
    {
        FT_Done_Face(ft_face_);
        ft_face_ = nullptr;
        TTFLib::release();
        throw;
    }
}

Text::~Text()
{
    release_character_textures();
    release_vertex_buffer();

    if (ft_face_ != nullptr)
    {
        FT_Done_Face(ft_face_);
    }

    TTFLib::release();
}

void Text::draw()
{
    RenderContext* render_context = RenderContext::get_active_context();
    if (render_context == nullptr)
    {
        return;
    }

    const auto shader = render_context->get_text_default_shader();
    draw(shader);
}

void Text::draw(const Shader shader)
{
    RenderContext* render_context = RenderContext::get_active_context();
    if (render_context == nullptr)
    {
        return;
    }

    float x = position_.x;
    float y = position_.y;

    glm::vec4   glm_pos{x, y, 1.0F, 1.0F};
    const auto& view = render_context->get_view();
    glm_pos          = view * glm_pos;
    x                = glm_pos.x;
    y                = render_context->get_height() - glm_pos.y;

    shader.use();
    glm::mat4 projection = glm::ortho(0.0F, static_cast<float>(render_context->get_width()), 0.0F,
                                      static_cast<float>(render_context->get_height()));
    GL_CALL(glUniformMatrix4fv(glGetUniformLocation(shader.get_id(), "projection"), 1, GL_FALSE,
                               glm::value_ptr(projection)));
    GL_CALL(glUniform3f(glGetUniformLocation(shader.get_id(), "text_color"), color_.r, color_.g,
                        color_.b));
    GL_CALL(glActiveTexture(GL_TEXTURE0));
    GL_CALL(glBindVertexArray(vertex_array_object_));

    const float start_x = x;
    const float max_x   = start_x + max_width_;

    auto current                  = text_.cbegin();
    auto pending_whitespace_begin = text_.cend();
    auto pending_whitespace_end   = text_.cend();
    bool has_pending_whitespace{false};
    bool line_has_content{false};
    bool skip_leading_whitespace{false};

    while (current != text_.cend())
    {
        if (is_line_break(*current))
        {
            has_pending_whitespace = false;
            current                = consume_line_break(current, text_.cend());
            move_to_next_line(x, y, start_x);
            line_has_content        = false;
            skip_leading_whitespace = false;
            continue;
        }

        if (is_wrapping_whitespace(*current))
        {
            pending_whitespace_begin = current;
            pending_whitespace_end   = find_whitespace_end(current, text_.cend());
            has_pending_whitespace   = true;
            current                  = pending_whitespace_end;
            continue;
        }

        const auto word_begin = current;
        const auto word_end   = find_word_end(current, text_.cend());
        current               = word_end;

        const bool should_draw_pending_whitespace =
            has_pending_whitespace && (line_has_content || !skip_leading_whitespace);
        const float pending_whitespace_width =
            should_draw_pending_whitespace
                ? get_text_width(pending_whitespace_begin, pending_whitespace_end)
                : 0.0F;
        const float word_width = get_text_width(word_begin, word_end);

        if (max_width_ > 0.0F && line_has_content &&
            x + pending_whitespace_width + word_width > max_x)
        {
            move_to_next_line(x, y, start_x);
            line_has_content        = false;
            skip_leading_whitespace = true;
        }
        else if (should_draw_pending_whitespace)
        {
            draw_text_range(pending_whitespace_begin, pending_whitespace_end, x, y);
            line_has_content = true;
        }

        has_pending_whitespace = false;

        if (max_width_ > 0.0F && word_width > max_width_)
        {
            draw_text_range_wrapped_by_character(word_begin, word_end, x, y, start_x,
                                                 line_has_content);
        }
        else
        {
            draw_text_range(word_begin, word_end, x, y);
            line_has_content = true;
        }

        skip_leading_whitespace = false;
    }

    GL_CALL(glBindVertexArray(0));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}

void Text::move(const Vector2f move_vector)
{
    position_.move(move_vector);
}

void Text::set_position(const Vector2f position)
{
    position_ = position;
}

void Text::set_size(const std::uint32_t new_size)
{
    size_ = new_size;
    generate_character_textures();
}

void Text::set_color(const Color color)
{
    color_ = color;
}

void Text::set_text(const std::string& text)
{
    text_ = text;
}

void Text::set_max_width(float max_width)
{
    max_width_ = max_width;
}

const Text::Character* Text::find_character(char character_key) const
{
    const auto character = characters_.find(character_key);
    if (character == characters_.end())
    {
        return nullptr;
    }

    return &character->second;
}

float Text::get_character_advance(char character_key) const
{
    const auto* character = find_character(character_key);
    if (character == nullptr)
    {
        return 0.0F;
    }

    return static_cast<float>(character->advance >> 6);
}

float Text::get_text_width(std::string::const_iterator begin, std::string::const_iterator end) const
{
    float width{0.0F};

    for (auto current = begin; current != end; ++current)
    {
        width += get_character_advance(*current);
    }

    return width;
}

float Text::get_line_height() const
{
    return std::ceil(static_cast<float>(size_) * LINE_SPACING);
}

void Text::move_to_next_line(float& x, float& y, float start_x) const
{
    x = start_x;
    y -= get_line_height();
}

void Text::draw_character(char character_key, float& x, float y) const
{
    const auto* character = find_character(character_key);
    if (character == nullptr)
    {
        return;
    }

    const float xpos = x + static_cast<float>(character->bearing.x);
    const float ypos = y - static_cast<float>(character->size.y - character->bearing.y);

    const float width  = static_cast<float>(character->size.x);
    const float height = static_cast<float>(character->size.y);

    const float vertices[6][4] = {
        {xpos, ypos + height, 0.0F, 0.0F},        {xpos, ypos, 0.0F, 1.0F},
        {xpos + width, ypos, 1.0F, 1.0F},

        {xpos, ypos + height, 0.0F, 0.0F},        {xpos + width, ypos, 1.0F, 1.0F},
        {xpos + width, ypos + height, 1.0F, 0.0F}};

    GL_CALL(glBindTexture(GL_TEXTURE_2D, character->texture_id));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_));
    GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));

    x += get_character_advance(character_key);
}

void Text::draw_text_range(std::string::const_iterator begin, std::string::const_iterator end,
                           float& x, float y) const
{
    for (auto current = begin; current != end; ++current)
    {
        draw_character(*current, x, y);
    }
}

void Text::draw_text_range_wrapped_by_character(std::string::const_iterator begin,
                                                std::string::const_iterator end, float& x, float& y,
                                                float start_x, bool& line_has_content) const
{
    const float max_x = start_x + max_width_;

    for (auto current = begin; current != end; ++current)
    {
        const float advance = get_character_advance(*current);
        if (max_width_ > 0.0F && line_has_content && x + advance > max_x)
        {
            move_to_next_line(x, y, start_x);
            line_has_content = false;
        }

        draw_character(*current, x, y);
        line_has_content = true;
    }
}

void Text::generate_character_textures()
{
    std::unordered_map<char, Character> new_characters{};
    new_characters.reserve(128U);

    FT_Set_Pixel_Sizes(ft_face_, 0, size_);

    GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

    try
    {
        for (unsigned char c = 0; c < 128; c++)
        {
            if (FT_Load_Char(ft_face_, c, FT_LOAD_RENDER))
            {
                throw ResourceLoadError{"Freetype: Failed to load glyph"};
            }

            std::uint32_t texture{};
            GL_CALL(glGenTextures(1, &texture));
            GL_CALL(glBindTexture(GL_TEXTURE_2D, texture));
            GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ft_face_->glyph->bitmap.width,
                                 ft_face_->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                                 ft_face_->glyph->bitmap.buffer));
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

            Character character = {
                texture, glm::ivec2(ft_face_->glyph->bitmap.width, ft_face_->glyph->bitmap.rows),
                glm::ivec2(ft_face_->glyph->bitmap_left, ft_face_->glyph->bitmap_top),
                static_cast<unsigned int>(ft_face_->glyph->advance.x)};
            new_characters.emplace(static_cast<char>(c), character);
        }
    }
    catch (...)
    {
        for (auto& [character_key, character] : new_characters)
        {
            (void)character_key;
            if (character.texture_id != 0U)
            {
                GL_CALL(glDeleteTextures(1, &character.texture_id));
            }
        }

        throw;
    }

    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

    release_character_textures();
    release_vertex_buffer();

    characters_ = std::move(new_characters);

    GL_CALL(glGenVertexArrays(1, &vertex_array_object_));
    GL_CALL(glGenBuffers(1, &vertex_buffer_object_));
    GL_CALL(glBindVertexArray(vertex_array_object_));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW));
    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_CALL(glBindVertexArray(0));
}

} // namespace rinvid
