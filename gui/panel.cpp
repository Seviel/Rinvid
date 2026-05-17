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

#include <rinvid/gui/panel.h>

namespace rinvid
{

namespace gui
{

namespace
{

std::int32_t to_rect_size(float value)
{
    return static_cast<std::int32_t>(std::ceil(std::max(0.0F, value)));
}

Vector2f get_center(Vector2f position, float width, float height)
{
    return Vector2f{position.x + (width / 2.0F), position.y + (height / 2.0F)};
}

} // namespace

Panel::Panel(Vector2f position, float width, float height)
    : children_{}, position_{position}, width_{std::max(0.0F, width)},
      height_{std::max(0.0F, height)}, border_thickness_{0.0F}, padding_{0.0F},
      background_color_{0x00000080}, border_color_{0xFFFFFFFF}, background_{nullptr},
      border_{nullptr}, visible_{true}, enabled_{true}
{
    update_shapes();
}

void Panel::add(Widget* widget, Vector2f local_position)
{
    if (widget == nullptr)
    {
        return;
    }

    children_.push_back(Child{widget, local_position});
    layout_child(children_.back());
}

void Panel::remove(Widget* widget)
{
    children_.erase(std::remove_if(children_.begin(), children_.end(),
                                   [widget](const Child& child) { return child.widget == widget; }),
                    children_.end());
}

void Panel::clear()
{
    children_.clear();
}

void Panel::set_size(float width, float height)
{
    width_  = std::max(0.0F, width);
    height_ = std::max(0.0F, height);
    update_shapes();
}

float Panel::get_width() const
{
    return width_;
}

float Panel::get_height() const
{
    return height_;
}

void Panel::set_background_color(Color color)
{
    background_color_ = color;
    update_shapes();
}

void Panel::set_border_color(Color color)
{
    border_color_ = color;
    update_shapes();
}

void Panel::set_border_thickness(float thickness)
{
    border_thickness_ = std::max(0.0F, thickness);
    update_shapes();
}

float Panel::get_border_thickness() const
{
    return border_thickness_;
}

void Panel::set_padding(float padding)
{
    padding_ = std::max(0.0F, padding);
    layout_children();
}

float Panel::get_padding() const
{
    return padding_;
}

std::size_t Panel::get_child_count() const
{
    return children_.size();
}

void Panel::update(double delta_time)
{
    if (!visible_ || !enabled_)
    {
        return;
    }

    for (Child& child : children_)
    {
        if (child.widget != nullptr)
        {
            child.widget->update(delta_time);
        }
    }
}

void Panel::draw()
{
    if (!visible_)
    {
        return;
    }

    if (border_ != nullptr)
    {
        border_->draw();
    }

    if (background_ != nullptr)
    {
        background_->draw();
    }

    for (Child& child : children_)
    {
        if (child.widget != nullptr)
        {
            child.widget->draw();
        }
    }
}

void Panel::draw(double delta_time)
{
    if (!visible_)
    {
        return;
    }

    if (border_ != nullptr)
    {
        border_->draw();
    }

    if (background_ != nullptr)
    {
        background_->draw();
    }

    for (Child& child : children_)
    {
        if (child.widget != nullptr)
        {
            child.widget->draw(delta_time);
        }
    }
}

void Panel::move(const Vector2f move_vector)
{
    position_.move(move_vector);
    update_shapes();

    for (Child& child : children_)
    {
        if (child.widget != nullptr)
        {
            child.widget->move(move_vector);
        }
    }
}

void Panel::set_position(const Vector2f position)
{
    position_ = position;
    update_shapes();
    layout_children();
}

Rect Panel::get_bounds()
{
    return Rect{position_, to_rect_size(width_), to_rect_size(height_)};
}

void Panel::set_visible(bool visible)
{
    visible_ = visible;
}

bool Panel::is_visible() const
{
    return visible_;
}

void Panel::set_enabled(bool enabled)
{
    enabled_ = enabled;
}

bool Panel::is_enabled() const
{
    return enabled_;
}

void Panel::update_shapes()
{
    const float clamped_border_thickness{
        std::min(border_thickness_, std::min(width_, height_) / 2.0F)};
    if (width_ <= 0.0F || height_ <= 0.0F)
    {
        border_.reset();
        background_.reset();
        return;
    }

    if (clamped_border_thickness > 0.0F)
    {
        border_ = std::make_unique<RectangleShape>(get_center(position_, width_, height_), width_,
                                                   height_);
        border_->set_color(border_color_);
    }
    else
    {
        border_.reset();
    }

    const float background_width{width_ - (2.0F * clamped_border_thickness)};
    const float background_height{height_ - (2.0F * clamped_border_thickness)};
    if (background_width <= 0.0F || background_height <= 0.0F)
    {
        background_.reset();
        return;
    }

    Vector2f background_position{position_.x + clamped_border_thickness,
                                 position_.y + clamped_border_thickness};
    background_ = std::make_unique<RectangleShape>(
        get_center(background_position, background_width, background_height), background_width,
        background_height);
    background_->set_color(background_color_);
}

void Panel::layout_child(Child& child)
{
    if (child.widget == nullptr)
    {
        return;
    }

    child.widget->set_position({position_.x + padding_ + child.local_position.x,
                                position_.y + padding_ + child.local_position.y});
}

void Panel::layout_children()
{
    for (Child& child : children_)
    {
        layout_child(child);
    }
}

} // namespace gui

} // namespace rinvid
