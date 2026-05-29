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

#include <rinvid/gui/progress_bar.h>

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

ProgressBar::ProgressBar(Vector2f position, float width, float height, float min_value,
                         float max_value, float value)
    : position_{position}, width_{std::max(0.0F, width)}, height_{std::max(0.0F, height)},
      min_value_{std::min(min_value, max_value)}, max_value_{std::max(min_value, max_value)},
      value_{value}, border_thickness_{0.0F}, background_color_{0x333333FF},
      fill_color_{0x4AA3FFFF}, border_color_{0xFFFFFFFF}, background_{nullptr}, fill_{nullptr},
      border_{nullptr}, visible_{true}, enabled_{true}
{
    value_ = clamp_value(value_);
    update_shapes();
}

void ProgressBar::set_size(float width, float height)
{
    width_  = std::max(0.0F, width);
    height_ = std::max(0.0F, height);
    update_shapes();
}

float ProgressBar::get_width() const
{
    return width_;
}

float ProgressBar::get_height() const
{
    return height_;
}

void ProgressBar::set_range(float min_value, float max_value)
{
    min_value_ = std::min(min_value, max_value);
    max_value_ = std::max(min_value, max_value);
    set_value(value_);
}

float ProgressBar::get_min_value() const
{
    return min_value_;
}

float ProgressBar::get_max_value() const
{
    return max_value_;
}

void ProgressBar::set_value(float value)
{
    const float clamped_value{clamp_value(value)};
    if (value_ == clamped_value)
    {
        return;
    }

    value_ = clamped_value;
    update_shapes();
}

float ProgressBar::get_value() const
{
    return value_;
}

void ProgressBar::set_progress(float progress)
{
    const float clamped_progress{std::clamp(progress, 0.0F, 1.0F)};
    set_value(min_value_ + (clamped_progress * (max_value_ - min_value_)));
}

float ProgressBar::get_progress() const
{
    if (max_value_ == min_value_)
    {
        return 0.0F;
    }

    return (value_ - min_value_) / (max_value_ - min_value_);
}

void ProgressBar::set_background_color(Color color)
{
    background_color_ = color;
    update_shapes();
}

void ProgressBar::set_fill_color(Color color)
{
    fill_color_ = color;
    update_shapes();
}

void ProgressBar::set_border_color(Color color)
{
    border_color_ = color;
    update_shapes();
}

void ProgressBar::set_border_thickness(float thickness)
{
    border_thickness_ = std::max(0.0F, thickness);
    update_shapes();
}

float ProgressBar::get_border_thickness() const
{
    return border_thickness_;
}

void ProgressBar::draw()
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

    if (fill_ != nullptr)
    {
        fill_->draw();
    }
}

void ProgressBar::draw(double delta_time)
{
    (void)delta_time;
    draw();
}

void ProgressBar::move(const Vector2f move_vector)
{
    position_.move(move_vector);
    update_shapes();
}

void ProgressBar::set_position(const Vector2f position)
{
    position_ = position;
    update_shapes();
}

Rect ProgressBar::get_bounds()
{
    return Rect{position_, to_rect_size(width_), to_rect_size(height_)};
}

void ProgressBar::set_visible(bool visible)
{
    visible_ = visible;
}

bool ProgressBar::is_visible() const
{
    return visible_;
}

void ProgressBar::set_enabled(bool enabled)
{
    enabled_ = enabled;
}

bool ProgressBar::is_enabled() const
{
    return enabled_;
}

float ProgressBar::clamp_value(float value) const
{
    if (max_value_ == min_value_)
    {
        return min_value_;
    }

    return std::clamp(value, min_value_, max_value_);
}

void ProgressBar::update_shapes()
{
    border_.reset();
    background_.reset();
    fill_.reset();

    if (width_ <= 0.0F || height_ <= 0.0F)
    {
        return;
    }

    const float clamped_border_thickness{
        std::min(border_thickness_, std::min(width_, height_) / 2.0F)};

    if (clamped_border_thickness > 0.0F)
    {
        border_ = std::make_unique<RectangleShape>(get_center(position_, width_, height_), width_,
                                                   height_);
        border_->set_color(border_color_);
    }

    const float content_width{width_ - (2.0F * clamped_border_thickness)};
    const float content_height{height_ - (2.0F * clamped_border_thickness)};
    if (content_width <= 0.0F || content_height <= 0.0F)
    {
        return;
    }

    Vector2f content_position{position_.x + clamped_border_thickness,
                              position_.y + clamped_border_thickness};
    background_ = std::make_unique<RectangleShape>(
        get_center(content_position, content_width, content_height), content_width, content_height);
    background_->set_color(background_color_);

    const float fill_width{content_width * get_progress()};
    if (fill_width <= 0.0F)
    {
        return;
    }

    fill_ = std::make_unique<RectangleShape>(Vector2f{content_position.x + (fill_width / 2.0F),
                                                      content_position.y + (content_height / 2.0F)},
                                             fill_width, content_height);
    fill_->set_color(fill_color_);
}

} // namespace gui

} // namespace rinvid
