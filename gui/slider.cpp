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

#include <glm/glm.hpp>

#include <rinvid/core/render_context.h>
#include <rinvid/gui/slider.h>
#include <rinvid/system/mouse.h>
#include <rinvid/util/collision_detection.h>

namespace rinvid
{

namespace gui
{

namespace
{

constexpr float DEFAULT_HEIGHT{24.0F};
constexpr float DEFAULT_TRACK_HEIGHT{6.0F};
constexpr float DEFAULT_THUMB_WIDTH{16.0F};
constexpr float DEFAULT_THUMB_HEIGHT{24.0F};

std::int32_t to_rect_size(float value)
{
    return static_cast<std::int32_t>(std::ceil(std::max(0.0F, value)));
}

} // namespace

Slider::Slider(Vector2f position, float width, float min_value, float max_value, float value)
    : position_{position}, width_{std::max(0.0F, width)}, height_{DEFAULT_HEIGHT},
      min_value_{std::min(min_value, max_value)}, max_value_{std::max(min_value, max_value)},
      value_{value}, step_{0.0F}, track_height_{DEFAULT_TRACK_HEIGHT},
      thumb_width_{DEFAULT_THUMB_WIDTH}, thumb_height_{DEFAULT_THUMB_HEIGHT},
      track_color_{0x333333FF}, fill_color_{0x4AA3FFFF}, thumb_color_{0xFFFFFFFF}, track_{nullptr},
      fill_{nullptr}, thumb_{nullptr}, visible_{true}, enabled_{true}, dragging_{false},
      mouse_was_down_{false}, was_value_changed_{false}
{
    height_ = std::max(height_, std::max(track_height_, thumb_height_));
    value_  = clamp_value(value_);
    update_shapes();
}

void Slider::set_size(float width, float height)
{
    width_  = std::max(0.0F, width);
    height_ = std::max(std::max(0.0F, height), std::max(track_height_, thumb_height_));
    update_shapes();
}

float Slider::get_width() const
{
    return width_;
}

float Slider::get_height() const
{
    return height_;
}

void Slider::set_range(float min_value, float max_value)
{
    min_value_ = std::min(min_value, max_value);
    max_value_ = std::max(min_value, max_value);
    set_value(value_);
}

float Slider::get_min_value() const
{
    return min_value_;
}

float Slider::get_max_value() const
{
    return max_value_;
}

void Slider::set_value(float value)
{
    const float clamped_value{clamp_value(value)};
    if (value_ == clamped_value)
    {
        return;
    }

    value_             = clamped_value;
    was_value_changed_ = true;
    update_shapes();
}

float Slider::get_value() const
{
    return value_;
}

void Slider::set_step(float step)
{
    step_ = std::max(0.0F, step);
    set_value(value_);
}

float Slider::get_step() const
{
    return step_;
}

void Slider::set_track_height(float height)
{
    track_height_ = std::max(0.0F, height);
    height_       = std::max(height_, track_height_);
    update_shapes();
}

float Slider::get_track_height() const
{
    return track_height_;
}

void Slider::set_thumb_size(float width, float height)
{
    thumb_width_  = std::max(0.0F, width);
    thumb_height_ = std::max(0.0F, height);
    height_       = std::max(height_, thumb_height_);
    update_shapes();
}

void Slider::set_track_color(Color color)
{
    track_color_ = color;
    update_shapes();
}

void Slider::set_fill_color(Color color)
{
    fill_color_ = color;
    update_shapes();
}

void Slider::set_thumb_color(Color color)
{
    thumb_color_ = color;
    update_shapes();
}

bool Slider::is_dragging() const
{
    return dragging_;
}

bool Slider::was_value_changed() const
{
    return was_value_changed_;
}

void Slider::update(double delta_time)
{
    (void)delta_time;

    was_value_changed_ = false;
    const bool mouse_down{system::Mouse::is_button_pressed(system::Mouse::Left)};

    if (!visible_ || !enabled_)
    {
        dragging_       = false;
        mouse_was_down_ = mouse_down;
        return;
    }

    Vector2f mouse_position{get_mouse_position()};

    if (!mouse_was_down_ && mouse_down && is_mouse_over(mouse_position))
    {
        dragging_ = true;
    }

    if (!mouse_down)
    {
        dragging_ = false;
    }

    if (dragging_)
    {
        set_value_from_position(mouse_position.x);
    }

    mouse_was_down_ = mouse_down;
}

void Slider::draw()
{
    if (!visible_)
    {
        return;
    }

    if (track_ != nullptr)
    {
        track_->draw();
    }

    if (fill_ != nullptr)
    {
        fill_->draw();
    }

    if (thumb_ != nullptr)
    {
        thumb_->draw();
    }
}

void Slider::draw(double delta_time)
{
    (void)delta_time;
    draw();
}

void Slider::move(const Vector2f move_vector)
{
    position_.move(move_vector);
    update_shapes();
}

void Slider::set_position(const Vector2f position)
{
    position_ = position;
    update_shapes();
}

Rect Slider::get_bounds()
{
    return Rect{position_, to_rect_size(width_), to_rect_size(height_)};
}

void Slider::set_visible(bool visible)
{
    visible_ = visible;
    if (!visible_)
    {
        dragging_ = false;
    }
}

bool Slider::is_visible() const
{
    return visible_;
}

void Slider::set_enabled(bool enabled)
{
    enabled_ = enabled;
    if (!enabled_)
    {
        dragging_ = false;
    }
}

bool Slider::is_enabled() const
{
    return enabled_;
}

float Slider::clamp_value(float value) const
{
    if (max_value_ == min_value_)
    {
        return min_value_;
    }

    float clamped_value{std::clamp(value, min_value_, max_value_)};
    if (step_ > 0.0F)
    {
        clamped_value = min_value_ + (std::round((clamped_value - min_value_) / step_) * step_);
        clamped_value = std::clamp(clamped_value, min_value_, max_value_);
    }

    return clamped_value;
}

float Slider::get_normalized_value() const
{
    if (max_value_ == min_value_)
    {
        return 0.0F;
    }

    return (value_ - min_value_) / (max_value_ - min_value_);
}

float Slider::get_track_width() const
{
    return std::max(0.0F, width_ - thumb_width_);
}

float Slider::get_track_left() const
{
    return position_.x + (thumb_width_ / 2.0F);
}

float Slider::get_track_center_y() const
{
    return position_.y + (height_ / 2.0F);
}

Vector2f Slider::get_mouse_position() const
{
    Vector2f  mouse_position{system::Mouse::get_mouse_pos()};
    glm::vec4 glm_mouse_position{mouse_position.x, mouse_position.y, 1.0F, 1.0F};

    RenderContext* render_context = RenderContext::get_active_context();
    if (render_context == nullptr)
    {
        return mouse_position;
    }

    const auto& view   = glm::inverse(render_context->get_view());
    glm_mouse_position = view * glm_mouse_position;
    mouse_position.x   = glm_mouse_position.x;
    mouse_position.y   = glm_mouse_position.y;

    return mouse_position;
}

bool Slider::is_mouse_over(Vector2f mouse_position)
{
    Rect mouse_rect{mouse_position, 1, 1};
    return intersects(get_bounds(), mouse_rect);
}

void Slider::set_value_from_position(float position_x)
{
    const float track_width{get_track_width()};
    if (track_width <= 0.0F)
    {
        set_value(min_value_);
        return;
    }

    const float normalized_value{
        std::clamp((position_x - get_track_left()) / track_width, 0.0F, 1.0F)};
    set_value(min_value_ + (normalized_value * (max_value_ - min_value_)));
}

void Slider::update_shapes()
{
    track_.reset();
    fill_.reset();
    thumb_.reset();

    if (width_ <= 0.0F || height_ <= 0.0F)
    {
        return;
    }

    const float track_width{get_track_width()};
    const float track_left{get_track_left()};
    const float track_center_y{get_track_center_y()};

    if (track_width > 0.0F && track_height_ > 0.0F)
    {
        track_ = std::make_unique<RectangleShape>(
            Vector2f{track_left + (track_width / 2.0F), track_center_y}, track_width,
            track_height_);
        track_->set_color(track_color_);

        const float fill_width{track_width * get_normalized_value()};
        if (fill_width > 0.0F)
        {
            fill_ = std::make_unique<RectangleShape>(
                Vector2f{track_left + (fill_width / 2.0F), track_center_y}, fill_width,
                track_height_);
            fill_->set_color(fill_color_);
        }
    }

    if (thumb_width_ > 0.0F && thumb_height_ > 0.0F)
    {
        const float thumb_center_x{track_left + (track_width * get_normalized_value())};
        thumb_ = std::make_unique<RectangleShape>(Vector2f{thumb_center_x, track_center_y},
                                                  thumb_width_, thumb_height_);
        thumb_->set_color(thumb_color_);
    }
}

} // namespace gui

} // namespace rinvid
