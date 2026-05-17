/**********************************************************************
 * Copyright (c) 2022 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <cstdint>

#include <glm/glm.hpp>

#include <rinvid/core/render_context.h>
#include <rinvid/gui/button.h>
#include <rinvid/system/mouse.h>
#include <rinvid/util/collision_detection.h>
#include <rinvid/util/vector2.h>

namespace rinvid
{

namespace gui
{

namespace
{

constexpr const char* IDLE_ANIMATION{"idle"};
constexpr const char* HOVERED_ANIMATION{"hovered"};
constexpr const char* PRESSED_ANIMATION{"pressed"};
constexpr const char* DISABLED_ANIMATION{"disabled"};

} // namespace

Button::Button()
    : idle_{}, hovered_{}, pressed_{}, disabled_{}, state_{ButtonState::Idle}, visible_{true},
      enabled_{true}, has_disabled_animation_{false}, mouse_was_down_{false},
      pressed_inside_{false}, was_pressed_{false}, was_released_{false}, was_activated_{false}
{
    sprite_animation_.add_animation(IDLE_ANIMATION, idle_);
    sprite_animation_.add_animation(HOVERED_ANIMATION, hovered_);
    sprite_animation_.add_animation(PRESSED_ANIMATION, pressed_);
    sprite_animation_.add_animation(DISABLED_ANIMATION, disabled_);

    sprite_animation_.play(IDLE_ANIMATION);
}

void Button::setup_from_atlas(Texture* texture, std::int32_t width, std::int32_t height,
                              Vector2f top_left, std::uint32_t state_count, Vector2f atlas_offset)
{
    Sprite::setup(texture, width, height, top_left);

    std::vector<Rect> regions{};
    regions.reserve(state_count);
    for (std::uint32_t index{0U}; index < state_count; ++index)
    {
        Rect region{};
        region.position.x =
            atlas_offset.x + (static_cast<float>(index) * static_cast<float>(width));
        region.position.y = atlas_offset.y;
        region.width      = width;
        region.height     = height;
        regions.push_back(region);
    }

    set_state_regions(regions);
}

void Button::update()
{
    clear_events();

    bool mouse_down{system::Mouse::is_button_pressed(system::Mouse::Left)};

    if (!visible_)
    {
        pressed_inside_ = false;
        mouse_was_down_ = mouse_down;
        set_state(ButtonState::Idle);
        return;
    }

    if (!enabled_)
    {
        pressed_inside_ = false;
        mouse_was_down_ = mouse_down;
        set_state(ButtonState::Disabled);
        return;
    }

    bool hovered{is_mouse_over()};

    if (mouse_down)
    {
        if (!mouse_was_down_)
        {
            pressed_inside_ = hovered;
            if (pressed_inside_)
            {
                was_pressed_ = true;
            }
        }

        if (hovered && pressed_inside_)
        {
            set_state(ButtonState::Pressed);
        }
        else if (hovered)
        {
            set_state(ButtonState::Hovered);
        }
        else
        {
            set_state(ButtonState::Idle);
        }
    }
    else
    {
        if (mouse_was_down_ && pressed_inside_)
        {
            was_released_ = true;
            was_activated_ = hovered;
        }

        pressed_inside_ = false;
        set_state(hovered ? ButtonState::Hovered : ButtonState::Idle);
    }

    mouse_was_down_ = mouse_down;
}

void Button::set_idle(const std::vector<Rect>& regions)
{
    idle_.setup(0.0, regions);
    sprite_animation_.set_animation_by_name(IDLE_ANIMATION, idle_);
    apply_state_animation();
}

void Button::set_hovered(const std::vector<Rect>& regions)
{
    hovered_.setup(0.0, regions);
    sprite_animation_.set_animation_by_name(HOVERED_ANIMATION, hovered_);
    apply_state_animation();
}

void Button::set_pressed(const std::vector<Rect>& regions)
{
    pressed_.setup(0.0, regions);
    sprite_animation_.set_animation_by_name(PRESSED_ANIMATION, pressed_);
    apply_state_animation();
}

void Button::set_disabled(const std::vector<Rect>& regions)
{
    disabled_.setup(0.0, regions);
    has_disabled_animation_ = !regions.empty();
    sprite_animation_.set_animation_by_name(DISABLED_ANIMATION, disabled_);
    apply_state_animation();
}

void Button::set_state_regions(const std::vector<Rect>& regions)
{
    if (regions.empty())
    {
        return;
    }

    set_idle({regions.at(0)});
    set_hovered({regions.size() > 1U ? regions.at(1) : regions.at(0)});
    set_pressed({regions.size() > 2U ? regions.at(2) : regions.back()});

    if (regions.size() > 3U)
    {
        set_disabled({regions.at(3)});
    }
    else
    {
        has_disabled_animation_ = false;
        apply_state_animation();
    }
}

void Button::set_enabled(bool enabled)
{
    enabled_ = enabled;
    if (!enabled_)
    {
        pressed_inside_ = false;
        set_state(ButtonState::Disabled);
    }
    else if (state_ == ButtonState::Disabled)
    {
        set_state(ButtonState::Idle);
    }
}

bool Button::is_enabled() const
{
    return enabled_;
}

void Button::set_visible(bool visible)
{
    visible_ = visible;
    if (!visible_)
    {
        pressed_inside_ = false;
        set_state(ButtonState::Idle);
    }
}

bool Button::is_visible() const
{
    return visible_;
}

ButtonState Button::get_state() const
{
    return state_;
}

bool Button::is_hovered() const
{
    return state_ == ButtonState::Hovered || state_ == ButtonState::Pressed;
}

bool Button::is_pressed() const
{
    return state_ == ButtonState::Pressed;
}

bool Button::was_pressed() const
{
    return was_pressed_;
}

bool Button::was_released() const
{
    return was_released_;
}

bool Button::was_activated() const
{
    return was_activated_;
}

void Button::draw()
{
    if (visible_)
    {
        Sprite::draw();
    }
}

void Button::draw(const Shader shader)
{
    if (visible_)
    {
        Sprite::draw(shader);
    }
}

void Button::draw(double delta_time)
{
    if (visible_)
    {
        Sprite::draw(delta_time);
    }
}

void Button::draw(double delta_time, const Shader shader)
{
    if (visible_)
    {
        Sprite::draw(delta_time, shader);
    }
}

void Button::clear_events()
{
    was_pressed_   = false;
    was_released_  = false;
    was_activated_ = false;
}

bool Button::is_mouse_over()
{
    Vector2f  mouse_pos = system::Mouse::get_mouse_pos();
    glm::vec4 glm_mouse_pos{mouse_pos.x, mouse_pos.y, 1.0F, 1.0F};

    RenderContext* render_context = RenderContext::get_active_context();
    if (render_context == nullptr)
    {
        return false;
    }

    const auto& view = glm::inverse(render_context->get_view());
    glm_mouse_pos    = view * glm_mouse_pos;
    mouse_pos.x      = glm_mouse_pos.x;
    mouse_pos.y      = glm_mouse_pos.y;

    Rect mouse_rect{mouse_pos, 1, 1};
    return intersects(bounding_rect(), mouse_rect);
}

void Button::set_state(ButtonState state)
{
    if (state_ == state)
    {
        return;
    }

    state_ = state;
    apply_state_animation();
}

void Button::apply_state_animation()
{
    switch (state_)
    {
        case ButtonState::Hovered:
            sprite_animation_.play(HOVERED_ANIMATION);
            break;
        case ButtonState::Pressed:
            sprite_animation_.play(PRESSED_ANIMATION);
            break;
        case ButtonState::Disabled:
            sprite_animation_.play(has_disabled_animation_ ? DISABLED_ANIMATION : IDLE_ANIMATION);
            break;
        case ButtonState::Idle:
        default:
            sprite_animation_.play(IDLE_ANIMATION);
            break;
    }
}

} // namespace gui

} // namespace rinvid
