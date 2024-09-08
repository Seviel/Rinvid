/**********************************************************************
 * Copyright (c) 2022 - 2024, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <iostream>

#include <SFML/Window.hpp>

#include "include/button.h"
#include "util/include/collision_detection.h"
#include "util/include/vector2.h"

namespace rinvid
{

namespace gui
{

Button::Button() : idle_{}, mouse_hovering_{}, clicked_{}, is_clicked_{false}
{
    sprite_animation_.add_animation("idle", idle_);
    sprite_animation_.add_animation("mouse_hovering", mouse_hovering_);
    sprite_animation_.add_animation("clicked", clicked_);

    sprite_animation_.play("idle");
}

void Button::update(Vector2f mouse_pos)
{
    glm::vec4   glm_mouse_pos{mouse_pos.x, mouse_pos.y, 1.0F, 1.0F};
    const auto& view = glm::inverse(RinvidGfx::get_view());
    glm_mouse_pos    = view * glm_mouse_pos;
    mouse_pos.x      = glm_mouse_pos.x;
    mouse_pos.y      = glm_mouse_pos.y;
    rinvid::Rect position_rect{mouse_pos, 1, 1};

    if (intersects(bounding_rect(), position_rect))
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sprite_animation_.play("clicked");
            is_clicked_ = true;
        }
        else
        {
            sprite_animation_.play("mouse_hovering");
            is_clicked_ = false;
        }
    }
    else
    {
        sprite_animation_.play("idle");
        is_clicked_ = false;
    }
}

void Button::set_idle(const std::vector<Rect>& regions)
{
    idle_.setup(0.0, regions);
    sprite_animation_.set_animation_by_name("idle", idle_);
}

void Button::set_mouse_hovering(const std::vector<Rect>& regions)
{
    mouse_hovering_.setup(0.0, regions);
    sprite_animation_.set_animation_by_name("mouse_hovering", mouse_hovering_);
}

void Button::set_clicked(const std::vector<Rect>& regions)
{
    clicked_.setup(0.0, regions);
    sprite_animation_.set_animation_by_name("clicked", clicked_);
}

bool Button::is_clicked() const
{
    return is_clicked_;
}

} // namespace gui

} // namespace rinvid
