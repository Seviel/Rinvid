/**********************************************************************
 * Copyright (c) 2025 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <algorithm>
#include <utility>

#include <rinvid/gui/label.h>

namespace rinvid
{

namespace gui
{

Label::Label(std::string text, const std::string& font_path, Vector2f position, Color color,
             std::uint32_t size, float bounds_width, LabelAlignment alignment)
    : Text(std::move(text), font_path, position, color, size), bounds_width_{0.0F},
      alignment_{alignment}, visible_{true}
{
    set_bounds_width(bounds_width);
}

void Label::set_alignment(LabelAlignment alignment)
{
    alignment_ = alignment;
}

LabelAlignment Label::get_alignment() const
{
    return alignment_;
}

void Label::set_bounds_width(float bounds_width)
{
    bounds_width_ = std::max(0.0F, bounds_width);
    Text::set_max_width(bounds_width_);
}

float Label::get_bounds_width() const
{
    return bounds_width_;
}

void Label::set_visible(bool visible)
{
    visible_ = visible;
}

bool Label::is_visible() const
{
    return visible_;
}

void Label::draw()
{
    if (visible_)
    {
        Text::draw();
    }
}

void Label::draw(const Shader shader)
{
    if (!visible_)
    {
        return;
    }

    Vector2f original_position{Text::get_position()};
    Text::set_position(get_aligned_position());
    Text::draw(shader);
    Text::set_position(original_position);
}

Vector2f Label::get_aligned_position() const
{
    Vector2f position{Text::get_position()};
    if (bounds_width_ <= 0.0F || alignment_ == LabelAlignment::Left)
    {
        return position;
    }

    const float text_width{std::min(Text::get_width(), bounds_width_)};
    const float extra_width{std::max(0.0F, bounds_width_ - text_width)};

    if (alignment_ == LabelAlignment::Center)
    {
        position.x += extra_width / 2.0F;
    }
    else if (alignment_ == LabelAlignment::Right)
    {
        position.x += extra_width;
    }

    return position;
}

} // namespace gui

} // namespace rinvid
