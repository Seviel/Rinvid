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

#include <rinvid/gui/stack_layout.h>

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

} // namespace

StackLayout::StackLayout(Vector2f position, StackLayoutOrientation orientation)
    : children_{}, position_{position}, orientation_{orientation}, spacing_{0.0F}, padding_{0.0F},
      visible_{true}, enabled_{true}
{
}

void StackLayout::add(Widget* widget)
{
    if (widget == nullptr)
    {
        return;
    }

    children_.push_back(widget);
    layout();
}

void StackLayout::remove(Widget* widget)
{
    children_.erase(std::remove(children_.begin(), children_.end(), widget), children_.end());
    layout();
}

void StackLayout::clear()
{
    children_.clear();
}

void StackLayout::layout()
{
    Vector2f current_position{position_.x + padding_, position_.y + padding_};

    for (Widget* child : children_)
    {
        if (child == nullptr)
        {
            continue;
        }

        child->set_position(current_position);
        Rect child_bounds{child->get_bounds()};

        if (orientation_ == StackLayoutOrientation::Vertical)
        {
            current_position.y += static_cast<float>(child_bounds.height) + spacing_;
        }
        else
        {
            current_position.x += static_cast<float>(child_bounds.width) + spacing_;
        }
    }
}

void StackLayout::set_orientation(StackLayoutOrientation orientation)
{
    orientation_ = orientation;
    layout();
}

StackLayoutOrientation StackLayout::get_orientation() const
{
    return orientation_;
}

void StackLayout::set_spacing(float spacing)
{
    spacing_ = std::max(0.0F, spacing);
    layout();
}

float StackLayout::get_spacing() const
{
    return spacing_;
}

void StackLayout::set_padding(float padding)
{
    padding_ = std::max(0.0F, padding);
    layout();
}

float StackLayout::get_padding() const
{
    return padding_;
}

std::size_t StackLayout::get_child_count() const
{
    return children_.size();
}

void StackLayout::update(double delta_time)
{
    if (!visible_ || !enabled_)
    {
        return;
    }

    for (Widget* child : children_)
    {
        if (child != nullptr)
        {
            child->update(delta_time);
        }
    }
}

void StackLayout::draw()
{
    if (!visible_)
    {
        return;
    }

    for (Widget* child : children_)
    {
        if (child != nullptr)
        {
            child->draw();
        }
    }
}

void StackLayout::draw(double delta_time)
{
    if (!visible_)
    {
        return;
    }

    for (Widget* child : children_)
    {
        if (child != nullptr)
        {
            child->draw(delta_time);
        }
    }
}

void StackLayout::move(const Vector2f move_vector)
{
    position_.move(move_vector);
    for (Widget* child : children_)
    {
        if (child != nullptr)
        {
            child->move(move_vector);
        }
    }
}

void StackLayout::set_position(const Vector2f position)
{
    position_ = position;
    layout();
}

Rect StackLayout::get_bounds()
{
    float       width{0.0F};
    float       height{0.0F};
    std::size_t valid_child_count{0U};

    for (Widget* child : children_)
    {
        if (child == nullptr)
        {
            continue;
        }

        Rect child_bounds{child->get_bounds()};
        if (orientation_ == StackLayoutOrientation::Vertical)
        {
            width = std::max(width, static_cast<float>(child_bounds.width));
            height += static_cast<float>(child_bounds.height);
        }
        else
        {
            width += static_cast<float>(child_bounds.width);
            height = std::max(height, static_cast<float>(child_bounds.height));
        }

        ++valid_child_count;
    }

    if (valid_child_count > 1U)
    {
        const float total_spacing{spacing_ * static_cast<float>(valid_child_count - 1U)};
        if (orientation_ == StackLayoutOrientation::Vertical)
        {
            height += total_spacing;
        }
        else
        {
            width += total_spacing;
        }
    }

    return Rect{position_, to_rect_size(width + (2.0F * padding_)),
                to_rect_size(height + (2.0F * padding_))};
}

void StackLayout::set_visible(bool visible)
{
    visible_ = visible;
}

bool StackLayout::is_visible() const
{
    return visible_;
}

void StackLayout::set_enabled(bool enabled)
{
    enabled_ = enabled;
}

bool StackLayout::is_enabled() const
{
    return enabled_;
}

} // namespace gui

} // namespace rinvid
