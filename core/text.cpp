/**********************************************************************
 * Copyright (c) 2023 - 2024, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <iostream>
#include <map>

#include "extern/glm/glm/glm.hpp"
#include "extern/glm/glm/gtc/type_ptr.hpp"

#include "core/include/rinvid_gfx.h"
#include "core/include/rinvid_gl.h"
#include "core/include/text.h"
#include "core/include/ttf_lib.h"

namespace rinvid
{

Text::Text(std::string text, std::string font_path, Vector2f position, Color color,
           std::uint32_t size)
    : size_{size}, text_{text}, position_{position}, color_{color}
{
    const auto* ft_lib = TTFLib::get_instance();

    auto error = FT_New_Face(*ft_lib, font_path.c_str(), 0, &ft_face_);
    if (error)
    {
        throw error;
    }

    generate_character_textures();
}

Text::~Text()
{
    FT_Done_Face(ft_face_);
}

void Text::draw()
{
    const auto shader = RinvidGfx::get_text_default_shader();
    draw(shader);
}

void Text::draw(const Shader shader)
{
    float scale = 1.0F;
    float x     = position_.x;
    float y     = position_.y;

    glm::vec4   glm_pos{x, y, 1.0F, 1.0F};
    const auto& view = RinvidGfx::get_view();
    glm_pos          = view * glm_pos;
    x                = glm_pos.x;
    y                = RinvidGfx::get_height() - glm_pos.y;

    shader.use();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(RinvidGfx::get_width()), 0.0f,
                                      static_cast<float>(RinvidGfx::get_height()));
    GL_CALL(glUniformMatrix4fv(glGetUniformLocation(shader.get_id(), "projection"), 1, GL_FALSE,
                               glm::value_ptr(projection)));
    GL_CALL(glUniform3f(glGetUniformLocation(shader.get_id(), "text_color"), color_.r, color_.g,
                        color_.b));
    GL_CALL(glActiveTexture(GL_TEXTURE0));
    GL_CALL(glBindVertexArray(vertex_array_object_));

    std::string::const_iterator c;
    for (c = text_.begin(); c != text_.end(); c++)
    {
        Character character = characters_[*c];

        float xpos = x + character.bearing.x * scale;
        float ypos = y - (character.size.y - character.bearing.y) * scale;

        float width          = character.size.x * scale;
        float height         = character.size.y * scale;
        float vertices[6][4] = {
            {xpos, ypos + height, 0.0f, 0.0f},        {xpos, ypos, 0.0f, 1.0f},
            {xpos + width, ypos, 1.0f, 1.0f},

            {xpos, ypos + height, 0.0f, 0.0f},        {xpos + width, ypos, 1.0f, 1.0f},
            {xpos + width, ypos + height, 1.0f, 0.0f}};
        GL_CALL(glBindTexture(GL_TEXTURE_2D, character.texture_id));
        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_));
        GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));
        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));
        x += (character.advance >> 6) * scale;
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

void Text::generate_character_textures()
{
    characters_.clear();

    FT_Set_Pixel_Sizes(ft_face_, 0, size_);

    GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

    for (unsigned char c = 0; c < 128; c++)
    {
        if (FT_Load_Char(ft_face_, c, FT_LOAD_RENDER))
        {
            throw "Freetype: Failed to load Glyph";
        }

        std::uint32_t texture;
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
        characters_.insert(std::pair<char, Character>(c, character));
    }
    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

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
