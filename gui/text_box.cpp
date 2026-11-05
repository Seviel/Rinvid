/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <algorithm>
#include <cmath>
#include <utility>

#include <SFML/Window.hpp>
#include <glm/glm.hpp>

#include <rinvid/core/render_context.h>
#include <rinvid/gui/text_box.h>
#include <rinvid/system/application_context.h>

namespace rinvid
{

namespace gui
{

TextBox::TextBox(std::string text, const std::string& font_path, Vector2f position, float width,
                 float height, Color text_color, std::uint32_t text_size)
    : background_{position, width, height},
      text_graphic_{"", font_path, position, text_color, text_size}, position_{position}
{
    background_.set_background_color(0x151B24FF);
    background_.set_border_color(border_color_);
    background_.set_border_thickness(2.0F);
    set_text(text);
    was_text_changed_ = false;
}

void TextBox::set_text(const std::string& text)
{
    std::string filtered;

    for (const unsigned char character : text)
    {
        if (character >= 32U && character <= 126U)
        {
            filtered.push_back(static_cast<char>(character));
        }
    }

    was_text_changed_ = was_text_changed_ || filtered != text_;
    text_             = std::move(filtered);
    caret_position_   = text_.size();
    blink_time_       = 0.0;
    update_layout();
}

const std::string& TextBox::get_text() const
{
    return text_;
}

void TextBox::set_text_color(Color color)
{
    text_graphic_.set_color(color);

    if (caret_ != nullptr)
    {
        caret_->set_color(color);
    }
}

void TextBox::set_text_size(std::uint32_t size)
{
    text_graphic_.set_size(size);
    update_layout();
}

void TextBox::set_size(float width, float height)
{
    background_.set_size(width, height);
    update_layout();
}

void TextBox::set_background_color(Color color)
{
    background_.set_background_color(color);
}

void TextBox::set_border_color(Color color)
{
    border_color_ = color;
    background_.set_border_color(focused_ ? focused_border_color_ : border_color_);
}

void TextBox::set_focused_border_color(Color color)
{
    focused_border_color_ = color;
    background_.set_border_color(focused_ ? focused_border_color_ : border_color_);
}

void TextBox::set_border_thickness(float thickness)
{
    background_.set_border_thickness(thickness);
    update_layout();
}

void TextBox::set_padding(float padding)
{
    padding_ = std::max(0.0F, padding);
    update_layout();
}

void TextBox::set_focused(bool focused)
{
    focused = focused && is_visible() && is_enabled();

    if (focused_ == focused)
    {
        return;
    }

    focused_    = focused;
    blink_time_ = 0.0;
    background_.set_border_color(focused_ ? focused_border_color_ : border_color_);
}

bool TextBox::is_focused() const
{
    return focused_;
}

bool TextBox::was_text_changed() const
{
    return was_text_changed_;
}

bool TextBox::was_submitted() const
{
    return was_submitted_;
}

void TextBox::update(double delta_time)
{
    was_text_changed_ = false;
    was_submitted_    = false;
    const auto* context{ApplicationContext::get_active_context()};

    if (!is_visible() || !is_enabled() || context == nullptr || context->get_window() == nullptr ||
        !context->get_window()->hasFocus())
    {
        set_focused(false);
        return;
    }

    blink_time_ = std::fmod(blink_time_ + std::max(0.0, delta_time), 1.0);
    for (const auto& event : context->get_input_state().get_events())
    {
        if (const auto* pressed{event.getIf<sf::Event::MouseButtonPressed>()})
        {
            if (pressed->button != sf::Mouse::Button::Left)
            {
                continue;
            }
            const auto& render_context{context->get_render_context()};
            const auto  canvas_position{render_context.map_pixel_to_canvas(
                {static_cast<float>(pressed->position.x), static_cast<float>(pressed->position.y)},
                static_cast<std::int32_t>(context->get_window()->getSize().y))};
            if (!canvas_position)
            {
                set_focused(false);
                continue;
            }
            const glm::vec4 mouse_position{
                glm::inverse(render_context.get_view()) *
                glm::vec4{canvas_position->x, canvas_position->y, 1.0F, 1.0F}};
            set_focused(mouse_position.x >= position_.x && mouse_position.y >= position_.y &&
                        mouse_position.x < position_.x + background_.get_width() &&
                        mouse_position.y < position_.y + background_.get_height());
            if (focused_)
            {
                place_caret(mouse_position.x);
            }
        }
        else if (focused_)
        {
            if (const auto* entered{event.getIf<sf::Event::TextEntered>()})
            {
                if (entered->unicode >= 32U && entered->unicode <= 126U)
                {
                    text_.insert(caret_position_, 1U, static_cast<char>(entered->unicode));
                    ++caret_position_;
                    was_text_changed_ = true;
                    blink_time_       = 0.0;
                    update_layout();
                }
            }
            else if (const auto* pressed{event.getIf<sf::Event::KeyPressed>()})
            {
                handle_key(pressed->code);
            }
        }
    }
}

void TextBox::draw()
{
    auto* context{RenderContext::get_active_context()};

    if (!is_visible() || context == nullptr)
    {
        return;
    }

    background_.draw();
    const float inset{get_inset()};
    const float width{background_.get_width() - 2.0F * inset};
    const float height{background_.get_height() - 2.0F * inset};

    if (width <= 0.0F || height <= 0.0F)
    {
        return;
    }

    context->push_clip_rectangle({position_.x + inset, position_.y + inset}, width, height);
    text_graphic_.draw();

    if (focused_ && blink_time_ < 0.5 && caret_ != nullptr)
    {
        caret_->draw();
    }
    context->pop_clip_rectangle();
}

void TextBox::draw(double delta_time)
{
    (void)delta_time;
    draw();
}

void TextBox::move(const Vector2f move_vector)
{
    set_position({position_.x + move_vector.x, position_.y + move_vector.y});
}

void TextBox::set_position(const Vector2f position)
{
    position_ = position;
    background_.set_position(position);
    update_layout();
}

Rect TextBox::get_bounds()
{
    return background_.get_bounds();
}

void TextBox::set_visible(bool visible)
{
    background_.set_visible(visible);

    if (!visible)
    {
        set_focused(false);
    }
}

bool TextBox::is_visible() const
{
    return background_.is_visible();
}

void TextBox::set_enabled(bool enabled)
{
    background_.set_enabled(enabled);

    if (!enabled)
    {
        set_focused(false);
    }
}

bool TextBox::is_enabled() const
{
    return background_.is_enabled();
}

float TextBox::get_inset() const
{
    return padding_ + std::min(background_.get_border_thickness(),
                               std::min(background_.get_width(), background_.get_height()) / 2.0F);
}

float TextBox::measure_prefix(std::size_t length)
{
    text_graphic_.set_text(text_.substr(0U, length));
    return text_graphic_.get_width();
}

void TextBox::update_layout()
{
    const float inset{get_inset()};
    const float available_width{std::max(0.0F, background_.get_width() - 2.0F * inset - 1.0F)};
    const float caret_offset{measure_prefix(caret_position_)};
    text_graphic_.set_text(text_);
    scroll_offset_ = std::clamp(scroll_offset_, 0.0F,
                                std::max(0.0F, text_graphic_.get_width() - available_width));

    if (caret_offset < scroll_offset_)
    {
        scroll_offset_ = caret_offset;
    }
    else if (caret_offset > scroll_offset_ + available_width)
    {
        scroll_offset_ = caret_offset - available_width;
    }

    const float font_size{static_cast<float>(text_graphic_.get_size())};
    const float baseline{position_.y + (background_.get_height() + font_size) / 2.0F};
    text_graphic_.set_position({position_.x + inset - scroll_offset_, baseline});
    const float caret_height{
        std::min(font_size, std::max(0.0F, background_.get_height() - 2.0F * inset))};
    caret_.reset();

    if (caret_height > 0.0F && available_width > 0.0F)
    {
        caret_ = std::make_unique<RectangleShape>(
            Vector2f{position_.x + inset + caret_offset - scroll_offset_ + 0.5F,
                     position_.y + background_.get_height() / 2.0F},
            1.0F, caret_height);
        caret_->set_color(text_graphic_.get_color());
    }
}

void TextBox::place_caret(float position_x)
{
    const float offset{position_x - position_.x - get_inset() + scroll_offset_};
    float       previous_width{0.0F};
    caret_position_ = text_.size();

    for (std::size_t index{0U}; index < text_.size(); ++index)
    {
        text_graphic_.set_text(text_.substr(index, 1U));
        const float width{previous_width + text_graphic_.get_width()};
        if (offset < (previous_width + width) / 2.0F)
        {
            caret_position_ = index;
            break;
        }
        previous_width = width;
    }

    blink_time_ = 0.0;
    update_layout();
}

void TextBox::handle_key(sf::Keyboard::Key key)
{
    switch (key)
    {
        case sf::Keyboard::Key::Left:
            if (caret_position_ > 0U)
            {
                --caret_position_;
            }
            break;
        case sf::Keyboard::Key::Right:
            caret_position_ = std::min(text_.size(), caret_position_ + 1U);
            break;
        case sf::Keyboard::Key::Home:
            caret_position_ = 0U;
            break;
        case sf::Keyboard::Key::End:
            caret_position_ = text_.size();
            break;
        case sf::Keyboard::Key::Backspace:
            if (caret_position_ > 0U)
            {
                text_.erase(--caret_position_, 1U);
                was_text_changed_ = true;
            }
            break;
        case sf::Keyboard::Key::Delete:
            if (caret_position_ < text_.size())
            {
                text_.erase(caret_position_, 1U);
                was_text_changed_ = true;
            }
            break;
        case sf::Keyboard::Key::Enter:
            was_submitted_ = true;
            break;
        case sf::Keyboard::Key::Escape:
            set_focused(false);
            break;
        default:
            return;
    }

    blink_time_ = 0.0;
    update_layout();
}

} // namespace gui

} // namespace rinvid
