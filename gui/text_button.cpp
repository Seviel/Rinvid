/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <algorithm>
#include <utility>

#include <rinvid/gui/text_button.h>

namespace rinvid
{

namespace gui
{

namespace
{

constexpr float DEFAULT_LABEL_PADDING{8.0F};

} // namespace

TextButton::TextButton(std::string text, const std::string& font_path, Color text_color,
                       std::uint32_t text_size)
    : Button{}, label_{std::move(text), font_path, {0.0F, 0.0F},          text_color,
                       text_size,       0.0F,      LabelAlignment::Center},
      label_padding_{DEFAULT_LABEL_PADDING}
{
}

void TextButton::setup(Texture* texture, std::int32_t width, std::int32_t height, Vector2f top_left,
                       Vector2f texture_offset)
{
    Button::setup(texture, width, height, top_left, texture_offset);
    update_label_layout();
}

void TextButton::setup_from_atlas(Texture* texture, std::int32_t width, std::int32_t height,
                                  Vector2f top_left, std::uint32_t state_count,
                                  Vector2f atlas_offset)
{
    Button::setup_from_atlas(texture, width, height, top_left, state_count, atlas_offset);
    update_label_layout();
}

void TextButton::move(const Vector2f move_vector)
{
    Button::move(move_vector);
    update_label_layout();
}

void TextButton::set_position(const Vector2f position)
{
    Button::set_position(position);
    update_label_layout();
}

void TextButton::set_visible(bool visible)
{
    Button::set_visible(visible);
}

void TextButton::set_text(const std::string& text)
{
    label_.set_text(text);
    update_label_layout();
}

void TextButton::set_text_color(Color color)
{
    label_.set_color(color);
}

void TextButton::set_text_size(std::uint32_t size)
{
    label_.set_size(size);
    update_label_layout();
}

void TextButton::set_label_padding(float padding)
{
    label_padding_ = std::max(0.0F, padding);
    update_label_layout();
}

Label& TextButton::get_label()
{
    return label_;
}

const Label& TextButton::get_label() const
{
    return label_;
}

void TextButton::draw()
{
    if (!is_visible())
    {
        return;
    }

    Button::draw();
    label_.draw();
}

void TextButton::draw(const Shader shader)
{
    if (!is_visible())
    {
        return;
    }

    Button::draw(shader);
    label_.draw();
}

void TextButton::draw(double delta_time)
{
    if (!is_visible())
    {
        return;
    }

    Button::draw(delta_time);
    label_.draw();
}

void TextButton::draw(double delta_time, const Shader shader)
{
    if (!is_visible())
    {
        return;
    }

    Button::draw(delta_time, shader);
    label_.draw();
}

void TextButton::update_label_layout()
{
    Rect        button_rect{bounding_rect()};
    const float button_width{static_cast<float>(button_rect.width)};
    const float button_height{static_cast<float>(button_rect.height)};
    const float text_width{std::max(0.0F, button_width - (2.0F * label_padding_))};

    label_.set_bounds_width(text_width);
    label_.set_alignment(LabelAlignment::Center);
    label_.set_position({button_rect.position.x + label_padding_,
                         button_rect.position.y +
                             ((button_height + static_cast<float>(label_.get_size())) / 2.0F)});
}

} // namespace gui

} // namespace rinvid
